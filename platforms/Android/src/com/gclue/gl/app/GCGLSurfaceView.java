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

package com.gclue.gl.app;

import java.util.List;
import java.util.Vector;

import android.content.Context;
import android.view.MotionEvent;

import com.gclue.gl.GLView;
import com.gclue.gl.JNILib;

/**
 * GLを描画するためのView.
 * @author GClue, Inc.
 */
public class GCGLSurfaceView extends GLView {
	/** タッチイベントを保持するリスト. */
	private List<TouchEvent> evts = new Vector<TouchEvent>();
	
	/**
	 * コンストラクタ.
	 * @param context コンテキスト
	 * @param renderer レンダラ
	 */
	public GCGLSurfaceView(Context context, Renderer renderer) {
		super(context, renderer);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		final int action = event.getAction();
		final float x = event.getX();
		final float y = event.getY();
		final long time = event.getEventTime();
		switch (action) {
		case MotionEvent.ACTION_DOWN:
		case MotionEvent.ACTION_UP:
		case MotionEvent.ACTION_MOVE:
			queueEvent(getTouchEvent(action, x, y, time));
			break;
		}
		return true;
	}
	
	/**
	 * 使用していないタッチイベント用のオブジェクトを取得します.
	 * もし、使用していないタッチイベントがない場合には新たに作成します。
	 * @param action アクション
	 * @param x x座標
	 * @param y y座標
	 * @param time タッチイベントの発生した時間
	 * @return タッチイベント
	 */
	private TouchEvent getTouchEvent(int action, float x, float y, long time) {
		for (int i = 0; i < evts.size(); i++) {
			if (!evts.get(i).isUsed) {
				TouchEvent evt = evts.get(i);
				evt.isUsed = true;
				evt.action = action;
				evt.x = x;
				evt.y = y;
				evt.time = time;
				return evt;
			}
		}
		
		TouchEvent evt = new TouchEvent();
		evt.isUsed = true;
		evt.action = action;
		evt.x = x;
		evt.y = y;
		evt.time = time;
		evts.add(evt);
		return evt;
	}
	
	/**
	 * タッチイベント.
	 * @author GClue, Inc.
	 */
	private static class TouchEvent implements Runnable {
		/** タッチイベントのアクション. */
		private int action;
		/** タッチされたx座標. */
		private float x;
		/** タッチされたy座標. */
		private float y;
		/** タッチされた時間. */
		private long time;
		/** 使用フラグ. */
		private boolean isUsed;
		@Override
		public void run() {
			JNILib.touchEvent(action, x, y, time);
			isUsed = false;
		}
	}
}