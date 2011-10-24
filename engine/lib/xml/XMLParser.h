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
 * XMLParser.h
 *
 *  Created on: 2011/09/17
 *      Author: GClue, Inc.
 */
#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <expat.h>

/**
 * XMLをSAXで解析します.
 */
class XMLParser {
public:
	/**
	 * コンストラクタ.
	 */
	XMLParser();
	/**
	 * デストラクタ.
	 */
	virtual ~XMLParser();

	/**
	 * 指定されたxmlを解析します.
	 * @param data XMLデータ
	 * @param len XMLのサイズ
	 * @return 解析結果
	 */
	int parse(const char *data, int len);

	/**
	 * Asssetからファイルを読み込みます.
	 * @param fileName ファイル名
	 * @return 解析結果
	 */
	int parseFromAsset(const char *fileName);

	/**
	 * スタートタグがヒットしたときに呼び出されます.
	 * @param name タグ名
	 * @param atts タグの属性リスト
	 */
	virtual void startElement(const XML_Char *name, const XML_Char *atts[]) = 0;

	/**
	 * エンドタグがヒットしたときに呼び出されます.
	 * @param name タグ名
	 */
	virtual void endElement(const XML_Char *name) = 0;

	/**
	 * タグのコンテンツがヒットしたときに呼び出されます.
	 * @param s コンテンツ
	 * @param len コンテンツのサイズ
	 */
	virtual void bodyElement(const XML_Char *s, int len) = 0;
};

#endif /* XMLPARSER_H_ */
