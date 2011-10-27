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
 * Layer2D.h
 *
 *  Created on: 2011/09/15
 *      Author: GClue, Inc.
 */
#ifndef LAYER2D_H_
#define LAYER2D_H_

#include "Layer.h"
#include "View.h"

class ViewGroup;
class Camera;
class SimpleShader;

/**
 * 2Dを描画するためのレイヤー.
 */
class Layer2D : public Layer {
private:
	View *root;				//!< ルートとなるView
	View *dialog;			//!< ダイアログ

	float aspect;			//!< アスペクト比

public:
	/**
	 * コンストラクタ.
	 * @param context 描画するためのコンテキスト
	 */
	Layer2D(GCContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~Layer2D();

	/**
	 * setContentViewで設定されたViewから同じIDのViewを取得します.
	 * 指定されたidと同じuserIDを持つViewが見つからない場合には、NULLを返却します。
	 * @param[in] id userID
	 * @return Viewのオブジェクト
	 */
	virtual View* findViewByID(int id);

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

	//////////////////////////////////////////////////////////
	// Layer の実装
	//////////////////////////////////////////////////////////
	/**
	 * セットアップを行います.
	 */
	virtual void setup();

	/**
	 * 画面のリサイズを行います.
	 * @param aspect 画面のアスペクト比
	 */
	virtual void resize(float aspect);

	/**
	 * 画面の描画を行います.
	 * @param dt 前回描画からの差分時間
	 */
	virtual void render(double dt = 0.033);

	/**
	 * タッチイベント.
	 * @param event タッチイベント
	 * @return true: 次のレイヤーにイベントを渡さない、false: 次のレイヤーにイベントを渡す
	 */
	virtual bool onTouch(TouchEvent &event);
	
};

#endif /* LAYER2D_H_ */
