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

#ifndef GL_WFOBJLOADER_H
#define GL_WFOBJLOADER_H

#include <vector>
#include <string>
#include <iterator>
#include <sstream>


#include "Figure.h"

/**
 * Obj形式の3Dオブジェクト読み込みクラス.
 */
class WFObjLoader {
private:
	/**
	 * 文字列分割.
	 */
	std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
	
	/**
	 * v/vn/vt用の行分割.
	 */
	void scanLine(const std::string &line, std::vector<float> &outupt, int max);
	
public:
	bool rightHanded;						//!< 右手座標フラグ

	/**
	 * コンストラクタ.
	 */
	WFObjLoader();

	/**
	 * デストラクタ.
	 */
	~WFObjLoader();
	
	/**
	 * ファイルから読み込みます.
	 * @param fileName ファイル名
	 */
	Figure* loadFile(const char *fileName);
	
	/**
	 * データから読み込みます.
	 * @param data データ
	 */
	Figure* loadData(std::vector<char>* data);

};

#endif // end of GL_WFOBJLOADER_H
