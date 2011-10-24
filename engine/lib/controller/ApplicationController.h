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

#ifndef APPLICATIONCONTROLLER_H_
#define APPLICATIONCONTROLLER_H_

#include <map>

#include "IScene.h"
#include "AEContext.h"

class Animation;
class SimpleShader;
class Camera;

/**
 * シーンID.
 */
enum SceneID {
	SceneID_None = 0,		//!< シーンなし
};

/**
 * アプリケーションコントローラクラス.
 */
class ApplicationController : public AEContext {
private:
	float red;		//!< 赤色成分
	float green;	//!< 緑色成分
	float blue;		//!< 青色成分
	bool pause;		//!< ポーズフラグ
	bool changeFlg;	//!< シーン切り替えフラグ

	std::map<int, IScene*> scenes;	//!< シーン保存用
	IScene *activeScene;			//!< アクティブシーン
	int nextSceneID;				//!< 次のシーンのID
	int currentSceneID;				//!< 現在のシーンID
	int preSceneID;					//!< 一つ前のシーンID
	Animation *fadeAnimation;		//!< フェードアニメーション

	static ApplicationController* singleton;

public:
	/**
	 * コンストラクタ.
	 */
	ApplicationController();

	/**
	 * デストラクタ.
	 */
	virtual ~ApplicationController();

	static ApplicationController* getInstance() {
		if (!singleton) {
			singleton = new ApplicationController();
		}
		return singleton;
	}

	/**
	 * 現在カレントになっているSceneを取得します.
	 * @return 現在カレントになっているScene
	 */
	IScene *getActiveScene() {
		return activeScene;
	}

	/**
	 * 一時停止中の状態を取得します.
	 * @return true: 一時停止中
	 */
	inline bool isPause() {
		return pause;
	}

	/**
	 * 再セットアップを行います.
	 */
	void resetup();

	/**
	 * 一時停止します.
	 */
	void onPause();

	/**
	 * 一時停止から復帰します.
	 */
	void onResume();

	/**
	 * リサイズ処理を行います.
	 * @param[in] width 横幅
	 * @param[in] height 縦幅
	 */
	void resize(int width, int height);

	/**
	 * ゲームをステップ実行します.
	 */
	void step(float dt);

	/**
	 * バックキーのイベント.
	 * @return 処理をした場合はtrue,しないでシステムに返す場合はfalse
	 */
	bool onPressBackKey();

	/**
	 * タッチイベント.
	 * @param[in] action アクション
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @param[in] time 時間
	 */
	void onTouch(int action, float x, float y, long time);

	/**
	 * 加速度のセンサーイベント.
	 * @param sensor 加速度のイベント
	 */
	void onMoveSenser(double sensor);

	/**
	 * Javaからのイベントを処理します.
	 * @param[in] type イベントタイプ
	 * @param[in] param1 イベントパラメータ
	 * @param[in] param2 イベントパラメータ
	 * @param[in] param3 イベントパラメータ
	 * @param[in] param4 イベントパラメータ
	 */
	void onGameEvent(int type, int param1, int param2, int param3, int param4, const char *param5);

	/**
	 * シーンの切り替えを行います.
	 */
	void sceneChange(int sceneid);

	/**
	 * 一つ前のシーンに戻ります.
	 * 一つ前のシーンが存在しない場合には、この関数は無視されます。
	 */
	void backScene();

	/**
	 * シーンを追加します.
	 * @param id シーンID
	 * @param scene シーン
	 */
	void addScene(int id, IScene *scene);

	/**
	 * 背景色を設定します.
	 * @param r 赤色成分(0.0〜1.0)
	 * @param g 緑色成分(0.0〜1.0)
	 * @param b 青色成分(0.0〜1.0)
	 */
	void setBackgroundColor(float r, float g, float b);
};

#endif /* APPLICATIONCONTROLLER_H_ */
