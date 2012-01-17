/*
 * main.cpp
 *
 */

#include "main.h"
#include "ApplicationController.h"
#include "APIGlue.h"
#include "SceneTitle.h"
#include "Scene3D.h"
#include <time.h>

// 初期化処理.
void Main::initApplicationController() {
	LOGD("*Main::initApplicationController()");
	
	// 乱数シード
	srand((unsigned) time(NULL));

	SceneTitle *scene1 = new SceneTitle(controller);
	Scene3D *scene2 = new Scene3D(controller);
	
	controller->addScene(1, scene1);
	controller->addScene(2, scene2);
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

// デバッグコマンド
bool Main::onDebugCommand(const char *command, int param) {
	return false;
}
