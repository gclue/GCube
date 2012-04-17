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

import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.widget.FrameLayout;

import com.gclue.gl.Figure;
import com.gclue.gl.GLView;
import com.gclue.gl.HttpResponse;
import com.gclue.gl.ImageData;
import com.gclue.gl.JNILib;
import com.gclue.gl.WFObjLoader;
import com.gclue.gl.texture.Sprite;
import com.gclue.gl.texture.Texture;
import com.gclue.util.HttpRequestManager;
import com.gclue.util.HttpRequestManager.HttpRequestListener;
import com.gclue.util.MusicPlayer;
import com.gclue.util.SoundManager;
import com.gclue.util.StringUtils;

/**
 * Cから呼び出されるメソッドのインターフェースとなるクラスです.
 * 
 * @author haga
 * 
 */
public class NDKInterface {
	/**
	 * デバック用フラグ.
	 */
	private static final boolean DEBUG = true;
	
	/**
	 * デバック用タグを定義します.
	 */
	private static final String TAG = "GCube";
	
	// ////////////////////////////////////////////////////////////////////////////
	// WebViewイベント
	// ////////////////////////////////////////////////////////////////////////////

	public static final int WebViewEvent_AddView = 0;
	public static final int WebViewEvent_CloseView = 1;
	public static final int WebViewEvent_RemoveView = 2;
	public static final int WebViewEvent_ReloadPage = 3;
	public static final int WebViewEvent_LoadURL = 4;
	public static final int WebViewEvent_ResizeView = 5;
	public static final int WebViewEvent_SetPosition = 6;
	
	//////////////////////////////////////////////////////////////////////////////
	// サウンドイベント
	//////////////////////////////////////////////////////////////////////////////
	
	/** BGMの作成イベント. */
	public static final int SoundMode_CreateBGM = 0;
	/** BGMの削除イベント. */
	public static final int SoundMode_DisposeBGM = 1;
	/** BGMの再生イベント. */
	public static final int SoundMode_PlayBGM = 2;
	/** BGMの停止イベント. */
	public static final int SoundMode_StopBGM = 3;
	/** BGMの連続再生イベント. */
	public static final int SoundMode_RepeatBGM = 4;
	/** BGMの一時停止イベント. */
	public static final int SoundMode_PauseBGM = 5;
	/** BGMの再開イベント. */
	public static final int SoundMode_ResumeBGM = 6;
	/** SEの準備イベント. */
	public static final int SoundMode_PrepareSE = 7;
	/** SEの削除イベント. */
	public static final int SoundMode_DisposeSE = 8;
	/** SEの再生イベント. */
	public static final int SoundMode_PlaySE = 9;
	/** SEの停止イベント. */
	public static final int SoundMode_StopSE = 10;
	/** SEのすべて停止イベント. */
	public static final int SoundMode_StopAllSE = 11;
	/** SEの一時停止イベント. */
	public static final int SoundMode_PauseSE = 12;
	/** SEの再開イベント. */
	public static final int SoundMode_ResumeSE = 13;
	
	/**
	 * SP処理の成功を定義します.
	 */
	public static final int SUCCESS = -1;
	
	/**
	 * コンテキスト.
	 */
	private Context context;
	
	/**
	 * BGM再生用のMediaPlayer.
	 */
	private MusicPlayer mediaPlayer;
	
	/**
	 * SEサウンド管理クラス.
	 */
	private SoundManager soundManager;
	
	/**
	 * OpenGLES表示用View.
	 */
	private GLView view;
	
	/**
	 * システムフォント
	 */
	private Texture texture;
	
	private FrameLayout root;
	
	/**
	 * NDKからのイベントを通知するリスナー.
	 */
	private List<INDKEventListener> listeners = new Vector<INDKEventListener>();
	
	private ArrayList<AbsoluteWebView> webViewList;
	
	/**
	 * コンストラクタ.
	 * 
	 * @param context
	 */
	public NDKInterface(Context context) {
		this.context = context;
		mediaPlayer = new MusicPlayer(context);
		webViewList = new ArrayList<AbsoluteWebView>();
	}
	
	/**
	 * このクラスが属するコンテキストを取得します.
	 * @return コンテキスト
	 */
	public Context getContext() {
		return context;
	}
	
	/**
	 * OpenGLES表示用のViewを設定します.
	 * @param view OpenGLES表示用のView
	 */
	public void setGLView(GLView view) {
		this.view = view;
	}
	
	/**
	 * OpenGLES表示用のViewを取得します.
	 * @return OpenGLES表示用のView
	 */
	public GLView getGLView() {
		return view;
	}
	
	/**
	 * ルートビューを設定します.
	 * @param root ルートビュー.
	 */
	public void setRootView(FrameLayout root) {
		this.root = root;
	}
	
	/**
	 * 一時停止します.
	 */
	public void pause() {
		// NDKを一時停止
		JNILib.onPause();
		// サウンドを一時停止
		if (mediaPlayer != null) {
			mediaPlayer.pause();
		}
		// GLSurfaceViewを一時停止
		if (view != null) {
			view.onPause();
		}
	}
	
	/**
	 * 一時停止しているプログラムを再開します.
	 */
	public void resume() {
		// NDKを再開
		JNILib.onResume();
		// サウンドを再開
		if (mediaPlayer != null) {
			mediaPlayer.resume();
		}
		// GLSurfaceViewを再開
		if (view != null) {
			view.onResume();
		}
	}
	
	/**
	 * 後始末を行います.
	 */
	public void destory() {
		// NDK側をシャットダウンします.
		JNILib.shutDown();
		// サウンドの停止
		if (mediaPlayer != null) {
			mediaPlayer.stop(null);
		}
	}
	
	/**
	 * イベントリスナーを追加します.
	 * @param listener リスナー
	 */
	public void setNDKEventListener(INDKEventListener listener) {
		listeners.add(listener);
	}
	
	/**
	 * NDKの初期化が完了したときに呼び出されます.
	 */
	public void onInitNDK() {
		//Log.e(TAG, "onInitNDK");
		for (int i = 0; i < listeners.size(); i++) {
			listeners.get(i).onInit();
		}
	}
	
	public void onWebViewEvent(int type, final int viewId, final double param1, final double param2, final double param3, final double param4, final String param5) {
		Log.i(TAG, "onWebViewEvent** type = " + type +", viewID = " + viewId);
		switch(type) {
		case WebViewEvent_AddView:
			root.post(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
					int l = webViewList.size();
					for(int i =  0 ; i < l  ; i++) {
						AbsoluteWebView wv = webViewList.get(i);
						int id = wv.getId();
						if(id == viewId) {
							Log.i(TAG, "this view had been added. view is into visible");
							wv.setVisibility(View.VISIBLE);
							return;
						}
					}
					
					AbsoluteWebView wv = new AbsoluteWebView(context);
					wv.setWebViewPosition((int)param1 + (int)param3/2, (int)param2 + (int)param4/2);
					wv.setWebViewSize((int)param3, (int)param4);
					wv.setId(viewId);
					wv.setVisibility(View.VISIBLE);
					if(param5 != null) {
						wv.loadUrl(param5);
					}
					
					root.addView(wv);
					webViewList.add(wv);
				}
				
			});
			break;
			
		case WebViewEvent_CloseView:
			
			root.post(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
					for(int i = 0 ; i < webViewList.size(); i++) {
						AbsoluteWebView wv = webViewList.get(i);
						if(wv.getId() == viewId) {
							wv.setVisibility(View.INVISIBLE);
							return;
						}
					}
					
					Log.i(TAG, "not found WebView(id: " + viewId+")");
				}
				
			});
			
			
			break;
		case WebViewEvent_RemoveView:
			
			root.post(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
					for(int i = 0 ; i < webViewList.size(); i++) {
						AbsoluteWebView wv = webViewList.get(i);
						if(wv.getId() == viewId) {
							root.removeView(wv);
							webViewList.remove(i);
							return;
						}
					}
 				}
				
			});
			break;
		}
	}
	
	/**
	 * JNIからのイベントを受け取るメソッド.
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	public void onGameEvent(int type, int param1, int param2, int param3, int param4, String param5) {
		for (int i = 0; i < listeners.size(); i++) {
			listeners.get(i).onGameEvent(type, param1, param2, param3, param4, param5);
		}
	}
	
	/**
	 * SE,BGM再生
	 * mode: 0:BGM再生
	 *       1:BGM停止
	 *       2:SE登録
	 *       3:SE再生
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @param mode モード
	 * @param fname ファイル名
	 */
	public void soundEvent(int mode, String fname) {
		if (DEBUG) {
			Log.d(TAG, "soundEvent: mode:" + mode + " fname: " + fname);
		}
		switch (mode) {
		case SoundMode_PlayBGM:
			mediaPlayer.play(fname, true, false);
			break;
		case SoundMode_RepeatBGM:
			mediaPlayer.play(fname, true, true);
			break;
		case SoundMode_StopBGM:
			mediaPlayer.pause();
			break;
		case SoundMode_PrepareSE:
			soundManager.add(fname);
			break;
		case SoundMode_PlaySE:
			soundManager.add(fname).play();
			soundManager.playAll();
			break;
		case SoundMode_StopSE:
			soundManager.stop(fname);
			break;
		case SoundMode_StopAllSE:
			soundManager.stopAll();
			break;
		case SoundMode_PauseSE:
			soundManager.pause();
			break;
		case SoundMode_ResumeSE:
			soundManager.resume();
			break;
		}
	}
	
	public boolean copyFileFromAssets(String fileName){
		final int bufSize = 8192;
		//Log.d(TAG,"copyFileFromAssets:"+fileName);
		InputStream is = null;
		FileOutputStream output = null;
		//System.gc();
		try {
			is = context.getAssets().open(fileName);
			output = context.openFileOutput("assets.tmp", Context.MODE_WORLD_READABLE); 
			
			int len;
			final byte[] b = new byte[bufSize];
			while ((len = is.read(b)) >= 0) {
				output.write(b, 0, len);
			}
		} catch (FileNotFoundException e) {
			if (DEBUG) {
				Log.w(TAG, "copyFileFromAssets", e);
			}
			return false;
		} catch (IOException e) {
			if (DEBUG) {
				Log.w(TAG, "copyFileFromAssets", e);
			}
			return false;
		} catch (OutOfMemoryError e) {
			if (DEBUG) {
				Log.w(TAG, "copyFileFromAssets", e);
			}
			return false;
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (Exception e) {
					// do nothing.
					if (DEBUG) {
						Log.w(TAG, "copyFileFromAssets", e);
					}
				}
			}
			
			if(output != null){
				try {
					output.close();
				} catch (Exception e){
					if (DEBUG) {
						Log.w(TAG, "copyFileFromAssets", e);
					}
				}
			}
		}
		//String path = "/data/data/"+context.getPackageName()+"/files/"+fileName;
		//Log.d(TAG,"copyFileFromAssets:"+path);
		//return baos.toByteArray();
		return true;
	}
	
	public String getFilePath(String name){
		//Log.d(TAG,"getFilePath:"+name);
		String path = "/data/data/"+context.getPackageName()+"/files/"+name;
		//Log.d(TAG,"filePath:"+path);
		return path;
	}
	
	
	/**
	 * assetから画像ファイルの読み込み
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @param fileName ファイル名
	 * @return ImageDataオブジェクト
	 */
	public ImageData loadImage(String fname) {
		if (DEBUG) {
			Log.i(TAG, "loadImage: " + fname);
		}
		ImageData data = new ImageData();
		if (data.loadImage(context, fname)) {
			return data;
		}
		return null;
	}
	
	/**
	 * Shaderを取得します.
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @param name シェーダ名
	 * @return Shaderのデータ
	 */
	public byte[] loadAsset(String name) {
		if (DEBUG) {
			Log.i(TAG, "loadAsset: " + name);
		}
		return readAsset(context, name);
	}
	
	/**
	 * Figureの読み込み
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @param id FigureのID
	 * @return Figureのインスタンス
	 */
	public Figure loadFigure(String name) {
		if (DEBUG) {
			Log.i(TAG, "loadFigure: " + name);
		}
		WFObjLoader loader = new WFObjLoader();
		Figure figure = loader.object3D(readAsset(context, name));
		return figure;
	}
	
	/**
	 * 文字列をテクスチャに書き込みます.
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @param text テキスト
	 * @param fontSize フォントサイズ
	 * @param r 赤色成分(0.0〜1.0)
	 * @param g 緑色成分(0.0〜1.0)
	 * @param b 青色成分(0.0〜1.0)
	 * @return 文字列が描画された範囲
	 */
	public Sprite drawText(String text, float fontSize, float r, float g, float b) {
		if (DEBUG) {
			Log.i(TAG, "drawText: " + text);
		}
		if (texture == null) {
			// 文字列用のテクスチャがない場合には作成する
			texture = new Texture();
			//texture.createBitmap(512, 512);
		}
		return texture.addDrawText(text, fontSize, r, g, b);
	}
	
	/**
	 * 文字列を描画したテクスチャを取得します.
	 * テクスチャの取得に失敗した場合にはnullを返却します.
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @return 文字列を描画したテクスチャ
	 */
	public Texture getTexture() {
		if (DEBUG) {
			Log.i(TAG, "getTexture: ");
		}
		try {
			if (texture != null) {
				texture.optimize();
			}
			return texture;
		} finally {
			texture = null;
		}
	}
	
	/**
	 * HTTP通信のリクエストを行います.
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @param url URL
	 * @param post 送信するデータ
	 */
	public HttpResponse requestHttp(String url, String[] headers, String post) {
		final HttpResponse[] response = new HttpResponse[1];
		
		// ヘッダーの作成
		Map<String, String> h = null;
		if (headers != null && headers.length > 1) {
			h = new HashMap<String, String>();
			for (int i = 0; i < headers.length / 2; i++) {
				h.put(headers[i * 2], headers[i * 2 + 1]);
			}
		}
		
		HttpRequestManager mgr = HttpRequestManager.getInstance();
		if (post == null || post.equals("")) {
			mgr.get(StringUtils.alterURL(url), h, new HttpRequestListener() {
				@Override
				public void onLoad(int status, byte[] buf) {
					response[0] = new HttpResponse();
					response[0].setStatusCode(status);
					response[0].setBody(buf);
				}
			});
		} else {
			mgr.post(StringUtils.alterURL(url), post, h, new HttpRequestListener() {
				@Override
				public void onLoad(int status, byte[] buf) {
					response[0] = new HttpResponse();
					response[0].setStatusCode(status);
					response[0].setBody(buf);
				}
			});
		}
		return response[0];
	}
	
	/**
	 * HTTP通信のリクエストを行います.
	 * <br><br>
	 * このメソッドは JNI 側から呼び出されることがあります。
	 * @param url URL
	 * @param post 送信するデータ
	 * @param id リクエストID
	 * @param async 同期処理フラグ
	 */
	public int requestHttpAsync(final String url, final String[] headers, final String post, final int id) {
		// ヘッダーの作成
		Map<String, String> h = null;
		if (headers != null && headers.length > 1) {
			h = new HashMap<String, String>();
			for (int i = 0; i < headers.length / 2; i++) {
				h.put(headers[i * 2], headers[i * 2 + 1]);
			}
		}
		
		HttpRequestManager mgr = HttpRequestManager.getInstance();
		if (post == null || post.equals("")) {
			mgr.runGet(StringUtils.alterURL(url), h, new HttpRequestListener() {
				@Override
				public void onLoad(int status, byte[] buf) {
					final HttpResponse response = new HttpResponse();
					response.setStatusCode(status);
					response.setBody(buf);
					if (view == null) {
						// まだ、起動していない
					} else {
						view.queueEvent(new Runnable() {
							@Override
							public void run() {
								Log.e(TAG, "sendHttpEvent");
								JNILib.sendHttpEvent(id, response);
							}
						});
					}
				}
			});
		} else {
			mgr.runPost(StringUtils.alterURL(url), post, h, new HttpRequestListener() {
				@Override
				public void onLoad(int status, byte[] buf) {
					final HttpResponse response = new HttpResponse();
					response.setStatusCode(status);
					response.setBody(buf);
					if (view == null) {
						// まだ、起動していない
					} else {
						view.queueEvent(new Runnable() {
							@Override
							public void run() {
								JNILib.sendHttpEvent(id, response);
							}
						});
					}
				}
			});
		}
		return -1;
	}
	
	/**
	 * JNIにイベントを配送します.
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	public void sendGameEvent(final int type, final int param1, final int param2, final int param3, final int param4, final String param5) {
		if (view == null) {
			Log.e(TAG, "NDKInterface#sendGameEvent()");
			Log.e(TAG, "view is null.");
		} else {
			view.queueEvent(new Runnable() {
				@Override
				public void run() {
					JNILib.sendGameEvent(type, param1, param2, param3, param4, param5);
				}
			});
		}
	}
	
	/**
	 * イベントをJNI側に配送します.
	 * 最後の文字列はnullが格納されます。
	 * @param type イベントタイプ
	 * @param param1 パラメータ
	 * @param param2 パラメータ
	 * @param param3 パラメータ
	 * @param param4 パラメータ
	 */
	public void sendGameEvent(int type, int param1, int param2, int param3, int param4) {
		sendGameEvent(type, param1, param2, param3, param4, null);
	}

	/**
	 * イベントをJNI側に配送します.
	 * 最後の文字列はnullが格納されます。
	 * @param type イベントタイプ
	 * @param param1 パラメータ
	 * @param param2 パラメータ
	 * @param param3 パラメータ
	 */
	public void sendGameEvent(int type, int param1, int param2, int param3) {
		sendGameEvent(type, param1, param2, param3, 0, null);
	}
	
	/**
	 * イベントをJNI側に配送します.
	 * 最後の文字列はnullが格納されます。
	 * @param type イベントタイプ
	 * @param param1 パラメータ
	 * @param param2 パラメータ
	 */
	public void sendGameEvent(int type, int param1, int param2) {
		sendGameEvent(type, param1, param2, 0, 0, null);
	}
	
	/**
	 * イベントをJNI側に配送します.
	 * 最後の文字列はnullが格納されます。
	 * @param type イベントタイプ
	 * @param param1 パラメータ
	 */
	public void sendGameEvent(int type, int param1) {
		sendGameEvent(type, param1, 0, 0, 0, null);
	}
	
	/**
	 * イベントをJNI側に配送します.
	 * 最後の文字列はnullが格納されます。
	 * @param type イベントタイプ
	 */
	public void sendGameEvent(int type) {
		sendGameEvent(type, 0, 0, 0, 0, null);
	}
	
	/**
	 * assetsフォルダにあるデータを読み込みます.
	 * <br>
	 * @param context コンテキスト
	 * @param fileName ファイル名
	 * @return データ
	 */
	public static byte[] readAsset(Context context, String fileName) {
		final int bufSize = 8192;
		
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		InputStream is = null;
		try {
			is = context.getAssets().open(fileName);
			int len;
			final byte[] b = new byte[bufSize];
			while ((len = is.read(b)) >= 0) {
				baos.write(b, 0, len);
			}
		} catch (FileNotFoundException e) {
			if (DEBUG) {
				Log.w(TAG, "readDataFromUri", e);
			}
			return null;
		} catch (IOException e) {
			if (DEBUG) {
				Log.w(TAG, "readDataFromUri", e);
			}
			return null;
		} catch (OutOfMemoryError e) {
			if (DEBUG) {
				Log.w(TAG, "readDataFromUri", e);
			}
			return null;
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (Exception e) {
					// do nothing.
					if (DEBUG) {
						Log.w(TAG, "readDataFromUri", e);
					}
				}
			}
		}
		return baos.toByteArray();
	}
}
