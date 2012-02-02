package com.gclue.util;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;

import android.util.Log;

public class HttpRequestManager {
	/**
	 * デバック用フラグ.
	 */
	private static final boolean DEBUG = false;
	
	/**
	 * デバック用タグを定義します.
	 */
	private static final String TAG = HttpRequestManager.class.getName();
	
	/**
	 * 並列に動作するスレッドの個数を定義します.
	 */
	private static final int THREAD_COUNT = 1;
	
	/**
	 * バッファサイズを定義します.
	 */
	private static final int BUFSIZE = 8192;
	
	/**
	 * エラーとなるレスポンスコードを定義します.
	 */
	private static final int ERROR_RESPONSE_CODE = 400;
	
	/**
	 * デフォルトのタイムアウトを定義します.
	 */
	private static final int DEFAULT_TIMEOUT = 1000 * 30;
	
	/**
	 * タイムアウトの最小値を定義します.
	 */
//	private static final int MINIMUM_TIMEOUT = 1000;
	
	/**
	 * タイムアウト.
	 */
	private static int timeout = DEFAULT_TIMEOUT;
	
	/**
	 * 並列Threadサービス.
	 */
	private ExecutorService executor;
	
	private static HttpRequestManager instance;
	
	private HttpRequestManager() {
		executor = Executors.newFixedThreadPool(THREAD_COUNT);
	}
	
	public static HttpRequestManager getInstance() {
		if (instance == null) {
			instance = new HttpRequestManager();
		}
		return instance;
	}
	
	public void post(String url, String post, Map<String, String> headers, HttpRequestListener listener) {
		HttpClient objHttp = new DefaultHttpClient();
		HttpParams params = objHttp.getParams();
		HttpConnectionParams.setConnectionTimeout(params, timeout);
		HttpConnectionParams.setSoTimeout(params, timeout);
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		InputStream objStream = null;
		int status;
		try {
			HttpPost objPost = new HttpPost(url);
			if (headers != null) {
				Set<String> keys = headers.keySet();
				for (String key : keys) {
					objPost.addHeader(key, headers.get(key));
				}
			}
			StringEntity paramEntity = new StringEntity(post);
			paramEntity.setChunked(false);
			paramEntity.setContentType("application/x-www-form-urlencoded");
			objPost.setEntity(paramEntity);
			
			HttpResponse objResponse = objHttp.execute(objPost);
			status = objResponse.getStatusLine().getStatusCode();
			if (status < ERROR_RESPONSE_CODE) {
				objStream = objResponse.getEntity().getContent();
				
				int len;
				byte[] buf = new byte[BUFSIZE];
				while ((len = objStream.read(buf)) > 0) {
					baos.write(buf, 0, len);
				}
			}
		} catch (IOException e) {
			status = 0;
		} catch (Exception e) {
			status = 0;
		} finally {
			if (objStream != null) {
				try {
					objStream.close();
				} catch (IOException e) {
					// do nothing.
					if (DEBUG) {
						Log.w(TAG, "objStream.close()", e);
					}
				}
			}
		}
		listener.onLoad(status, baos.toByteArray());
	}
	
	public void runPost(final String url, final String post, final Map<String, String> headers, final HttpRequestListener listener) {
		executor.execute(new Runnable() {
			@Override
			public void run() {
				post(url, post, headers, listener);
			}
		});
	}
	
	
	public void get(String url, Map<String, String> headers, HttpRequestListener listener) {
		HttpClient objHttp = new DefaultHttpClient();
		HttpParams params = objHttp.getParams();
		HttpConnectionParams.setConnectionTimeout(params, timeout);
		HttpConnectionParams.setSoTimeout(params, timeout);
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		InputStream objStream = null;
		int status;
		Log.e("GLLibrary", "JAVA GET: " + url);
		try {
			HttpGet objGet = new HttpGet(url);
			if (headers != null) {
				Set<String> keys = headers.keySet();
				for (String key : keys) {
					objGet.addHeader(key, headers.get(key));
				}
			}
			HttpResponse objResponse = objHttp.execute(objGet);
			status = objResponse.getStatusLine().getStatusCode();
			if (status < ERROR_RESPONSE_CODE) {
				objStream = objResponse.getEntity().getContent();
				
				int len;
				byte[] buf = new byte[BUFSIZE];
				while ((len = objStream.read(buf)) > 0) {
					baos.write(buf, 0, len);
				}
			}
		} catch (IOException e) {
			status = 0;
			Log.e("GLLibrary", "JAVA: " + url, e);
		} finally {
			if (objStream != null) {
				try {
					objStream.close();
				} catch (IOException e) {
					// do nothing.
					if (DEBUG) {
						Log.w(TAG, "objStream.close()", e);
					}
				}
			}
		}
		listener.onLoad(status, baos.toByteArray());
	}
	
	public void runGet(final String url, final Map<String, String> headers, final HttpRequestListener listener) {
		executor.execute(new Runnable() {
			@Override
			public void run() {
				get(url, headers, listener);
			}
		});
	}
	
	public static interface HttpRequestListener {
		void onLoad(int status, byte[] buf);
	}
}
