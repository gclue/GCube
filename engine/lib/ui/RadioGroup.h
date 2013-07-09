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
 * RadioGroup.h
 *
 *  Created on: 2011/08/10
 *      Author: GClue, Inc.
 */
#ifndef RADIOGROUP_H_
#define RADIOGROUP_H_

#include "ViewGroup.h"

class RadioButton;
class RadioGroup;

/**
 * ボタンがクリックされたときのイベントを通知するリスナー.
 */
class OnCheckedChangeListener {
public:
	virtual ~OnCheckedChangeListener() {}
	/**
	 * ボタンがクリックされたイベント.
	 * @param[in] group グループ
	 * @param[in] btn 選択されたラジオボタン
	 */
	virtual void onCheckedChanged(RadioGroup *group, RadioButton *btn) = 0;
};

/**
 * ラジオボタンをまとめるグループ.
 */
class RadioGroup : public ViewGroup {
private:
	OnCheckedChangeListener *listener;
	bool isTouch;		//!< タッチフラグ
	Pointf startPos;		//!< タッチ開始位置
	Pointf currentPos;		//!< タッチ位置
public:
	/**
	 * コンストラクタ.
	 */
	RadioGroup();
	
	/**
	 * コンストラクタ.
	 * @param context View用のコンテキスト
	 * @deprecated コンテキストを使用しないように変更したので、コンストラクタで設定する必要がない
	 */
	RadioGroup(GCContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~RadioGroup();

	//////////////////////////////////////////////////////////////
	// Viewからの継承
	//////////////////////////////////////////////////////////////

	/**
	 * タッチイベントを処理します.
	 * @param[in] event タッチイベント
	 * @retval true 次へイベントを渡さない
	 * @retval false 次へイベントを渡す
	 */
	bool onTouch(TouchEvent &event);

	//////////////////////////////////////////////////////////////
	// publicな関数
	//////////////////////////////////////////////////////////////

	/**
	 * リスナーを設定します.
	 * @param[in] listener リスナー
	 */
	void setOnCheckedChangeListener(OnCheckedChangeListener *listener);

	/**
	 * ラジオボタンを追加します.
	 */
	void addRadioButton(RadioButton *view);

	/**
	 * 指定されたViewにフォーカスを移動します.
	 * @param[in] btn フォーカスを移動するボタン
	 */
	void setSelected(RadioButton *btn);
};

#endif /* RADIOGROUP_H_ */
