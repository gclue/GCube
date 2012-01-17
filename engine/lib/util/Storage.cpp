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
 * Storage.cpp
 *
 *  Created on: 2011/09/16
 *      Author: GClue, Inc.
 */
#include "Storage.h"
#include "APIGlue.h"
#include "Log.h"

#include <fstream>
#include <stdio.h>


// コンストラクタ.
Storage::Storage() {
	pos = 0;
}

// デストラクタ.
Storage::~Storage() {
}

// ファイルからデータを読み込みます.
bool Storage::readFile(const char *path) {
	std::ifstream fin(path, std::ios::in | std::ios::binary);
	if (fin.fail()) {
		return false;
	}
	// 初期化
	buffer.clear();
	pos = 0;
	// 全ファイル読み込み
	char buffer, ex = 0;
	while(!fin.eof()) {
		fin.read(&buffer, sizeof(char));
		//LOGD("%d,%d",buffer, buffer ^ ex);
		// シーザー複合化
		buffer ^= ex;
		ex = buffer ^ ex;
		//
		this->addData(&buffer, 1);
	};
	fin.close();
	return true;
}

// ファイルへデータを書き込みます.
bool Storage::writeFile(const char *path) {
	std::ofstream fout;
	fout.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
	if (fout.fail()) {
		return false;
	}
	// シーザー暗号化
	char ex = 0;
	for (int i=0; i<buffer.size(); i++) {
		//LOGD("%d,%d",buffer[i], buffer[i] ^ ex);
		buffer[i] ^= ex;
		ex = buffer[i];
	}
	const char *data = (const char *) &buffer[0];
	fout.write(data, buffer.size());
	fout.close();
	return true;
}

// データを追加します.
void Storage::addData(const char* data, int len) {
	std::vector<char> tmp(data, data+len);
	buffer.insert(buffer.end(), tmp.begin(), tmp.end());
}

// 現在取得中の場所から次のデータを取得します.
bool Storage::nextData(void *outData, int size) {
	bool flg = this->getData(outData, pos, size);
	pos += size;
	return flg;
}

// データを取得します.
bool Storage::getData(void *outData, int index, int size) {
	if (size==0) {
		size = buffer.size() - index;
	}
	if (index + size > buffer.size()) {
		return false;
	}
	memcpy(outData, &buffer[index], size);
	return true;
}

// データサイズを取得します.
int Storage::length() {
	return buffer.size();
}

// 保存先のパスを取得します.
const char* Storage::getStoragePath(const char* fileName) {
	return GCGetStoragePath(fileName);
}
