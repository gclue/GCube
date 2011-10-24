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

import com.gclue.gl.scene.GCScene;

/**
 * アプリケーションを管理するためのクラス.
 * @author GClue, Inc.
 */
public class ApplicationController {
	/**
	 * ApplicationControllerのインスタンス.
	 */
	public static ApplicationController instance;
	
	/**
	 * このクラスはシングルトンにするので、privateとする。
	 */
	private ApplicationController() {
	}
	
	/**
	 * ApplicationControllerのインスタンスを取得します.
	 * @return ApplicationControllerのインスタンス
	 */
	public static ApplicationController getInstance() {
		if (instance == null) {
			instance = new ApplicationController();
		}
		return instance;
	}
	
	/**
	 * シーンを追加します.
	 * @param id シーンID
	 * @param scene シーン
	 */
	public void addScene(int id, GCScene scene) {
		nativeAddScene(id, scene.getPear());
	}
	
	/**
	 * シーンを変更します.
	 * 指定されたシーンが存在しない場合には、特に何も処理を行いません。
	 * @param id 遷移先のシーンID
	 */
	public void sceneChange(int id) {
		nativeSceneChange(id);
	}
	
	private native void nativeAddScene(int id, int pear);
	private native void nativeSceneChange(int id);
}
