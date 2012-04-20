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

/*
 * Scene.h
 *
 *  Created on: 2011/09/15
 *      Author: GClue, Inc.
 */
#ifndef SCENE_H_
#define SCENE_H_

#include <map>
#include "IScene.h"

class Layer;

/**
 * シーンクラス.
 */
class Scene : public IScene {
private:
	typedef IScene super;

protected:
	std::map<int, Layer*> layers;	//!< レイヤー

public:
	/**
	 * コンストラクタ.
	 */
	Scene(ApplicationController *controller);

	/**
	 * デストラクタ.
	 */
	virtual ~Scene();
	

	/**
	 * レイヤーを追加します.
	 * @param id レイヤーID (1以上の値を指定すること)
	 * @param layer 追加するレイヤー
	 */
	void addLayer(int id, Layer *layer);

	/**
	 * 指定したIDに対応するLayerを取得します.
	 * 指定したIDに対応するLayerが存在しない場合にはNULLを返却します.
	 * @param id レイヤーID
	 * @return レイヤー
	 */
	Layer *getLayer(int id);
	
	/**
	 * 指定したIDのレイヤを削除します.
	 * @param[in] id レイヤーID
	 * @return 削除できたかどうか.
	 */
	bool removeLayer(int id);
	
	
	void removeAllLayers();

	//////////////////////////////////////////////////////////
	// IScene の実装
	//////////////////////////////////////////////////////////
	/**
	 * セットアップ処理を行います.
	 */
	virtual void setup();

	/**
	 * リサイズ処理を行います.
	 * @param[in] width 横幅
	 * @param[in] height 縦幅
	 */
	virtual void resize(int width, int height);

	/**
	 * ステップ実行します.
	 */
	virtual void step(float dt);

	/**
	 * 活性化します.
	 */
	virtual void onActivate();

	/**
	 * 休止します.
	 */
	virtual void onSuspend();

	/**
	 * 活性化してシーンが切り替え終わったこと通知します.
	 */
	virtual void onStart();

	/**
	 * 非活性化してシーンが切り替え終わったこと通知します.
	 */
	virtual void onEnd();

	/**
	 * コンテキストが切り替わったことを通知します.
	 */
	virtual void onContextChanged();

	/**
	 * バックキーのイベント.
	 * @return 処理をした場合はtrue,しないでシステムに返す場合はfalse
	 */
	virtual bool onPressBackKey();

	/**
	 * タッチイベント.
	 * @param[in] event タッチイベント
	 */
	virtual bool onTouch(TouchEvent &event);

	/**
	 * 加速度センサーイベント.
	 * @param senser 加速度の値
	 */
	virtual void onMoveSensor(double sensor){};

	/**
	 * ゲームイベントを受け取るメソッド.
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	virtual void onGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5){};
	
	/**
	 * ツイッターイベントを受け取ります.
	 * @param[in] type イベントタイプ.
	 * @param[in] param1 パラメータ.
	 */
	virtual void onTwitterEvent(int type, int param1){};
};

#endif /* SCENE_H_ */
