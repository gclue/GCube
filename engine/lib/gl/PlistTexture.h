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
 * PlistTexture.h
 *
 *  Created on: 2013/06/26
 *      Author: GClue, Inc.
 */
#ifndef __GCube__PlistTexture__
#define __GCube__PlistTexture__

#include "SharedTexture.h"
#include "defines.h"
#include <vector>
#include <string>

/**
 * Plist形式のファイルからTextureを使用するためのクラス.
 */
class PlistTexture : public SharedTexture {
private:
    std::vector<TexData> list;		//!< TexturePackerで出力されるデータリスト
    
	/**
	 * 指定されたPlist形式で出力されたファイルを読み込みます.
	 * @param plist ファイル名
	 */
	void load(const char *plist);

public:
    
    /**
	 * 指定されたplist形式のファイルからインスタンスを生成します。
     *
	 * @param plist ファイル名
	 */
    PlistTexture(const char *plist);
    virtual ~PlistTexture();

    /**
	 * テクスチャのデータを設定します.
	 * @param[in] tex テクスチャ
	 */
	void addTexData(TexData tex);
    
	/**
	 * テクスチャデータを取得します.
	 * @param[in] ID 対象のID
	 * @param[in] sub サブID
	 * @return テクスチャデータ
	 */
	TexData getTexData(int ID, int sub);
    
	/**
	 * 指定された文字列に対応したテクスチャデータを取得します.
	 * @param[in] name 画像名
	 * @return テクスチャデータ
	 */
	TexData getTexData(const char *name);
    
};

#endif /* defined(__GCube__PlistTexture__) */
