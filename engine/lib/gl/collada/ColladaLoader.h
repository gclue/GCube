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
 * ColladaLoader.h
 *
 *  Created on: 2011/08/25
 *      Author: GClue, Inc.
 */
#ifndef COLLADALOADER_H_
#define COLLADALOADER_H_

#include <stack>
#include <vector>
#include <string>
#include <expat.h>

class Collada;

/**
 * Colladaフォーマットのファイルを読み込むためのローダー.
 *
 */
class ColladaLoader {
private:
	std::vector<int> state;

	Collada *collada;

	/////////////////////////////////////////////////////////////////
	/**
	 * geometryタグの要素を解析します.
	 * @param[in] atts geometryタグの属性データ一覧
	 */
	void parseGeometry(const XML_Char *atts[]);

	/**
	 * sourceタグの要素を解析します.
	 * @param[in] atts sourceタグの属性データ一覧
	 */
	void parseSource(const XML_Char *atts[]);

	/**
	 * float_arrayタグの要素を解析します.
	 * @param[in] atts float_arrayタグの属性データ一覧
	 */
	void parseFloatArray(const XML_Char *atts[]);

	/**
	 * float_arrayタグの要素を解析します.
	 * @param[in] data float_arrayタグの要素
	 * @param[in] len 文字列の長さ
	 */
	void parseFloatArrayBody(const char *data, int len);

	/**
	 * trianglesタグの要素を解析します.
	 * @param[in] atts trianglesタグの属性データ一覧
	 */
	void parseTriangles(const XML_Char *atts[]);

	/**
	 * pタグのボディを解析します.
	 * @param[in] data float_arrayタグのボディ
	 * @param[in] len 文字列の長さ
	 */
	void parsePBody(const char *data, int len);

	/**
	 * 次の半角スペースまでの文字数を取得します.
	 * @param[in] data 文字列
	 * @param[in] len 文字列の長さ
	 * @return 次の半角スペースまでの文字数
	 */
	int nextToken(const char *data, int len);

	/**
	 * 空白やタブをスキップして次の文字列までの文字数を取得します.
	 * @param[in] data 文字列
	 * @param[in] len 文字列の長さ
	 * @return 文字列までの文字数
	 */
	int skipWhitespace(const char *data, int len);

	/**
	 * タグのIDを取得します.
	 * @param[in] name タグ名
	 */
	int getTagID(const char *name);

	/**
	 * 属性のIDを取得します.
	 * @param[in] name 属性名
	 */
	int getAttrID(const char *name);

public:
	ColladaLoader();
	virtual ~ColladaLoader();

	void parse(const char *data, int len);

	void startElement(const XML_Char *name, const XML_Char *atts[]);
	void endElement(const XML_Char *name);
	void bodyElement(const XML_Char *s, int len);
};

#endif /* COLLADALOADER_H_ */
