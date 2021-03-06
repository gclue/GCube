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
 * ImageAnimationView.h
 *
 *  Created on: 2011/10/08
 *      Author: GClue, Inc.
 */
#ifndef IMAGEANIMATIONVIEW_H_
#define IMAGEANIMATIONVIEW_H_

#include <map>
#include <vector>

#include "ViewGroup.h"

/**
 * フレームID.
 */
enum AnimationFrameID {
	AnimationFrameID_None = 0,
};

/**
 * 画像のアニメーション通知リスナー.
 */
class ImageAnimationListener {
public:
	virtual ~ImageAnimationListener() {}
	/**
	 * アニメーションが完了したときに呼び出します.
	 * @param[in] frameId 完了したフレームのID
	 */
	virtual void onFrameEnd(int frameId) = 0;
};

/**
 * フレームアニメーションを保持する値.
 */
class AnimationFrame {
public:
	std::vector<int> indexs;	//!< ImageViewのインデックスを保持
	std::vector<float> frames;	//!< フレームの表示時間を保持
};

/**
 * Viewを指定されてフレームレートで切り替えていく為のView.
 * ぱらぱらアニメーションを作ることができます.
 */
class ImageAnimationView : public ViewGroup {
private:
	float frame;	//!< フレームの時間
	float time;		//!< 経過時間
	int index;		//!< 現在のインデックス
	int frameId;	//!< 現在指定されているフレームアニメーションのID
	std::map<int, AnimationFrame*> frames;	//!< フレーム保存用
	ImageAnimationListener *listener;	//!< リスナー
	
	void initImageAnimationView();
	
public:
	/**
	 * コンストラクタ.
	 */
	ImageAnimationView();
	
	/**
	 * コンストラクタ.
	 * @param[in] filename ファイル名
	 */
	ImageAnimationView(const char *filename);
	
	/**
	 * コンストラクタ.
	 * @param[in] context View用のコンテキスト
	 * @deprecated コンテキストを使用しないように変更したので、コンストラクタで設定する必要がない
	 */
	ImageAnimationView(GCContext *context);
	virtual ~ImageAnimationView();
	
	/**
	 * リスナーを設定します.
	 * @param[in] listener リスナー
	 */
	void setImageAnimationListener(ImageAnimationListener *listener);

	/**
	 * Viewを切り替える時間を指定します.
	 * @param frame フレーム
	 */
	void setFrame(float frame);

	/**
	 * フレームアニメーションを追加します.
	 * @param id アニメーションのID(1以上を指定すること)
	 * @param index 画像のインデックス
	 * @param frame 画像を表示する時間
	 */
	void addAnimationFrame(int id, int index, float frame);

	/**
	 * フレームアニメーションを追加します.
	 * @param id アニメーションのID(1以上を指定すること)
	 * @param af アニメーション
	 */
	void addAnimationFrame(int id, AnimationFrame *af);

	/**
	 * 再生するフレームアニメーションのIDを設定します.
	 * 指定されたIDのフレームアニメーションが存在しない場合には何も処理しません。
	 * @param id フレームアニメーションのID
	 */
	void setAnimationFrameIndex(int id);
	
	/**
	 * アニメーションデータが記載されたJSONファイルを読み込みます。
	 * @param[in] filename ファイル名
	 */
	void loadFile(const char *filename);
	
	/**
	 * アニメーションデータのJSONを読み込みます.
	 * @param[in] json JSONデータ
	 * @exmaple
	 * {
	 *		"png" : "texture/chara_texture.png",
	 *		"txt" : "texture/chara_texture.txt",
	 *		"figure": [
	 *			"chara00.png",
	 *			"chara01.png",
	 *			"chara02.png",
	 *			"chara03.png",
	 *			"chara04.png",
	 *			"chara05.png",
	 *			"chara06.png",
	 *			"chara07.png",
	 *			"chara08.png",
	 *			"chara09.png",
	 *			"chara10.png",
	 *			"chara11.png"
	 *		],
	 *		"animation" : [
	 *			{
	 *				"id" : 1,
	 *				"frame" : [
	 *					{"index" : 0, "time" : 0.12},
	 *					{"index" : 2, "time" : 0.12}
	 *				]
	 *			},
	 *			{
	 *				"id" : 2,
	 *				"frame" : [
	 *					{"index" : 9, "time" : 0.12},
	 *					{"index" :11, "time" : 0.12}
	 *				]
	 *			}
	 *		]
	 *	}
	 */
	void loadJson(const char *json);

	//////////////////////////////////////////////////////////////
	// Viewからの継承
	//////////////////////////////////////////////////////////////

	/**
	 * 指定されたアニメーションを反映して描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 * @param[in] animation 反映するアニメーション
	 */
	virtual void draw(double dt, ViewContext *context);
};

#endif /* IMAGEANIMATIONVIEW_H_ */
