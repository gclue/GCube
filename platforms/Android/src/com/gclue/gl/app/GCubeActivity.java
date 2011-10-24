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

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;

import com.gclue.gl.JNILib;

/**
 * 起動用アクティビティ.
 * @author GClue, Inc.
 */
public class GCubeActivity extends Activity {
	static {
		System.loadLibrary("gcube");
	}
	
	/**
	 * デバック用フラグ.
	 * リリースする場合にはfalseに設定すること。
	 */
	public static final boolean DEBUG = true;
	/** デバック用タグ. */
	public static final String TAG = "GLLibrary";
	
	/** GLの描画を行うView. */
	private GCGLSurfaceView view;
	/** NDKとのインターフェイス */
	private NDKInterface ndk;
	/** レンダリング. */
	private Renderer renderer;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		//自動スリープをオフにします
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		// Notificationバーとタイトルバーを消します
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		// 初期化
		ndk = new NDKInterface(this);
		// レンダラーの初期化
		renderer = new Renderer(ndk);
		// Viewの初期化
		view = new GCGLSurfaceView(this, renderer);
		// ndkにGLViewを設定
		ndk.setGLView(view);
		
		// Viewを設定
		setContentView(view);
	}
	
	/**
	 * NDKとのインターフェイスクラスを取得します.
	 * @return NDKInterfaceのインスタンス
	 */
	public NDKInterface getNDKInterface() {
		return ndk;
	}
	
	/**
	 * GLを描画するためのViewを取得します.
	 * @return GCGLSurfaceViewのインスタンス
	 */
	public GCGLSurfaceView getGLSurfaceView() {
		return view;
	}
	
	/**
	 * FPSを設定します.
	 * ただし、端末の性能を超えて
	 * @param fps 設定するfps
	 */
	public void setFPS(int fps) {
		renderer.setFPS(fps);
	}
	
	/**
	 * アクティビティが破棄されたときに呼ばれます.
	 */
	@Override
	public void onDestroy() {
		if (DEBUG) {
			Log.d(TAG, "********************** onDestory");
		}
		if (ndk != null) {
			ndk.destory();
		}
		view = null;
		ndk = null;
		super.onDestroy();
	}

	@Override
	public void onLowMemory() {
		super.onLowMemory();
		if (DEBUG) {
			Log.d(TAG, "********************** onLowMemory.");
		}
	}

	
	@Override
	protected void onPause() {
		if (DEBUG) {
			Log.d(TAG,"********************** onPause");
		}
		if (ndk != null) {
			ndk.pause();
		}
		super.onPause();
	}

	@Override
	protected void onRestart() {
		if (DEBUG) {
			Log.i(TAG, "********************** onRestart");
		}
		super.onRestart();
	}

	@Override
	protected void onResume() {
		if (DEBUG) {
			Log.i(TAG, "********************** onResume");
		}
		if (ndk != null) {
			ndk.resume();
		}
		super.onResume();
	}

	@Override
	protected void onStart() {
		if (DEBUG) {
			Log.i(TAG, "********************** onStart");
		}
		super.onStart();
	}

	@Override
	protected void onStop() {
		if (DEBUG) {
			Log.i(TAG, "********************** onStop");
		}
		// ここでゲームのステートを一旦保存。
		// onDestroyが呼ばれるようであれば、勝手に消去されます。
		// staticで持つ場合は、static変数の値をonDestroyで初期化します.
		super.onStop();
	}
	
	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		if (event.getAction() == KeyEvent.ACTION_DOWN) {
			if (event.getKeyCode() == KeyEvent.KEYCODE_BACK){
				if (JNILib.onPressBackKey()) {
					return true;
				}
			}
		}
		return super.dispatchKeyEvent(event);
	}
}