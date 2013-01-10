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
#include "Joint.h"
#include "JointAnimation.h"

class BinaryStream;

/**
 * Obj形式の3Dオブジェクト読み込みクラス.
 */
class WFObjLoader {
private:
	/**
	 * 文字列分割.
	 */
	static std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
	
	/**
	 * v/vn/vt用の行分割.
	 */
	static void scanLine(const std::string &line, std::vector<float> &outupt, int max, bool rightHanded);
	
	/**
	 * スキニング用のデータを読み込みます.
	 */
	static void loadWeight(BinaryStream& stream, Figure *fig);
	
	/**
	 * ジョイントのデータを読み込みます.
	 */
	static Joint* loadJoint(BinaryStream& stream);

public:
	/**
	 * ファイルから読み込みます.
	 * @param fileName ファイル名
	 */
	static Figure* loadFile(const char *fileName, bool rightHanded=true);
	
	/**
	 * データから読み込みます.
	 * @param data データ
	 */
	static Figure* loadData(std::vector<char>* data, bool rightHanded=true);
	
	
	/**
	 * .gcbファイルからFigureを読み込みます.
	 * @param fileName ファイル名
	 * @return Figureクラス
	 */
	static Figure* loadGCBFile(const char *fileName);
	
	/**
	 * .gcbデータからFigureを読み込みます.
	 * @param data データ
	 * @return Figureクラス
	 */
	static Figure* loadGCBData(std::vector<char>* data);
	
	/**
	 * .gavファイルからアニメーションデータを読み込みます.
	 * @param fileName ファイル名
	 * @return アニメーションデータ
	 */
	static JointAnimation* loadGAVFile(const char *fileName);
	
	/**
	 * .gavのデータからアニメーションデータを読み込みます.
	 * @param data データ
	 * @return アニメーションデータ
	 */
	static JointAnimation* loadGAVData(std::vector<char>* data);
};

#endif // end of GL_WFOBJLOADER_H
