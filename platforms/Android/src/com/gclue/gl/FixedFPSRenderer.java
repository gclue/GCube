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

import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.os.SystemClock;

/**
 * FPS固定で描画するレンダラ抽象クラス.
 * デフォルトでは30fpsで描画されます。
 * 
 * ただし、端末によっては30fpsを出せない場合には、その端末のmaxのfpsで動作します。
 * 
 * @author GClue, Inc.
 */
public abstract class FixedFPSRenderer implements GLSurfaceView.Renderer {
	/**
	 * FPS固定化用クラス.
	 */
	private FPSManager fps = new FPSManager(30);
	
	/**
	 * 経過時間計測用
	 */
	private long oldTime = SystemClock.uptimeMillis();

	/**
	 * FPS固定で描画.
	 * @param gl
	 * @param dt 経過時間
	 */
	public abstract void onDraw(GL10 gl, float dt);
	
	/* (non-Javadoc)
	 * @see android.opengl.GLSurfaceView.Renderer#onDrawFrame(javax.microedition.khronos.opengles.GL10)
	 */
	@Override
	public void onDrawFrame(GL10 gl) {
		// 経過時間
		long nowTime = SystemClock.uptimeMillis();
		// 描画処理
		onDraw(gl, (nowTime - oldTime) / 1000.0f);
		// スリープ
		fps.sleep();
		oldTime = nowTime;
	}
	
	/**
	 * FPSを設定します.
	 * @param fps FPS
	 */
	public void setFPS(int fps) {
		this.fps.setFPS(fps);
	}
	
	/**
	 * FPS固定化用クラス
	 */
	private static class FPSManager {
		/** フレームあたりミリ秒 */
		private long frameDuration;
		/** 1フレーム前の時間 */
		private long oldTime = 0;
		
		/**
		 * FPS指定でオブジェクトを生成します.
		 * @param FPS FPSの設定
		 */
		public FPSManager(int FPS) {
			frameDuration = 1000 / FPS;
		}
		
		/**
		 * FPSを設定します.
		 * @param FPS FPSの設定
		 */
		public void setFPS(int FPS) {
			frameDuration = 1000 / FPS;
		}
		
		/**
		 * 1フレーム分スリープします.
		 */
		public void sleep() {
			long nowTime = SystemClock.uptimeMillis();
			long sleepTime = oldTime + frameDuration;
			oldTime = nowTime;
			while (true) {
				nowTime = SystemClock.uptimeMillis();
				if (nowTime < sleepTime) {
					try {
						Thread.sleep(1);
					} catch (InterruptedException e) {
						// do nothing.
					}
				} else {
					oldTime = nowTime;
					break;
				}
			}
		}
	}
}
