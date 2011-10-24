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
 * RadioButton.h
 *
 *  Created on: 2011/08/10
 *      Author: GClue, Inc.
 */
#ifndef RADIOBUTTON_H_
#define RADIOBUTTON_H_

#include "View.h"

/**
 * ラジオボタン.
 */
class RadioButton : public View {
private:
	View *view[2];						//!< ボタンの画像

public:
	bool selectFlag;			//!< ラジオボタンが選択されているか(true: 選択中)

	/**
	 * コンストラクタ.
	 * @param context View用のコンテキスト
	 */
	RadioButton(AEContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~RadioButton();

	//////////////////////////////////////////////////////////////
	// Viewからの継承
	//////////////////////////////////////////////////////////////
	/**
	 * 指定されたidと同じuserIDを持つViewを返却します.
	 * もし同じidをもつViewが無かった場合にはNULLを返却します.
	 * @param[in] id Viewに付けられてID
	 * @return Viewのオブジェクト
	 */
	virtual View* findViewByID(int id);

	/**
	 * 指定されたuserObjと同じuserObjを持つViewを返却します.
	 * もし同じuserObjをもつViewが無かった場合にはNULLを返却します.
	 *
	 * func に NULL が渡された場合には、userObjが同じアドレスか
	 * どうかで比較します。
	 *
	 * @param[in] userObj Viewに付けられてuserObj
	 * @param[in] func userObj比較用関数
	 * @return Viewのオブジェクト
	 */
	virtual View* findViewByUserObj(void *userObj, COMPARE_FUNC_PTR *func = NULL);

	/**
	 * タッチイベントを処理します.
	 * @param[in] event タッチイベント
	 * @retval true 次へイベントを渡さない
	 * @retval false 次へイベントを渡す
	 */
	bool onTouch(TouchEvent &event);

	/**
	 * 指定されたアニメーションを反映して描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 * @param[in] animation 反映するアニメーション
	 */
	virtual void draw(double dt, IAnimation *animation = NULL);

	//////////////////////////////////////////////////////////////
	// publicな関数
	//////////////////////////////////////////////////////////////

	/**
	 * 選択いない状態のViewを設定します.
	 * @param[in] view 選択いないときに表示されるView
	 */
	void setView(View *view);

	/**
	 * 選択された状態のViewを設定します.
	 * @param[in] view 選択中のときに表示されるView
	 */
	void setSelectedView(View *view);

	/**
	 * RadioButtonが選択状態かチェックします.
	 * @retval true 選択中
	 * @retval false 上記以外
	 */
	bool isSelected();

	/**
	 * RadioButtonを選択状態にします.
	 */
	void selected();

	/**
	 * 選択状態を設定します.
	 * @param[in] flag 選択状態
	 */
	void setSelected(bool flag);
};

#endif /* RADIOBUTTON_H_ */
