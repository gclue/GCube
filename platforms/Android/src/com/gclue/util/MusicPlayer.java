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

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.util.Log;

/**
 * サウンドの操作を管理します.
 * @author GClue, Inc.
 */
public class MusicPlayer {
	/**
	 * デバック用フラグ.
	 */
	private static final boolean DEBUG = true;
	
	/**
	 * デバック用タグを定義します.
	 */
	private static final String TAG = "GLLibrary";
	
	/**
	 * MediaPlayerのインスタンス.
	 */
	private final MediaPlayer mediaPlayer = new MediaPlayer();
	
	/**
	 * コンテキスト.
	 */
	private Context context;
	
	/**
	 * 再生中のサウンドファイル.
	 */
	private String audioFileName;
	
	/**
	 * ボリューム.
	 */
	private float leftVolume;
	
	/**
	 * ボリューム.
	 */
	private float rightVolume;
	
	/**
	 * コンストラクタ.
	 * @param context コンテキスト
	 */
	public MusicPlayer(Context context) {
		this.context = context;
		leftVolume = 1.0f;
		rightVolume = 1.0f;
		mediaPlayer.setVolume(leftVolume, rightVolume);
	}
	
	/**
	 * サウンド再生を行います.
	 * @param fname ファイル名
	 * @param loop ループ再生フラグ
	 * @param repeat 連続再生フラグ
	 */
	public void play(String fname, boolean loop, boolean repeat) {
		if (fname == null) {
			if (audioFileName == null) {
				// 前回再生させていたサウンドがない場合には何もしない
				return;
			}
			// fnameがnullの場合には前回再生していたサウンドのファイル名を使用する
			fname = audioFileName;
		} else {
			if (repeat) {
				// すでに同じ曲が再生されている場合には再生を続ける
				if (audioFileName != null && 
					audioFileName.equals(fname) &&
					mediaPlayer.isPlaying()) {
					return;
				}
			}
			// 再生中のサウンドのファイル名を一時的に変数に格納しておく
			audioFileName = fname;
		}
		
		if (DEBUG) {
			Log.i(TAG, "MusicPlayer#play(): " + fname);
		}
		
		try {
			AssetFileDescriptor assetFileDescritor = context.getAssets().openFd(fname);
			mediaPlayer.reset();
			mediaPlayer.setDataSource(assetFileDescritor.getFileDescriptor(), assetFileDescritor.getStartOffset(), assetFileDescritor.getLength());
			mediaPlayer.setLooping(loop);
			mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
			assetFileDescritor.close(); 
			mediaPlayer.prepare();
			mediaPlayer.start();
		} catch (Throwable e) {
			Log.e(TAG, "", e);
			// 再生に失敗した場合は名前をnullにしておく
			audioFileName = null;
		}
	}
	
	/**
	 * サウンド再生を停止します.
	 * @param fname 停止するサウンド名
	 */
	public void stop(String fname) {
		if (mediaPlayer != null) {
			// NDKから呼ばれたスレッドで停止すると落ちてしまうことが
			// 多々あるので、別スレッドを用意して停止するようにする
			new Thread(new Runnable() {
				@Override
				public void run() {
					try {
						mediaPlayer.pause();
					} catch (Throwable e) {
						Log.e(TAG, "mediaPlayer.pause", e);
					}
				}
			}).start();
			if (fname != null) {
				if (fname.equals(audioFileName)) {
					audioFileName = null;
				}
			}
		}
	}
	
	/**
	 * サウンドを一時停止します.
	 */
	public void pause() {
		if (mediaPlayer != null) {
			new Thread(new Runnable() {
				@Override
				public void run() {
					try {
						mediaPlayer.pause();
					} catch (Throwable e) {
						Log.e(TAG, "mediaPlayer.pause", e);
					}
				}
			}).start();
		}
	}
	
	/**
	 * 一時停止しているサウンドを再開します.
	 */
	public void resume() {
		if (mediaPlayer != null) {
			new Thread(new Runnable() {
				@Override
				public void run() {
					try {
						mediaPlayer.start();
					} catch (Throwable e) {
						Log.e(TAG, "mediaPlayer.pause", e);
					}
				}
			}).start();
		}
	}
	
	/**
	 * サウンドのボリュームを設定します.
	 * @param left 左側のボリューム
	 * @param right 右側のボリューム
	 */
	public void setVolume(float left, float right) {
		this.leftVolume = left;
		this.rightVolume = right;
		mediaPlayer.setVolume(left, right);
	}
	
	/**
	 * 左側のボリュームを取得します.
	 * @return ボリューム
	 */
	public float getLeftVolume() {
		return leftVolume;
	}
	
	/**
	 * 右側のボリュームを取得します.
	 * @return ボリューム
	 */
	public float getRightVolume() {
		return rightVolume;
	}
	
	/**
	 * 再生中かチェックします.
	 * @return true: 再生中
	 */
	public boolean isPlaying() {
		return mediaPlayer.isPlaying();
	}
	
	/**
	 * ループ再生中かチェックします.
	 * @return true: ループ再生中
	 */
	public boolean isLooping() {
		return mediaPlayer.isLooping();
	}
}
