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

#ifndef SCENE3D_H_
#define SCENE3D_H_

#include "Scene.h"
#include "JointAnimation.h"

/**
 * 3Dサンプルシーンクラス.
 */
class Scene3D : public Scene {
private:
	typedef Scene super;
	float rot;
	Figure *fig;
	JointAnimation *animation;

public:
	/**
	 * コンストラクタ.
	 */
	Scene3D(ApplicationController *controller);
    
	/**
	 * デストラクタ.
	 */
	virtual ~Scene3D();
    
	//////////////////////////////////////////////////////////
	// IScene の実装
	//////////////////////////////////////////////////////////
	
	/**
	 * ステップ実行します.
	 */
	virtual void step(float dt);
    
	/**
	 * タッチイベント.
	 * @param[in] event タッチイベント
	 */
	virtual bool onTouch(TouchEvent &event);
    
	/**
	 * Java側からのイベントを受け取るメソッド.
	 * @param type イベントタイプ
	 * @param param1 イベントパラメータ
	 * @param param2 イベントパラメータ
	 * @param param3 イベントパラメータ
	 * @param param4 イベントパラメータ
	 * @param param5 イベントパラメータ
	 */
	virtual void onGameEvent(int type, int param1, int param2, int param3, int param4, const char *param5);
};

#endif /* SCENE3D_H_ */
