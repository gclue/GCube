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

package com.gclue.gl.ui;

import com.gclue.gl.animation.GCAnimation;
import com.gclue.gl.util.CoordinateTransform;
import com.gclue.gl.util.Pointf;

/**
 * Viewの基底クラス.
 * @author GClue, Inc.
 */
public abstract class GCView extends GCObject {
	/**
	 * 親View.
	 */
	protected GCView parent;
	
	/**
	 * コンストラクタ.
	 */
	public GCView() {
	}
	
	/**
	 * Viewの表示位置を設定します.
	 * OpenGLESの座標系なるので、{@link CoordinateTransform#toGLPoint(int, int)}を使用して座標系を変換してください。
	 * @param x x座標
	 * @param y y座標
	 */
	public void setPosition(float x, float y) {
		nativeSetPosition(pear, x, y);
	}
	
	/**
	 * Viewの表示位置を設定します.
	 * @param point 表示する座標
	 */
	public void setPosition(Pointf point) {
		setPosition(point.x, point.y);
	}
	
	/**
	 * Viewの表示位置を取得します.
	 * @return 表示位置
	 */
	public Pointf getPosition() {
		Pointf point = new Pointf();
		point.x = nativeGetX(pear);
		point.y = nativeGetY(pear);
		return point;
	}
	
	/**
	 * Viewのサイズを設定します.
	 * この値は、Viewのタッチの当たり判定などに使用されます。
	 * @param point サイズ
	 */
	public void setSize(Pointf point) {
		nativeSetSize(pear, point.x, point.y);
	}
	
	/**
	 * Viewのサイズを設定します.
	 * @param w 横幅
	 * @param h 縦幅
	 */
	public void setSize(float w, float h) {
		nativeSetSize(pear, w, h);
	}
	
	/**
	 * Viewの横幅を取得します.
	 * @return 横幅
	 */
	public float getWidth() {
		return nativeGetWidth(pear);
	}
	
	/**
	 * Viewの縦幅を取得します.
	 * @return 縦幅
	 */
	public float getHeight() {
		return nativeGetHeight(pear);
	}
	
	/**
	 * Viewのサイズを取得します.
	 * @return サイズ
	 */
	public Pointf getSize() {
		Pointf p = new Pointf();
		p.x = getWidth();
		p.y = getHeight();
		return p;
	}
	
	/**
	 * Viewのスケールを設定します.
	 * @param xscale x軸方向へのスケール
	 * @param yscale y軸方向へのスケール
	 */
	public void setScale(float xscale, float yscale) {
		nativeSetScale(pear, xscale, yscale);
	}
	
	/**
	 * Viewのスケールを取得します.
	 * @return スケール
	 */
	public Pointf getScale() {
		Pointf p = new Pointf();
		p.x = nativeGetScaleX(pear);
		p.y = nativeGetScaleY(pear);
		return p;
	}
	
	/**
	 * Viewの回転角度を設定します.
	 * @param rotate 角度(0〜360)
	 */
	public void setRotate(float rotate) {
		nativeSetRotate(pear, rotate);
	}
	
	/**
	 * Viewの回転角度を取得します.
	 * @return 角度
	 */
	public float getRotate() {
		return nativeGetRotate(pear);
	}
	
	/**
	 * Viewのアルファ値を設定します.
	 * @param alpha アルファ値
	 */
	public void setAlpha(float alpha) {
		nativeSetAlpha(pear, alpha);
	}
	
	/**
	 * Viewのアルファ値を取得します.
	 * @return アルファ値
	 */
	public float getAlpha() {
		return nativeGetAlpha(pear);
	}
	
	/**
	 * Viewのクリック判定をON/OFFします.
	 * falseを設定するとタッチイベントが発生しなくなるので、GCButtonなどが反応しなくなります。
	 * @param clickable クリックフラグ
	 */
	public void setClickable(boolean clickable) {
		nativeSetClickable(pear, clickable);
	}
	
	/**
	 * Viewのクリック判定の設定を取得します.
	 * @return クリック判定フラグ
	 */
	public boolean getClickable() {
		return nativeGetClickable(pear);
	}
	
	/**
	 * Viewの表示フラグを設定します.
	 * @param visible 表示設定
	 */
	public void setVisible(boolean visible) {
		nativeSetVisible(pear, visible);
	}
	
	/**
	 * Viewの表示設定を取得します.
	 * @return 表示設定
	 */
	public boolean getVisible() {
		return nativeGetVisible(pear);
	}
	
	/**
	 * Viewのアニメーションを開始します.
	 * @param animation 開始するアニメーション
	 */
	public void startAnimation(GCAnimation animation) {
		nativeStartAnimation(pear, animation.pear);
	}
	
	/**
	 * Viewのアニメーションを停止します.
	 */
	public void stopAnimaiton() {
		nativeStopAnimation(pear);
	}
	
	/**
	 * ユーザIDを設定します.
	 * @param id ID
	 */
	public void setUserID(int id) {
		nativeSetUserID(pear, id);
	}
	
	/**
	 * ユーザIDを取得します.
	 * @return ユーザID
	 */
	public int getUserID() {
		return nativeGetUserID(pear);
	}
	
	/**
	 * 指定されたIDのViewを取得します.
	 * 指定されたIDと違う場合にはnullを返却します.
	 * @param id ID
	 * @return GCViewのインスタンス
	 */
	public GCView findViewByID(int id) {
		if (getUserID() == id) {
			return this;
		}
		return null;
	}
	
	private native void nativeSetPosition(int pear, float x, float y);
	private native void nativeSetScale(int pear, float xscale, float yscale);
	private native void nativeSetRotate(int pear, float rotate);
	private native void nativeSetAlpha(int pear, float alpha);
	private native void nativeSetSize(int pear, float w, float h);
	private native void nativeSetClickable(int pear, boolean clickable);
	private native void nativeSetVisible(int pear, boolean visible);
	private native void nativeSetUserID(int pear, int id);
	
	private native float nativeGetX(int pear);
	private native float nativeGetY(int pear);
	private native float nativeGetWidth(int pear);
	private native float nativeGetHeight(int pear);
	private native float nativeGetScaleX(int pear);
	private native float nativeGetScaleY(int pear);
	private native float nativeGetRotate(int pear);
	private native float nativeGetAlpha(int pear);
	private native boolean nativeGetClickable(int pear);
	private native boolean nativeGetVisible(int pear);
	private native int nativeGetUserID(int pear);
	
	private native void nativeStartAnimation(int pear, int pearAnimation);
	private native void nativeStopAnimation(int pear);
}
