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
 * FigureCache.h
 *
 *  Created on: 2011/08/22
 *      Author: GClue, Inc.
 */
#ifndef FIGURECACHE_H_
#define FIGURECACHE_H_

#include <vector>
#include <string>
#include "SharedTexture.h"

class Figure;

/**
 * Figureをキャッシュするためにコンテナ.
 */
class FigureObj {
public:
	int id;				//!< フィギュアのID
	int subId;			//!< フィギュアのサブID
	float w;			//!< ポリゴンの横幅
	float h;			//!< ポリゴンの縦幅
	std::string name;	//!< フィギュアの名前
	Figure *figure;		//!< フィギュア

	/**
	 * コンストラクタ.
	 */
	FigureObj();

	/**
	 * デストラクタ.
	 */
	virtual ~FigureObj();
};

/**
 * Figureをキャッシュして再利用するためのクラス.
 */
class FigureCache {
private:
	std::vector<FigureObj*> cache;		//!< Figureのキャッシュ

public:
	/**
	 * コンストラクタ.
	 * @param tex キャッシュに使用するテクスチャ
	 */
	FigureCache();

	/**
	 * デストラクタ.
	 * 設定されたSharedTextureはdeleteしないので、
	 * 設定先でdeleteしてください。
	 */
	virtual ~FigureCache();

	/**
	 * 指定されたIDのキャッシュが無いか検索します.
	 * 検索して見つからなかった場合にはNULLを返却します。
	 * @param[in] id ID
	 * @param[in] subId サブID
	 * @param[in] w 横幅
	 * @param[in] h 縦幅
	 * @return 検索結果
	 */
	Figure *searchFigure(int id, int subId, float w, float h);

	/**
	 * 指定された名前のキャッシュが無いか検索します.
	 * 検索して見つからなかった場合にはNULLを返却します。
	 * @param[in] name 名前
	 * @param[in] w 横幅
	 * @param[in] h 縦幅
	 * @return 検索結果
	 */
	Figure *searchFigure(const char *name, float w, float h);

	/**
	 * フィギュアをキャッシュします.
	 * @param[in] id ID
	 * @param[in] subId サブID
	 * @param[in] w 横幅
	 * @param[in] h 縦幅
	 * @param[in] figure キャッシュするフィギュア
	 */
	void putFigure(int id, int subId, float w, float h, Figure *figure);

	/**
	 * フィギュアをキャッシュします.
	 * @param[in] name 名前
	 * @param[in] w 横幅
	 * @param[in] h 縦幅
	 * @param[in] figure キャッシュするフィギュア
	 */
	void putFigure(const char *name, float w, float h, Figure *figure);

	/**
	 * キャッシュされているVAOのデータを削除します.
	 */
	void destroyVAO();

	/**
	 * キャッシュされているFigureを再buildVAOを行います.
	 */
	void rebuild();
};

#endif /* FIGURECACHE_H_ */
