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
 * View.h
 *
 *  Created on: 2011/07/22
 *      Author: GClue, Inc.
 */
#ifndef VIEW_H_
#define VIEW_H_

#include "defines.h"
#include "Matrix3D.h"
#include "GCContext.h"
#include "GCObject.h"

#define TOUCH_MARGIN (15/320.0)

/**
 * タッチイベントのタイプを定義します.
 */
enum TouchEventAction {
	touchDown = 0,		//!< タッチダウン
	touchUp = 1,		//!< タッチアップ
	touchMove = 2,		//!< タッチムーブ
	touchCancel = 3,	//!< タッチキャンセル
};

enum BlendingType {
	BLEND_TYPE_ALPHA = 0,
	BLEND_TYPE_ADD,
	BLEND_TYPE_MULTIPLE,
	BLEND_TYPE_REVERSE,
};

/**
 * タッチイベント.
 */
struct _TouchEvent {
	float x;			//!< x座標
	float y;			//!< y座標
	int type;			//!< イベントタイプ
	long time;			//!< イベントが発生した時間
};
typedef struct _TouchEvent TouchEvent;


/**
 * userObj1 と userObj2 のオブジェクトを比較して、値を返却します.
 * @param[in] userObj1 Viewに設定されていたuserObj
 * @param[in] userObj2 findViewByUserObjに設定されたuserObj
 * @retval 0 同じ
 * @retval 0以外 違うオブジェクト
 */
typedef int (*COMPARE_FUNC_PTR)(void* userObj1, void* userObj2);

class Animation;
class Camera;
class Figure;
class IAnimation;
class SimpleShader;
class Texture;
class CommonTexture;
class View;


class IViewTouchListener {
	
public:
	virtual ~IViewTouchListener(){};
	virtual void onViewTouchEvent(View *view) = 0;
};

/**
 * 描画するための基底クラス.
 */
class View : public GCObject{
protected:
	/**
	 * 引数に渡されたマトリクスにViewに設定している値をかけていきます.
	 * 最終的には、親ViewのマトリクスなどもかかりViewを表示するマトリクスになります。
	 * @param[out] m マトリクス
	 * @param[out] a アルファ値
	 * @param[out] b ブライトネス
	 */
	virtual void testMatrix3D(Matrix3D *m, float *a, float *b);

	/**
	 * Viewが表示されている位置の計算を行います.
	 * @param[out] p 位置を格納するポインタ
	 */
	virtual void testMatrix2D(Pointf *p, Pointf *s, float *r);

public:
	GCContext *context;			//!< Viewを描画するためのコンテキスト
	IAnimation *animation;		//!< アニメーション

	View *parent;				//!< 親になるView

	Texture *texture;			//!< 使用するテクスチャ

	Pointf point;				//!< 表示位置
	Pointf size;				//!< 表示サイズ
	Pointf scale;				//!< スケール
	float rotate;				//!< 回転
	float alpha;				//!< アルファ値
	float bright;				//!< ブライト
	float aspect;				//!< アスペクト比

	bool visible;				//!< 表示フラグ(true: 表示, false: 非表示)
	bool clickable;				//!< クリック判定の有無(true: クリック可能, false: クリック不可能)
	bool absolute;				//!< 絶対座標で計算を行うフラグ(true: 絶対座標)

	void *userObj;				//!< オブジェクト判別用オブジェクト
	int userID;					//!< オブジェクト判別用のID
	
	int blendType;				//!< アルファブレンド、加算減算合成を識別するタイプ
	
	IViewTouchListener *touchListener;	//!< タッチイベントのリスナー.

	/**
	 * コンストラクタ.
	 * @param[in] context コンテキスト
	 */
	View(GCContext *context);

	/**
	 * デストラクタ.
	 */
	virtual ~View();

	/**
	 * アスペクト比を設定します.
	 * @param[in] aspect アスペクト比
	 */
	virtual void setAspect(float aspect) {
		this->aspect = aspect;
	}

	/**
	 * 表示フラグの有無を取得します.
	 * @retval true 表示される
	 * @retval false 非表示
	 */
	virtual bool isVisible() {
		return visible;
	}

	/**
	 * クリック判定の有無を取得します.
	 * @retval true クリック可能
	 * @retval false クリック不可能
	 */
	virtual bool isClickable() {
		return clickable;
	}
    
    virtual bool isTouchable() {
        return visible&&clickable;
    }
    
    

	/**
	 * Viewの表示位置を設定します.
	 * @param[in] pos 表示位置
	 */
	virtual void setPosition(Pointf pos);

	/**
	 * Viewの表示位置を設定します.
	 * @param[in] x x座標
	 * @param[in] y y座標
	 */
	virtual void setPosition(float x, float y);

	/**
	 * Viewのサイズを設定します.
	 * @param[in] size サイズ
	 */
	virtual void setSize(Pointf size);

	/**
	 * Viewのサイズを設定します.
	 * @param[in] w 横幅
	 * @param[in] h 縦幅
	 */
	virtual void setSize(float w, float h);

	/**
	 * Viewの横幅を取得します.
	 * @return 横幅
	 */
	virtual float getWidth() {
		return this->size.x;
	}

	/**
	 * Viewの縦幅を取得します.
	 * @return 縦幅
	 */
	virtual float getHeight() {
		return this->size.y;
	}

	/**
	 * スケールを設定します.
	 * @param sx 横幅のスケール
	 * @param sy 縦幅のスケール
	 */
	virtual void setScale(float sx, float sy);

	/**
	 * 回転を設定します.
	 * @param rotate 回転
	 */
	virtual void setRotate(float rotate);

	/**
	 * アルファ値を設定します.
	 * @param alpha アルファ値
	 */
	virtual void setAlpha(float alpha);

	/**
	 * テクスチャを設定します.
	 * @param[in] texture 設定するテクスチャ
	 */
	virtual void setTexture(Texture *texture);

	/**
	 * アニメーションを開始します.
	 * @param[in] animation アニメーション
	 */
	virtual void startAnimation(IAnimation *animation);

	/**
	 * アニメーションを停止します.
	 */
	virtual void stopAnimation();

	/**
	 * アニメーション中かチェックします.
	 * @retval true アニメーション中
	 * @retval false 上記以外
	 */
	virtual bool isAnimation();

	/**
	 * 指定された座標がボタンの範囲内にいるのかチェックします.
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @retval true 範囲内
	 * @retval false 範囲外
	 */
	virtual bool isBound(float x, float y);

	/**
	 * Viewの表示フラグを設定します.
	 * @param[in] v 表示フラグ(true: 表示, false: 非表示)
	 */
	virtual void setVisible(bool v);

	/**
	 * Viewのクリックフラグを設定します.
	 * @param[in] c 表示フラグ(true: クリック可能, false: クリック不可能)
	 */
	virtual void setClickable(bool c);

	/**
	 * userIDに値を設定します.
	 *
	 * デフォルトでは-1が設定されていますので、
	 * 判別したい場合には他の値を設定してください。
	 *
	 * @param[in] id 設定するID
	 */
	virtual void setUserID(int id);

	/**
	 * userIDの値を取得します.
	 * @return userIDの値
	 */
	virtual int getUserID();

	/**
	 * userObjに値を設定します.
	 *
	 * デフォルトではNULLが設定されていますので、
	 * 判別したい場合には他の値を設定してください。
	 *
	 * @param[in] obj userObjに設定する値
	 */
	virtual void setUserObj(void *obj);

	/**
	 * userObjの値を取得します.
	 * @return userObjの値
	 */
	virtual void* getUserObj();

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
	 * タッチイベントを通知します.
	 * @param[in] event タッチイベント
	 * @retval true 次へイベントを渡さない
	 * @retval false 次へイベントを渡す
	 */
	virtual bool onTouch(TouchEvent &event);

	/**
	 * Viewの描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 */
	virtual void render(double dt);
	
	
	virtual void setBlendType(int type);
	
	
	virtual void setOnTouchEventListener(IViewTouchListener *listener);
	

	/**
	 * Viewの描画を行います.
	 * アニメーションがない場合にはanimationにはNULLが渡されます.
	 * @param[in] dt 前回描画からの経過時間
	 * @param[in] animation アニメーション
	 */
	virtual void draw(double dt, IAnimation *animation = NULL) = 0;
	
	
	
};

#endif /* VIEW_H_ */
