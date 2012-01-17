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
 * Storage.h
 *
 *  Created on: 2011/09/16
 *      Author: GClue, Inc.
 */
#ifndef STORAGE_H_
#define STORAGE_H_

#include <vector>

class Storage {
private:
	std::vector<char> buffer;	//!< バッファ
	int pos;	//!< 読み込み位置

public:
	/**
	 * コンストラクタ.
	 */
	Storage();
	
	/**
	 * デストラクタ.
	 */
	virtual ~Storage();
	
	/**
	 * 保存先のパスを取得します.
	 * @param[in] fileName パスの最後に付加するファイル名
	 * @return パス
	 */
	static const char* getStoragePath(const char* fileName=0);
	
	/**
	 * データを追加します.
	 * @param[in] data データ
	 * @param[in] len 長さ
	 */
	void addData(const char* data, int len);
	
	/**
	 * データを取得します.
	 * @param[out] outData データ取得用
	 * @param[in] index 取得する場所
	 * @param[in] size 取得するサイズ
	 * @return 取得成功ならtrue
	 */
	bool getData(void *outData, int index=0, int size=0);
	
	/**
	 * 現在取得中の場所から次のデータを取得します.
	 * @param[out] outData データ取得用
	 * @param[in] size 取得するサイズ
	 * @return 取得成功ならtrue
	 */
	bool nextData(void *outData, int size);

	/**
	 * ファイルからデータを読み込みます.
	 * @param[in] path ファイルパス
	 * @return 成功ならtrue
	 */
	bool readFile(const char *path);
	
	/**
	 * ファイルへデータを書き込みます.
	 * @param[in] path ファイルパス
	 * @return 成功ならtrue
	 */
	bool writeFile(const char *path);

	/**
	 * データサイズを取得します.
	 * @return データサイズ
	 */
	int length();
};

#endif /* STORAGE_H_ */
