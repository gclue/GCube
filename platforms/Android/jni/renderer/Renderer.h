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
 * Renderer.h
 *
 *  Created on: 2011/05/27
 *      Author: haga
 */
#ifndef RENDERER_H_
#define RENDERER_H_

#include "IScene.h"

class View;
class ViewGroup;
class Camera;
class SimpleShader;

/**
 * レンダラーの基底クラス.
 */
class Renderer {
protected:
	View *root;
	View *dialog;

	SimpleShader *shader;
	Camera *camera;

	float aspect;

public:
	/**
	 * コンストラクタ.
	 */
	Renderer();

	/**
	 * デストラクタ.
	 */
	virtual ~Renderer();

	/**
	 * セットアップを行います.
	 */
	virtual void setup() = 0;

	/**
	 * 画面のリサイズを行います.
	 * @param aspect 画面のアスペクト比
	 */
	virtual void resize(float aspect) = 0;

	/**
	 * setContentViewで設定されたViewから同じIDのViewを取得します.
	 * 指定されたidと同じuserIDを持つViewが見つからない場合には、NULLを返却します。
	 * @param[in] id userID
	 * @return Viewのオブジェクト
	 */
	virtual View* findViewByID(int id);

	virtual View* findViewByID(const char *name);

	/**
	 * 画面の描画を行います.
	 * @param dt 前回描画からの差分時間
	 */
	virtual void render(double dt = 0.033);

	/**
	 * タッチイベント.
	 * @param event タッチイベント
	 */
	virtual void onTouch(TouchEvent &event);

	/**
	 * Viewを設定します.
	 *
	 * 【重要】
	 * 前回設定されたViewは内部でdeleteされてしまうので注意してください。
	 * また、このクラスが削除されたときにも設定されていたViewも一緒に
	 * deleteしますので、注意してください。
	 *
	 * @param[in] view rootに設定するView
	 */
	virtual void setContentView(View *view);

	/**
	 * setContentViewで設定されたViewを取得します.
	 * @return setContentViewで設定されたView
	 */
	virtual View *getContentView();

	/**
	 * ダイアログを設定します.
	 *
	 * 【重要】
	 * 既にダイアログが設定されている場合には、前のダイアログを削除してから
	 * ダイアログを表示します。
	 *
	 * ダイアログが設定中は、setContentViewで設定したViewには
	 * タッチイベントは発生しません。ダイアログのViewの方に渡されます。
	 * ただし、setContentViewで設定したViewのrenderは呼び出されます
	 * ので、ご注意ください。
	 *
	 * @param[in] view ダイアログを表示するView
	 */
	virtual void openDialog(View *view);

	/**
	 * ダイアログを閉じます.
	 */
	virtual void closeDialog();

	/**
	 * ダイアログが存在するかチェックします.
	 * @retval true 存在する
	 * @retval false 存在しない
	 */
	virtual bool isDialog();

	/**
	 * カメラを設定します.
	 * @param[in] camera カメラ
	 */
	virtual void setCamera(Camera *camera);

	/**
	 * シェーダを設定します.
	 * @param[in] shader シェーダ
	 */
	virtual void setShader(SimpleShader *shader);
};

#endif /* RENDERER_H_ */
