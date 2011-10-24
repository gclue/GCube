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

/**
 * サウンド再生するためのIDを保持するためのクラス.
 * @author GClue, Inc.
 */
public class SoundObject {
	/**
	 * サウンドID.
	 */
	int sndId;
	
	/**
	 * プライオリティ
	 */
	int priority = 1;
	
	/**
	 * 再生フラグ.
	 */
	boolean isPlay;
	
	/**
	 * 左のボリューム.
	 */
	float leftVol;
	
	/**
	 * 右のボリューム.
	 */
	float rightVol;

	/**
	 * コンストラクタ.
	 * @param sndId サウンドID
	 */
	public SoundObject(int sndId) {
		this.sndId = sndId;
		this.isPlay = false;
	}

	/**
	 * ボリュームを指定して、再生フラグをONにします.
	 * @param leftVol 左のボリューム.
	 * @param rightVol 右のボリューム.
	 */
	public void play(float leftVol, float rightVol) {
		this.leftVol = leftVol;
		this.rightVol = rightVol;
		this.isPlay = true;
	}

	/**
	 * ボリュームを指定して、再生フラグをONにします.
	 * @param vol ボリューム.
	 */
	public void play(int vol) {
		play(vol, vol);
	}

	/**
	 * 再生フラグをONにします.
	 */
	public void play() {
		play(100, 100);
	}

	/**
	 * @return the priority
	 */
	public int getPriority() {
		return priority;
	}

	/**
	 * @param priority the priority to set
	 */
	public void setPriority(int priority) {
		this.priority = priority;
	}
}
