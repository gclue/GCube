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

package com.gclue.util;


import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;

import android.content.Context;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;

/**
 * SEサウンド管理用クラス.
 * <br><br>
 * 
 * SoundPoolを使用して、効果音を再生します。
 * 再生できる効果音の秒数は約6秒程度です。
 * 
 * GalexySが6秒で、他の端末は10秒程度再生することができるみたい。
 * 端末毎に再生できる時間が違うので、サポートする端末によって効果音を変える必要がある。
 * 
 * @author GClue, Inc.
 */
public class SoundManager {
	/**
	 * サウンドオブジェクトを格納するためのマップ.
	 */
	private HashMap<String, SoundObject> soundMap = new HashMap<String, SoundObject>();
	
	/**
	 * サウンド再生を行うクラス.
	 */
	private SoundPool sp;
	
	/**
	 * このクラスが属するコンテキスト.
	 */
	private Context context;

	/**
	 * コンストラクタ.
	 * @param context コンテキスト
	 */
	public SoundManager(Context context) {
		this.context = context;
	}

	/**
	 * 指定された名前のサウンドオブジェクトを取得します.
	 * <br><br>
	 * 対応するサウンドが存在しない場合には、<code>null</code>を返却します。
	 * @param name 名前
	 * @return サウンドオブジェクト
	 */
	public SoundObject get(String name) {
		return soundMap.get(name);
	}

	/**
	 * サウンド再生登録されているサウンドオブジェクトを再生します.
	 */
	public void playAll() {
		if (sp == null) {
			return;
		}
		
		for (SoundObject obj : soundMap.values()) {
			if (obj.isPlay) {
				sp.play(obj.sndId, obj.leftVol, obj.rightVol, obj.priority, 0, 1f);
				obj.isPlay = false;
			}
		}
	}
	
	/**
	 * すべてのサウンドを停止します.
	 */
	public void stopAll() {
		if (sp != null) {
			for (Iterator<?> it = soundMap.keySet().iterator(); it.hasNext();) {
				Object key = it.next();
				SoundObject so = (SoundObject) soundMap.get(key);
				if (so != null) {
					sp.stop(so.sndId);
				}
			}
		}
	}
	
	/**
	 * 指定された名前のサウンドを停止します.
	 * @param name サウンド名
	 */
	public void stop(String name) {
		SoundObject obj = get(name);
		if (obj != null) {
			try {
				sp.stop(obj.sndId);
			} catch (Exception e) {
				// do nothing.
			}
		}
	}

	/**
	 * サウンドオブジェクトを追加します.
	 * <br><br>
	 * 既に同じサウンドオブジェクトが登録されている場合には、処理をスキップします。
	 * @param id リソースID
	 * @param name 名前
	 * @return サウンドオブジェクト
	 */
	public SoundObject add(int id, String name) {
		SoundObject sm = soundMap.get(name);
		if (sm == null) {
			if (sp == null) {
				sp = new SoundPool(10, AudioManager.STREAM_MUSIC, 100);
			}
			sm = new SoundObject(sp.load(context, id, 1));
			soundMap.put(name, sm);
		}
		return sm;
	}
	
	public SoundObject add(String name) {
		SoundObject sm = soundMap.get(name);
		if (sm == null) {
			if (sp == null) {
				sp = new SoundPool(10, AudioManager.STREAM_MUSIC, 100);
			}
			AssetManager am = context.getAssets();
			try {
				int id = sp.load(am.openFd(name), 1);
				sm = new SoundObject(id);
				soundMap.put(name, sm);
			} catch (IOException e) {}
		}
		return sm;
	}
	
	public void pause() {
		if (sp != null) {
//			sp.autoPause();
			for (Iterator<?> it = soundMap.keySet().iterator(); it.hasNext();) {
				Object key = it.next();
				SoundObject so = (SoundObject) soundMap.get(key);
				if (so != null) {
					sp.pause(so.sndId);
				}
			}
		}
	}
	
	public void resume() {
		if (sp != null) {
			// この関数は API Level 8からサポートされている
			// Android2.1 は 7 なのでサポートされていない
//			sp.autoResume();
			for (Iterator<?> it = soundMap.keySet().iterator(); it.hasNext();) {
				Object key = it.next();
				SoundObject so = (SoundObject) soundMap.get(key);
				if (so != null) {
					sp.resume(so.sndId);
				}
			}
		}
	}

	/**
	 * 登録されているサウンドオブジェクトをすべてリリースします.
	 */
	public void release() {
		for (SoundObject obj : soundMap.values()) {
			try {
				sp.stop(obj.sndId);
				sp.unload(obj.sndId);
			} catch (Exception e) {
				// do nothing.
			}
		}
		soundMap.clear();
		sp.release();
		sp = null;
		context = null;
	}
}
