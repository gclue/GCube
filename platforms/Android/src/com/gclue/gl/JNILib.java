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


import com.gclue.gl.app.NDKInterface;

/**
 * JNIのインターフェイス.
 * @author GClue, Inc.
 */
public final class JNILib {
	/**
	 * このクラスのインスタンスは作成させないのでprivateとしておく
	 */
	private JNILib() {
	}
	
	// 定義はしてあるが、ここではなにもせず、Cのコードの関数を呼び出す.
	/**
	 * @param width the current view width
	 * @param height the current view height
	 */
	public static native boolean init(int width, int height);

	/**
	 * JNI側の初期化処理を行います.
	 * @param ndkInterface
	 */
	public synchronized static native void setInterface(NDKInterface ndkInterface);

	/**
	 * JNIにステップ実行を行います.
	 * @param dt 経過時間
	 */
	public static native void step(double dt);

	/**
	 * バックボタンをクリックした時のイベント
	 * @return 処理をした場合はtrue,しないでシステムに返す場合はfalse
	 */
	public synchronized static native boolean onPressBackKey();

	/**
	 * 停止処理.
	 */
	public synchronized static native void onPause();

	/**
	 * 開始処理.
	 */
	public synchronized static native void onResume();

	/**
	 * JNI側にタッチイベントを通知します.
	 * @param action アクション
	 * @param x x座標
	 * @param y y座標
	 * @param time イベント発生時間
	 */
	public static native void touchEvent(int action, float x, float y, long time);
	
	/**
	 * シャットダウン処理を行います.
	 */
	public static native void shutDown();
	
	/**
	 * JNIにイベントを配送します.
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	public synchronized static native void sendGameEvent(int type, int param1, int param2, int param3, int param4, String param5);
	
	/**
	 * JNIにHttpリクエストのレスポンスを返却します.
	 * @param response レスポンス
	 */
	public synchronized static native void sendHttpEvent(int id, HttpResponse response);
}
