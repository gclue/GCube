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

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.Log;

/**
 * GLを描画するためのViewクラス.
 */
public class GLView extends GLSurfaceView {
	
	/**
	 * デバック用フラグ.
	 */
	private static final boolean DEBUG = false;
	
	/**
	 * デバック用タグを定義します.
	 */
	private static final String TAG = GLView.class.getName();
	
	/**
	 * コンストラクタ.
	 * @param context コンテキスト
	 * @param renderer レンダラ
	 */
	public GLView(Context context, Renderer renderer) {
		super(context);
		init(renderer, false, 16, 0);
	}

	/**
	 * コンストラクタ.
	 * @param context コンテキスト
	 * @param renderer レンダラ
	 * @param translucent 透明フラグ
	 * @param depth デップス
	 * @param stencil ステンシル
	 */
	GLView(Context context, Renderer renderer, boolean translucent, int depth, int stencil) {
		super(context);
		init(renderer, translucent, depth, stencil);
	}

	/**
	 * GameViewの初期化処理を行います.
	 * @param renderer レンダラ
	 * @param translucent 透過フラグ
	 * @param depth 色深度
	 * @param stencil ステンシル
	 */
	private void init(Renderer renderer, boolean translucent, int depth, int stencil) {
		// By default, GLSurfaceView() creates a RGB_565 opaque surface. If we
		// want a translucent one, we should change the surface's format here,
		// using PixelFormat.TRANSLUCENT for GL Surfaces is interpreted as any
		// 32-bit surface with alpha by SurfaceFlinger.
		if (translucent) {
			getHolder().setFormat(PixelFormat.TRANSLUCENT);
		}

		// Setup the context factory for 2.0 rendering. See ContextFactory class
		// definition below
		setEGLContextFactory(new ContextFactory());

		// We need to choose an EGLConfig that matches the format of our surface
		// exactly. This is going to be done in our custom config chooser. See
		// ConfigChooser class definition below.
		setEGLConfigChooser(translucent ? new ConfigChooser(8, 8, 8, 8, depth, stencil) : new ConfigChooser(5, 6, 5, 0, depth, stencil));

		// Set the renderer responsible for frame rendering
		setRenderer(renderer);
		
		requestFocus();
	}
	
	/**
	 * OpenGLESのコンテキストを作成するためのファクトリークラス.
	 * @author GClue, Inc.
	 */
	private static class ContextFactory implements GLSurfaceView.EGLContextFactory {
		/**
		 * EGLのバージョンの定義.
		 */
		private static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

		@Override
		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
			if (DEBUG) {
				Log.w(TAG, "creating OpenGL ES 2.0 context");
			}

			final int[] attrib_list = {
				EGL_CONTEXT_CLIENT_VERSION,
				2, 
				EGL10.EGL_NONE
			};
			
			checkEglError("Before eglCreateContext", egl);
			EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
			checkEglError("After eglCreateContext", egl);
			return context;
		}

		@Override
		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
			if (DEBUG) {
				Log.w(TAG, "destroy OpenGL ES 2.0 context");
			}
			
			egl.eglDestroyContext(display, context);
		}
		
		/**
		 * エラーを出力します.
		 * @param prompt 文字列
		 * @param egl eglのコンテキスト
		 */
		private void checkEglError(String prompt, EGL10 egl) {
			int error;
			while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
				Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
			}
		}
	}

	/**
	 * OpenGLESのコンフィグ選択用クラス.
	 * @author GClue, Inc.
	 */
	private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser {
		// Subclasses can adjust these values:
		protected int mRedSize;
		protected int mGreenSize;
		protected int mBlueSize;
		protected int mAlphaSize;
		protected int mDepthSize;
		protected int mStencilSize;
		private int[] mValue = new int[1];

		// This EGL config specification is used to specify 2.0 rendering. We
		// use a minimum size of 4 bits for red/green/blue, but will perform
		// actual matching in chooseConfig() below.
		private static final int EGL_OPENGL_ES2_BIT = 4;
		private static final int[] s_configAttribs2 = {
			EGL10.EGL_RED_SIZE, 
			4, 
			EGL10.EGL_GREEN_SIZE, 
			4, 
			EGL10.EGL_BLUE_SIZE, 
			4, 
			EGL10.EGL_RENDERABLE_TYPE,
			EGL_OPENGL_ES2_BIT, 
			EGL10.EGL_DEPTH_SIZE,
			16, 
			EGL10.EGL_NONE
		};

		public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
			mRedSize = r;
			mGreenSize = g;
			mBlueSize = b;
			mAlphaSize = a;
			mDepthSize = depth;
			mStencilSize = stencil;
		}

		@Override
		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
			// Get the number of minimally matching EGL configurations
			int[] num_config = new int[1];
			egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

			int numConfigs = num_config[0];
			if (numConfigs <= 0) {
				throw new IllegalArgumentException("No configs match configSpec");
			}

			// Allocate then read the array of minimally matching EGL configs
			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

			if (DEBUG) {
				printConfigs(egl, display, configs);
			}

			// Now return the "best" one
			return chooseConfig(egl, display, configs);
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
			for (EGLConfig config : configs) {
				int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);

				// We need at least mDepthSize and mStencilSize bits
				if (d < mDepthSize || s < mStencilSize) {
					continue;
				}
				
				// We want an *exact* match for red/green/blue/alpha
				int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
				int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
				int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
				int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);

				if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize) {
					return config;
				}
			}
			return null;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue) {
			if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
				return mValue[0];
			}
			return defaultValue;
		}
		
		/**
		 * コンフィグ出力用関数.
		 * @param egl eglのインスタンス
		 * @param display eglのディスプレイ
		 * @param configs コンフィグ
		 */
		private void printConfigs(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
			int numConfigs = configs.length;
			Log.w(TAG, String.format("%d configurations", numConfigs));
			for (int i = 0; i < numConfigs; i++) {
				Log.w(TAG, String.format("Configuration %d:\n", i));
				printConfig(egl, display, configs[i]);
			}
		}

		/**
		 * コンフィグ出力用関数.
		 * @param egl eglのインスタンス
		 * @param display eglのディスプレイ
		 * @param config コンフィグ
		 */
		private void printConfig(EGL10 egl, EGLDisplay display, EGLConfig config) {
			final int[] attributes = {
				EGL10.EGL_BUFFER_SIZE, 
				EGL10.EGL_ALPHA_SIZE, 
				EGL10.EGL_BLUE_SIZE, 
				EGL10.EGL_GREEN_SIZE, 
				EGL10.EGL_RED_SIZE,
				EGL10.EGL_DEPTH_SIZE, 
				EGL10.EGL_STENCIL_SIZE, 
				EGL10.EGL_CONFIG_CAVEAT, 
				EGL10.EGL_CONFIG_ID, 
				EGL10.EGL_LEVEL, 
				EGL10.EGL_MAX_PBUFFER_HEIGHT,
				EGL10.EGL_MAX_PBUFFER_PIXELS, 
				EGL10.EGL_MAX_PBUFFER_WIDTH, 
				EGL10.EGL_NATIVE_RENDERABLE, 
				EGL10.EGL_NATIVE_VISUAL_ID,
				EGL10.EGL_NATIVE_VISUAL_TYPE,
				0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
				EGL10.EGL_SAMPLES, 
				EGL10.EGL_SAMPLE_BUFFERS, 
				EGL10.EGL_SURFACE_TYPE, 
				EGL10.EGL_TRANSPARENT_TYPE, 
				EGL10.EGL_TRANSPARENT_RED_VALUE,
				EGL10.EGL_TRANSPARENT_GREEN_VALUE, 
				EGL10.EGL_TRANSPARENT_BLUE_VALUE, 
				0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
				0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
				0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
				0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
				EGL10.EGL_LUMINANCE_SIZE, 
				EGL10.EGL_ALPHA_MASK_SIZE, 
				EGL10.EGL_COLOR_BUFFER_TYPE, 
				EGL10.EGL_RENDERABLE_TYPE, 
				0x3042 // EGL10.EGL_CONFORMANT
			};
			final String[] names = {
				"EGL_BUFFER_SIZE", 
				"EGL_ALPHA_SIZE", 
				"EGL_BLUE_SIZE", 
				"EGL_GREEN_SIZE", 
				"EGL_RED_SIZE", 
				"EGL_DEPTH_SIZE", 
				"EGL_STENCIL_SIZE",
				"EGL_CONFIG_CAVEAT", 
				"EGL_CONFIG_ID", 
				"EGL_LEVEL", 
				"EGL_MAX_PBUFFER_HEIGHT", 
				"EGL_MAX_PBUFFER_PIXELS", 
				"EGL_MAX_PBUFFER_WIDTH",
				"EGL_NATIVE_RENDERABLE", 
				"EGL_NATIVE_VISUAL_ID", 
				"EGL_NATIVE_VISUAL_TYPE", 
				"EGL_PRESERVED_RESOURCES", 
				"EGL_SAMPLES", 
				"EGL_SAMPLE_BUFFERS",
				"EGL_SURFACE_TYPE", 
				"EGL_TRANSPARENT_TYPE", 
				"EGL_TRANSPARENT_RED_VALUE", 
				"EGL_TRANSPARENT_GREEN_VALUE", 
				"EGL_TRANSPARENT_BLUE_VALUE",
				"EGL_BIND_TO_TEXTURE_RGB", 
				"EGL_BIND_TO_TEXTURE_RGBA", 
				"EGL_MIN_SWAP_INTERVAL", 
				"EGL_MAX_SWAP_INTERVAL", 
				"EGL_LUMINANCE_SIZE",
				"EGL_ALPHA_MASK_SIZE", 
				"EGL_COLOR_BUFFER_TYPE", 
				"EGL_RENDERABLE_TYPE", 
				"EGL_CONFORMANT"
			};
			int[] value = new int[1];
			for (int i = 0; i < attributes.length; i++) {
				int attribute = attributes[i];
				String name = names[i];
				if (egl.eglGetConfigAttrib(display, config, attribute, value)) {
					Log.w(TAG, String.format("  %s: %d\n", name, value[0]));
				} else {
					// Log.w(TAG, String.format("  %s: failed\n", name));
					while (egl.eglGetError() != EGL10.EGL_SUCCESS)
						;
				}
			}
		}
	}
}
