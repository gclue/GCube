/*
 * The MIT License (MIT)
 * Copyright (c) 2012 GClue, inc.
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

#include "main.h"
#include "ApplicationController.h"
#include "Scene3D.h"

// 初期化処理.
void Main::initApplicationController() {
	LOGD("*Main::initApplicationController()");
	    
    // シーンの生成
	Scene3D *hello = new Scene3D();
    
    // シーンの登録
    controller->addScene(1, hello);
    
    // シーンへ遷移
	controller->sceneChange(1);
    
}

// 一時停止処理.
void Main::onPause() {
	LOGD("*Main::onPause()");
}

// 一時停止から復帰処理.
void Main::onResume() {
	LOGD("*Main::onResume()");
}

// データ保存.
void Main::saveData() {
	LOGD("*Main::saveData()");
}

// ゲームイベント受信.
bool Main::onGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5) {
	LOGD("*Main::onGameEvent(%d, %d, %ld, %f, %d, %s)", type, param1, param2, param3, param4, param5);
	return false;
}

// ツイッターイベント受信.
bool Main::onTwitterEvent(int type, int param1) {
	LOGD("*Main::onTwitterEvent(%d, %d)",type, param1);
	return false;
}

// iOSのGameCenterイベント受信.
bool Main::onGameCenterEvent(int type, int param1, int param2) {
	LOGD("*Main::onGameCenterEvent %d, %d, %d",type, param1, param2);
	return false;
}

// デバッグコマンド
bool Main::onDebugCommand(const char *command, int param) {
    LOGD("*Main::onDebugCommand %s, %d",command, param);
	return false;
}
