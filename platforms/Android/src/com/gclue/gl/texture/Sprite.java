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

import android.graphics.Bitmap;
import android.graphics.Rect;

/**
 * 文字列が描画された範囲を格納するためのクラス.
 * @author GClue, Inc.
 */
public class Sprite {
	/**
	 * 描画された文字列.
	 */
	private String text;
	
	/**
	 * 画像データ.
	 */
	private Bitmap bitmap;
	
	/**
	 * 描画された範囲.
	 */
	private Rect rect;
	
	/**
	 * 文字列が回転されて描画されたときの値.
	 */
	private int rotate;
	
	/**
	 * フォントサイズ.
	 */
	private float fontSize;
	
	/**
	 * 赤色成分.
	 */
	private int r;
	
	/**
	 * 緑色成分.
	 */
	private int g;
	
	/**
	 * 青色成分.
	 */
	private int b;
	
	/**
	 * 横幅.
	 */
	private int width;
	
	/**
	 * 縦幅.
	 */
	private int height;
	
	/**
	 * 配置された.
	 */
	private boolean decision;
	
	/**
	 * Sprite範囲を設定します.
	 * @param left left
	 * @param top top
	 * @param right right
	 * @param bottom bottom
	 */
	public void setBound(int left, int top, int right, int bottom) {
		setRect(new Rect(left, top, right, bottom));
	}
	
	/**
	 * 文字列が描画されたx座標を取得します.
	 * @return x座標
	 */
	public int getLeft() {
		return rect.left + 1;
	}
	
	/**
	 * 文字列が描画されたx座標を取得します.
	 * @return x座標
	 */
	public int getRight() {
		return rect.right - 1;
	}
	
	/**
	 * 文字列が描画されたy座標を取得します.
	 * @return y座標
	 */
	public int getTop() {
		return rect.top + 1;
	}
	
	/**
	 * 文字列が描画されたy座標を取得します.
	 * @return y座標
	 */
	public int getBottom() {
		return rect.bottom - 1;
	}
	
	/**
	 * 描画された文字列を取得します.
	 * @return 文字列
	 */
	public String getText() {
		return text;
	}

	/**
	 * 描画された文字列を設定します.
	 * @param text 文字列
	 */
	public void setText(String text) {
		this.text = text;
	}

	/**
	 * 描画された文字列の回転角度を取得します.
	 * @return 角度
	 */
	public int getRotate() {
		return rotate;
	}

	/**
	 * 描画された文字列の回転角度を設定します.
	 * @param rotate 角度
	 */
	public void setRotate(int rotate) {
		this.rotate = rotate;
	}

	/**
	 * 描画された範囲を取得します.
	 * @return 範囲
	 */
	public Rect getRect() {
		return rect;
	}
	
	/**
	 * 文字列が描画された範囲を設定します.
	 * @param rect 範囲
	 */
	public void setRect(Rect rect) {
		this.rect = rect;
	}

	/**
	 * テクスチャの横幅を取得します.
	 * @return テクスチャの横幅
	 */
	public int getWidth() {
		return width;
	}
	
	public void setWidth(int width) {
		this.width = width;
	}
	
	/**
	 * テクスチャの縦幅を取得します.
	 * @return テクスチャの縦幅
	 */
	public int getHeight() {
		return height;
	}
	
	public void setHeight(int height) {
		this.height = height;
	}
	
	public float getFontSize() {
		return fontSize;
	}

	public void setFontSize(float fontSize) {
		this.fontSize = fontSize;
	}

	public void setRGB(int r, int g, int b) {
		this.r = r;
		this.g = g;
		this.b = b;
	}
	
	public int getR() {
		return r;
	}

	public int getG() {
		return g;
	}

	public int getB() {
		return b;
	}

	public boolean isDecision() {
		return decision;
	}

	public void setDecision(boolean decision) {
		this.decision = decision;
	}

	public Bitmap getBitmap() {
		return bitmap;
	}

	public void setBitmap(Bitmap bitmap) {
		this.bitmap = bitmap;
	}
	
	public void setPosition(int x, int y) {
		setBound(x, y, x + width, y + height);
	}
}
