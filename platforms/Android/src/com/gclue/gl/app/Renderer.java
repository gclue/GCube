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

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;

import com.gclue.gl.FixedFPSRenderer;
import com.gclue.gl.JNILib;

/**
 * レンダラー.
 * @author GClue, Inc.
 */
public class Renderer extends FixedFPSRenderer {
	/**
	 * NDKとのやり取りを行う為のインターフェイス.
	 */
	private NDKInterface ndk;
	
	private int curWidth;
	private int curHeight;
	
	/**
	 * コンストラクタ.
	 * @param ndk NDKとのやり取り行うインターフェイス
	 */
	public Renderer(NDKInterface ndk) {
		super();
		this.ndk = ndk;
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		//Log.e("Render","onSurfaceChanged"+width+" "+height);
		
		if(curWidth == width && curHeight == height) return;
		
		curWidth = width;
		curHeight = height;
		
		System.gc();
		boolean first = JNILib.init(width, height);
		if (!first) {
			// OpenGLESの初期化でエラーが発生したので、ダイアログを表示して終了する
			final Context context = ndk.getContext();
			AlertDialog.Builder builder = new AlertDialog.Builder(context);
			builder.setTitle("ERROR");
			builder.setMessage("Failed to initialize OpenGLES.");
			AlertDialog dlg = builder.show();
			dlg.setOnDismissListener(new OnDismissListener() {
				@Override
				public void onDismiss(DialogInterface dialog) {
					((Activity) ndk.getContext()).finish();
				}
			});
			return;
		}
		// NDKの初期化が完了したことを通知
		ndk.onInitNDK();
	}
	
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// JNI側にNDKInterfaceを設定
		JNILib.setInterface(ndk);
		curWidth = -1;
		curHeight = -1;
	}
	
	@Override
	public void onDraw(GL10 gl, float dt) {
		JNILib.step(dt);
	}
}
