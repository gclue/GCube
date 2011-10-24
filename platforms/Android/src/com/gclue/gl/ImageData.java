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

package com.gclue.gl;

import java.io.InputStream;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

/**
 * テクスチャの情報をJNI側に渡すためのクラス.
 */
public class ImageData {
	
	static {
		pixels = new int[1024*1024];
	}
	
	/**
	 * デバック用タグ.
	 */
	private static final String TAG = "ImageData";
	
	/**
	 * テクスチャのピクセル情報.
	 */
	private static int[] pixels;
	
	/**
	 * テクスチャの横幅.
	 */
	private int width;
	
	/**
	 * テクスチャの縦幅.
	 */
	private int height;
	
	/**
	 * テクスチャのピクセル情報を取得します.
	 * @return ピクセルデータ
	 */
	public int[] getPixels() {
		return pixels;
	}
	
	/**
	 * テクスチャの横幅を取得します.
	 * @return テクスチャの横幅
	 */
	public int getWidth() {
		return width;
	}
	
	/**
	 * テクスチャの縦幅を取得します.
	 * @return テクスチャの縦幅
	 */
	public int getHeight() {
		return height;
	}
	
	/**
	 * 指定されたファイル名のテクスチャをassetsから読み込みます.
	 * @param context コンテキスト
	 * @param fileName ファイル名
	 */
	public boolean loadImage(Context context, String fileName) {
		
		Bitmap b = null;
		BitmapFactory.Options opt = null;
		opt = new BitmapFactory.Options(); 
		opt.inScaled = false; 
		
		InputStream is = null;
		try {
			is = context.getResources().getAssets().open(fileName);
			b = BitmapFactory.decodeStream(is, null, opt);
			
			// Bitmapをint型の配列取得する
			width = b.getWidth();
			height = b.getHeight();
			
			if ((width & (width - 1)) == 0 && (height & (height - 1)) == 0 && width == height) {
				// 2のべき乗
			} else {
				int size = 2;
				// 2のべき乗ではない
				if (width > height) {
					while (size < width) {
						size *= 2;
					}
				} else {
					while (size < height) {
						size *= 2;
					}
				}
			}
			
			if (pixels == null) {
				pixels = new int[1024 * 1024];
			}
			
			b.getPixels(pixels, 0, width, 0, 0, width, height);
		} catch (Exception e) {
			Log.w(TAG, "loadImage:"+fileName, e);
			return false;
		} catch (OutOfMemoryError e) {
			Log.w(TAG, "loadImage:OutOfMemoryError", e);
			pixels = null;
			return false;
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (Exception e) {
					// do noting.
				}
			}
			if (b!=null) {
				b.recycle();
			}
			is = null;
			b = null;
			opt = null;
		}
		return true;
	}
	
	/**
	 * 指定されたリソースIDのテクスチャを読み込みます.
	 * @param context コンテキスト
	 * @param id リソースID
	 */
	public boolean loadImage(Context context, int id) {
		Resources r = null;
		BitmapFactory.Options opt = null;
		Bitmap b = null;
		try {
			// 画像を取得する
			r = context.getResources();
			opt = new BitmapFactory.Options(); 
			opt.inScaled = false; 
			b = BitmapFactory.decodeResource(r, id, opt);
			// Bitmapをint型の配列取得する
			return loadBitmap(b);
		} catch (OutOfMemoryError e) {
			Log.w(TAG, "loadImage:OutOfMemoryError", e);
			pixels = null;
			return false;
		} finally {
			// メモリ解放!
			if (b != null) {
				b.recycle();
			}
			b = null;
			opt = null;
			r = null;
		}
	}
	
	/**
	 * 指定されたBitmapからint型の配列を取得します.
	 * @param bitmap int型の配列を取得するBitmap
	 * @return 取得の成功フラグ
	 */
	public boolean loadBitmap(Bitmap bitmap) {
		try {
			width = bitmap.getWidth();
			height = bitmap.getHeight();
			if (pixels == null) {
				pixels = new int[1024 * 1024];
			}
			bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
		} catch (OutOfMemoryError e) {
			Log.w(TAG, "loadImage:OutOfMemoryError", e);
			pixels = null;
			return false;
		}
		
		return true;
	}
	
	/**
	 * 指定されたバイナリデータからbitmapを読み込みます.
	 * @param data bitmapのバイナリデータ
	 * @return bitmapの作成の正否
	 */
	public boolean loadImage(byte[] data) {
		BitmapFactory.Options opt = null;
		Bitmap b = null;
		try {
			// 画像を取得する
			opt = new BitmapFactory.Options(); 
			opt.inScaled = false; 
			b = BitmapFactory.decodeByteArray(data, 0, data.length, opt);
			// Bitmapをint型の配列取得する
			return loadBitmap(b);
		} catch (OutOfMemoryError e) {
			Log.w(TAG, "loadImage:OutOfMemoryError", e);
			pixels = null;
			return false;
		} finally {
			// メモリ解放!
			if (b != null) {
				b.recycle();
			}
			b = null;
			opt = null;
		}
	}

	public static void setNull() {
		pixels = null;
//		System.gc();
//		System.runFinalization();
	}
}
