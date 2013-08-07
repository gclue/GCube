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
 * ViewGroup.h
 *
 *  Created on: 2011/07/22
 *      Author: GClue, Inc.
 */
#ifndef VIEWGROUP_H_
#define VIEWGROUP_H_

#include "View.h"
#include <vector>
#include <algorithm>
#include <functional>

/**
 * Viewをソートするときに比較を行う関数の定義.
 * @param[in] a 比較対象のView
 * @param[in] b 比較対象のView
 * @return aよりbを後ろにする場合にはtrue, それ以外はfalse
 */
typedef bool (*ViewSortFunc)(View *a, View *b);

/**
 * Viewを複数格納し管理するViewクラス.
 */
class ViewGroup : public View {
public:
	std::vector<View *> views;			//!< Viewを格納するviector

	/**
	 * コンストラクタ.
	 */
	ViewGroup();
	
	/**
	 * コンストラクタ.
	 * @param[in] context コンテキスト
	 * @deprecated コンテキストを使用しないように変更したので、コンストラクタで設定する必要がない
	 */
	ViewGroup(GCContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~ViewGroup();

	/**
	 * Viewを追加します.
	 * 追加されたViewは、ViewGroup削除時にdelete処理を行います。
	 * また、removeViewで削除されたときもdelete処理を行います。
	 * @param[in] view 追加するView
	 */
	void addView(View *view);

	/**
	 * Viewを削除します.
	 * @param[in] view 削除するView
	 */
	void removeView(View *view);

	/**
	 * 指定されたIDと同じIDを持つViewを削除します.
	 * ただし、検索して最初に見つかったViewのみを削除します。
	 * @param id UserID
	 */
	void removeViewByID(int id);

	/**
	 * すべての子Viewを削除します.
	 */
	void removeAllView();

	/**
	 * 指定されたインデックスのViewを取得します.
	 * @param[in] index インデックス
	 * @return Viewのインスタンス
	 */
	View *getView(int index);

	/**
	 * 同じuserObjを取得します.
	 * @param[in] userObj ユーザが設定するオブジェクト
	 * @return Viewのインスタンス
	 */
	View *getView(void *userObj);

	/**
	 * addされたViewの個数を取得します.
	 * @return 個数
	 */
	int count();

	/**
	 * addViewされたViewをソートします。
	 * funcが指定されていない場合には、View::getUserIDの小さい順にソートします。
	 * ViewGroup::draw中に、この関数が呼ばれると動作がおかしくなるので、注意してください。
	 * @param[in] func ソート関数
	 */
	void sort(ViewSortFunc func=NULL);
	
	/**
	 * 指定されたViewを一番上に移動します.
	 * 指定されたviewが存在しない場合には無視します.
	 * ViewGroup::draw中に、この関数が呼ばれると動作がおかしくなるので、注意してください。
	 * @param[in] v 一番上にくるView
	 */
	void front(View *v);
	
	/////////////////////////////////////////////////////////////////
	// Viewからの継承
	/////////////////////////////////////////////////////////////////
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
	 * 指定された座標がボタンの範囲内にいるのかチェックします.
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @retval true 範囲内
	 * @retval false 範囲外
	 */
	virtual bool isBound(float x, float y);
	
	/**
	 * タッチイベントを処理します.
	 * @param[in] event タッチイベント
	 * @retval true 次へイベントを渡さない
	 * @retval false 次へイベントを渡す
	 */
	virtual bool onTouch(TouchEvent &event);

	/**
	 * Viewの描画を行います.
	 * アニメーションがない場合にはanimationにはNULLが渡されます.
	 * @param[in] dt 前回描画からの経過時間
	 * @param[in] animation アニメーション
	 */
	virtual void draw(double dt, ViewContext *context);
};

#endif /* VIEWGROUP_H_ */
