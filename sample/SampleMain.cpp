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

#include "SampleMain.h"
#include "ApplicationController.h"
#include "APIGlue.h"
#include "SceneTitle.h"
#include "SceneSample1.h"
#include "SceneTwitterTest.h"
#include "Scene3D.h"
#include <time.h>
#include "MathExpression.h"
#include "SceneGameCenterSample.h"
#include "SceneAdMobView.h"

// 初期化処理.
void SampleMain::initApplicationController() {
	LOGD("*Main::initApplicationController()");
	
	// 乱数シード
	srand((unsigned) time(NULL));

	SceneTitle *scene1 = new SceneTitle(controller);
	Scene3D *scene2 = new Scene3D(controller);
    SceneSample1 *sceneSample1 = new SceneSample1(controller);
    SceneTwitterTest *scene3 = new SceneTwitterTest(controller);
	SceneGameCenterSample *scene4 = new SceneGameCenterSample(controller);
	SceneAdMobView *scene5 = new SceneAdMobView(controller);
	
	controller->addScene(1, scene1);
	controller->addScene(2, scene2);
//    controller->addScene(2, scene3);
//    controller->addScene(3, sceneSample1);
//	controller->addScene(4, scene4);
//	controller->addScene(5, scene5);
	
	controller->sceneChange(2);
    
////    MathExpression *mathEx = new MathExpression("12+0.5*3.567/0.0003");
////    float ret = mathEx->eval();
////    LOGD("answer %f",ret);
//    delete mathEx;
}

// 一時停止処理.
void SampleMain::onPause() {
	LOGD("*SampleMain::onPause()");
}

// 一時停止から復帰処理.
void SampleMain::onResume() {
	LOGD("*SampleMain::onResume()");
}

// データ保存.
void SampleMain::saveData() {
	LOGD("*SampleMain::saveData()");
}

// ゲームイベント受信.
bool SampleMain::onGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5) {
	LOGD("*SampleMain::onGameEvent(%d, %d, %ld, %f, %d, %s)", type, param1, param2, param3, param4, param5);
	return false;
}

//ツイッターイベント受信.
bool SampleMain::onTwitterEvent(int type, int param1) {
	LOGD("*SampleMain::onTwitterEvent(%d, %d)",type, param1);
	return false;
}

// iOSのGameCenterイベント受信.
bool SampleMain::onGameCenterEvent(int type, int param1, int param2) {
	LOGD("*SampleMain::onGameCenterEvent %d, %d, %d",type, param1, param2);
	return false;
}

// デバッグコマンド.
bool SampleMain::onDebugCommand(const char *command, int param) {
    LOGD("*SampleMain::onGameCenterEvent %s, %d",command, param);
	return false;
}
