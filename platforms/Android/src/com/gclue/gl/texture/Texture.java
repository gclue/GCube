/*
 * The MIT License (MIT)
 * Copyright (c) 2011 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.gclue.gl.texture;

import java.util.ArrayList;
import java.util.List;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.FontMetrics;
import android.graphics.Rect;
import android.util.Log;

import com.gclue.gl.ImageData;

/**
 * 文字列のテクスチャを作成するためのクラス.
 * @author GClue, Inc.
 */
public class Texture {
	/**
	 * デバック用フラグ.
	 */
	private static final boolean DEBUG = true;
	
	/**
	 * デバック用タグを定義します.
	 */
	private static final String TAG = "GLLibaray";
	
	/**
	 * 描画を行う為のキャンバス.
	 */
	private Canvas canvas;
	
	/**
	 * 描画設定を行う為のPaint.
	 */
	private Paint paint = new Paint();
	
	/**
	 * 描画先のBitmap.
	 */
	private Bitmap bitmap;
	
	/**
	 * 描画を行った文字列のリスト.
	 */
	private List<Sprite> list = new ArrayList<Sprite>();
	
	/**
	 * ソートのアルゴリズム.
	 */
	private ISpriteSortable sort = new TextSpriteSort();
	
	/**
	 * 最適化フラグ.
	 */
	private boolean optimize;
	
	/**
	 * テキストが長いときに改行するサイズ.
	 */
	private int brSize;
	
	/**
	 * Bitmapを作成します.
	 * すでに作成されている場合には前のBitmapを破棄します
	 * @param width 横幅
	 * @param height 縦幅
	 */
	public void createBitmap(int width, int height) {
		if (bitmap != null) {
			bitmap.recycle();
		}
		bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		canvas = new Canvas(bitmap);
		paint = new Paint();
		if (brSize == 0) {
			brSize = width - 2;
		}
	}
	
	/**
	 * Bitmapを破棄します.
	 */
	public void disposeBitmap() {
		if (bitmap != null) {
			bitmap.recycle();
		}
		for (int i = 0; i < list.size(); i++) {
			Bitmap b = list.get(i).getBitmap();
			if (b != null) {
				b.recycle();
			}
		}
		list.clear();
		bitmap = null;
		canvas = null;
		paint = null;
	}
	
	/**
	 * 文字列がかかれたBitmapを取得します
	 * @return Bitmap
	 */
	public Bitmap getBitmap() {
		return bitmap;
	}
	
	/**
	 * テキストの改行サイズを設定します.
	 * addDrawTextで追加されたテキストは、ここで設定されたサイズを超えた場合には
	 * 改行して高さが増えます.
	 * @param size
	 */
	public void setBrSize(int size) {
		brSize =size;
	}
	
	/**
	 * 描画した文字列数を取得します.
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @return 文字列数
	 */
	public int getCount() {
		return list.size();
	}
	
	/**
	 * 描画した文字列のデータを取得します.
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @param index インデックス
	 * @return 文字列のデータ
	 */
	public Sprite getSprite(int index) {
		return list.get(index);
	}
	
	/**
	 * 文字列を描画したテクスチャを取得します.
	 * テクスチャの取得に失敗した場合にはnullを返却します.
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @return 文字列を描画したテクスチャ
	 */
	public ImageData getImageData() {
		try {
			Bitmap b = getBitmap();
			if (b == null) {
				return null;
			}
			ImageData data = new ImageData();
			if (data.loadBitmap(b)) {
				return data;
			}
		} catch (Throwable e) {
			Log.e(TAG, "getStringTexture:", e);
		} finally {
			disposeBitmap();
		}
		return null;
	}
	
	/**
	 * 文字列の並べ替えを行い最適化してから実際にテクスチャに描画を行います.
	 */
	public void optimize() {
		// Sprite配置を初期化
		clearSpriteData();
		
		// ソートを行う
		sort.sort(list, new Rect(1, 1, bitmap.getWidth() - 2, bitmap.getHeight() - 2));
		
		// すべてのサイズが決定できなかった場合には、
		// テクスチャを大きくして再度ソートしてみる
		if (!isDecision()) {
			if (DEBUG) {
				Log.w(TAG, "Sprite can not deploy.");
			}
			
			// 前のテクスチャ用のビットマップを削除
			disposeBitmap();
			
			// 前のテクスチャを2倍のサイズに変更
			int w = bitmap.getWidth() * 2;
			int h = bitmap.getHeight() * 2;
			createBitmap(w, h);
			
			// 最適化
			optimize();
		} else {
			if (optimize) {
				return;
			}
			optimize = true;
			
			final int size = list.size();
			for (int i = 0; i < size; i++) {
				draw(list.get(i));
			}
		}
	}
	
	/**
	 * テクスチャに文字列を描画します.
	 * @param text テキスト
	 * @param fontSize フォントサイズ
	 * @param r 赤色成分(0〜255)
	 * @param g 緑色成分(0〜255)
	 * @param b 青色成分(0〜255)
	 * @return 追加した文字列のSpriteデータ
	 */
	public Sprite addDrawText(String text, float fontSize, int r, int g, int b) {
		// 追加された文字列がnullの場合は、適当な文字列に変更
		if (text == null) {
			text = "no-data";
		}
		
		paint.setTextSize(fontSize);
		paint.setColor(Color.argb(255, r, g, b));
		paint.setAntiAlias(true);
		
		FontMetrics fontMetrics = paint.getFontMetrics();
		
		int width = (int) paint.measureText(text);
		int height = (int) (fontSize + fontMetrics.bottom);
		
		if (width > brSize) {
			int maxWidth = brSize;
			int brPoint = Integer.MAX_VALUE;
			int currentIndex = 0;
			while (brPoint != 0) {
				String mesureString = text.substring(currentIndex);
				brPoint = paint.breakText(mesureString, true, maxWidth, null);
				if (brPoint != 0) {
					currentIndex += brPoint;
					height += (int) (fontSize + fontMetrics.bottom);
				}
			}
			width = brSize;
		}
		
		Sprite sd = new Sprite();
		sd.setWidth(width + 2);
		sd.setHeight(height + 2);
		sd.setFontSize(fontSize);
		sd.setRGB(r, g, b);
		sd.setText(text);
		
		list.add(sd);
		return sd;
	}
	
	/**
	 * Bitmapをテクスチャに追加します.
	 * @param bitmap 追加するBitmap
	 * @param width 横幅
	 * @param height　縦幅
	 * @return 追加したBitmapのSpriteデータ
	 */
	public Sprite addDrawBitmap(Bitmap bitmap, int width, int height) {
		Sprite sd = new Sprite();
		sd.setWidth(width + 2);
		sd.setHeight(height + 2);
		sd.setBitmap(bitmap);
		list.add(sd);
		return sd;
	}
	
	/**
	 * 配置が決定しているフラグをすべて解除します.
	 */
	private void clearSpriteData() {
		for (int i = 0; i < list.size(); i++) {
			list.get(i).setDecision(false);
		}
	}
	
	/**
	 * すべてのSpriteの配置が決定したかチェックします。
	 * @return true 決定した
	 */
	private boolean isDecision() {
		for (int i = 0; i < list.size(); i++) {
			if (!list.get(i).isDecision()) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * 指定された文字列データをテクスチャに書き込みます.
	 * @param sprite テキストデータ
	 */
	private void draw(Sprite sprite) {
		int x = sprite.getLeft();
		int y = sprite.getTop();
		
		if (sprite.getText() != null) {
			String t = sprite.getText();
			float s = sprite.getFontSize();
			int r = sprite.getR();
			int g = sprite.getG();
			int b = sprite.getB();
			draw(t, x, y, s, r, g, b);
		} else if (sprite.getBitmap() != null) {
			canvas.drawBitmap(sprite.getBitmap(), x, y, paint);
		}
	}
	
	/**
	 * 指定された文字列データをテクスチャに書き込みます.
	 * @param text 文字列
	 * @param x 描画を行うx座標
	 * @param y 描画を行うy座標
	 * @param fontSize フォントサイズ
	 * @param r 赤色成分(0〜255)
	 * @param g 緑色成分(0〜255)
	 * @param b 青色成分(0〜255)
	 */
	private void draw(String text, int x, int y, float fontSize, int r, int g, int b) {
		FontMetrics fontMetrics = paint.getFontMetrics();
		
		paint.setTextSize(fontSize);
		paint.setColor(Color.argb(255, r, g, b));
		paint.setAntiAlias(true);
		paint.setFakeBoldText(true);
		
		int maxWidth = brSize;
		int brPoint = Integer.MAX_VALUE;
		int currentIndex = 0;
		int linePointY = (int) fontSize;
		while (brPoint != 0) {
			String mesureString = text.substring(currentIndex);
			brPoint = paint.breakText(mesureString, true, maxWidth, null);
			if (brPoint != 0) {
				String line = text.substring(currentIndex, currentIndex + brPoint);
				canvas.drawText(line, x, y + linePointY, paint);
				linePointY += (int) fontSize + fontMetrics.bottom;
				currentIndex += brPoint;
			}
		}
	}
}
