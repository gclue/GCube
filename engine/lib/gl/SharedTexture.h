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

#ifndef SHAREDTEXTURE_H_
#define SHAREDTEXTURE_H_

#include "defines.h"
#include <string>

class Figure;
class FigureCache;
class Texture;

/**
 * TexturePackerで出力されるデータを格納する構造体.
 */
struct _TexData {
	std::string name;		//!< 画像ファイル名
	Rectf rect;				//!< テクスチャサイズ
	Rectf padding;			//!< パディング
	int rotate;				//!< 回転
};
typedef struct _TexData TexData;

/**
 * テクスチャ.
 */
class SharedTexture {
protected:
	FigureCache *cache;		//!< フィギュアキャッシュ
	Texture *texture;		//!< テクスチャ
	float dispW;			//!< ディスプレイの横幅
public:
	/**
	 * コンストラクタ.
	 */
	SharedTexture();
    
	/**
	 * デストラクタ.
	 */
	virtual ~SharedTexture();
    
	/**
	 * 対象のIDのTexDataを返します.
	 * @param[in] ID 対象のID
	 * @param[in] sub サブID
	 * @return TexData
	 */
	virtual TexData getTexData(int ID, int sub) = 0;
    
	/**
	 * 対象の画像名のTexDataを返します.
	 * @param[in] name 画像名
	 * @return TexData
	 */
	virtual TexData getTexData(const char *name) = 0;
    
	/**
	 * 画面のサイズを設定します.
	 * デフォルトでは、480に設定してあります。
	 * @param[in] w 横幅
	 */
	void setSize(float w);
    
	/**
	 * 使用しているテクスチャを取得します.
	 * @return テクスチャ
	 */
	Texture& getTexture();
    
	/**
	 * テクスチャを設定します.
	 * @param[in] texture 設定するテクスチャ
	 */
	void setTexture(Texture *texture);
    
	/**
	 * テクスチャを再読み込みします.
	 */
	void reload();
    
	/**
	 * 座標を正規化します.
	 * @param[in] Texture 対象のテクスチャ
	 * @param[in] rect 対象のRect座標
	 * @return 正規化座標
	 */
	Rectf normalize(Texture *tex, Rectf rect);
    
	/**
	 * 対象のテクスチャ用の板ポリゴンを作成します.
	 * @param[in] ID 対象のID
	 * @param[in] sub サブID
	 * @return 板ポリゴン
	 */
	Figure* makePlate(int ID, int sub);
    
	/**
	 * 対象のテクスチャ用の板ポリゴンを作成します（アスペクト比固定）.
	 * @param[in] ID 対象のID
	 * @param[in] sub サブID
	 * @param[in] w ポリゴンの幅
	 * @return 板ポリゴン
	 */
	Figure* makeFixPlate(int ID, int sub, float w);
    
	/**
	 * 対象のテクスチャ用の板ポリゴンを作成します（アスペクト比固定）.
	 * @param[in] ID 対象のID
	 * @param[in] sub サブID
	 * @param[in] h ポリゴンの高さ
	 * @return 板ポリゴン
	 */
	Figure* makeFixHeightPlate(int ID, int sub, float h);
    
	/**
	 * 対象のテクスチャ用の板ポリゴンを作成します.
	 * @param[in] ID 対象のID
	 * @param[in] sub サブID
	 * @param[in] w ポリゴンの横幅
	 * @param[in] w ポリゴンの縦幅
	 * @return 板ポリゴン
	 */
	Figure* makePlateWithSize(int ID, int sub, float w, float h);
    
	/**
	 * 対象のテクスチャ用の板ポリゴンを作成します.
	 * @param[in] name 画像名
	 * @return 板ポリゴン
	 */
	Figure* makePlate(const char *name);
    
	/**
	 * 指定された名前の画像名のFigureを作成します.
	 * @param[in] name 画像名
	 * @param[in] w フィットさせる横幅
	 * @return Figureのインスタンス
	 */
	Figure* makeFixPlate(const char *name, float w);
    
	/**
	 * 対象のテクスチャ用の板ポリゴンを作成します（アスペクト比固定）.
	 * @param[in] ID 対象のID
	 * @param[in] sub サブID
	 * @param[in] h ポリゴンの高さ
	 * @return 板ポリゴン
	 */
	Figure* makeFixHeightPlate(const char *name, float h);
    
	/**
	 * 指定された名前の画像名のFigureを作成します.
	 * @param[in] name 画像名
	 * @param[in] w ポリゴンの横幅
	 * @param[in] h ポリゴンの縦幅
	 * @return Figureのインスタンス
	 */
	Figure* makePlateWithSize(const char *name, float w, float h);
};

#endif /* SHAREDTEXTURE_H_ */
