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
 * PackerTexture.cpp
 *
 *  Created on: 2011/08/11
 *      Author: GClue, Inc.
 */
#include "PackerTexture.h"
#include "Figure.h"
#include "Texture.h"
#include "APIGlue.h"
#include "PlistParser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PackerTexture::PackerTexture(const char *texname, const char *packername) {
	list.clear();
	if (texname) {
		load(texname);
	}
	if (packername) {
		load(packername);
	}
}

PackerTexture::~PackerTexture() {
	LOGD("PackerTextuer destructor");
	list.clear();
}

void PackerTexture::addTexData(TexData& tex) {
	list.push_back(tex);
}

TexData PackerTexture::getTexData(int ID, int sub) {
	return list.at(ID);
}

TexData PackerTexture::getTexData(const char *name) {
	std::vector<TexData>::iterator it = list.begin();
	while (!list.empty() && it != list.end()) {
		TexData dat = *it;
		if (strcmp(dat.name.c_str(), name) == 0) {
			return dat;
		}
		it++;
	}
	return list.at(0);
}

void PackerTexture::load(const char *packername) {
	LOGI("PackerTexture load.");
	std::vector<char> *fdata = GCLoadAsset(packername);
	if (fdata == NULL) {
		return ;
	}
	fdata->push_back('\0');
    
	const char *data = (const char *) &(*fdata)[0];
	int size = fdata->size();
	int count = 0;
	while (count < size - 1) {
		count += parseLine(&data[count]);
	}
	delete fdata;
	LOGI("PackerTexture load end.");
}

int PackerTexture::parseLine(const char *data) {
	TexData tex;
	int cnt = 0;
	int start = 0;
	int state = 0;
	while (true) {
		// CSVなので、カンマがあった場合、もしくは改行コードが有った場合には
		// カラムが終了なのでデータをそれぞれ格納する
		if (data[cnt] == ',' || data[cnt] == '\n' || data[cnt] == '\r') {
			switch (state) {
                case 0:	// 画像の名前
                    tex.name.append(&data[start], cnt - start);
                    break;
                case 1:
                    tex.rect.left = atoi(&data[start]) + 0.5;
                    break;
                case 2:
                    tex.rect.top = atoi(&data[start]) + 0.5;
                    break;
                case 3:
                    tex.rect.right = atoi(&data[start]) + tex.rect.left - 1;
                    break;
                case 4:
                    tex.rect.bottom = atoi(&data[start]) + tex.rect.top - 1;
                    break;
                case 5:
                    tex.padding.left = atoi(&data[start]);
                    break;
                case 6:
                    tex.padding.top = atoi(&data[start]);
                    break;
                case 7:
                    tex.padding.right = atoi(&data[start]) + tex.padding.left;
                    break;
                case 8:
                    tex.padding.bottom = atoi(&data[start]) + tex.padding.top;
                    break;
			}
			// カンマの分はスキップしておく
			start = cnt + 1;
			// 次のカラムに移動
			state++;
            
			// 改行コード、もしくはNULLストップがあった場合には終了
			if (data[cnt] == '\n' || data[cnt] == '\r' || data[cnt] == '\0') {
				break;
			}
		}
		cnt++;
	}
    
    // 今のところ回転に対応していないので0で初期化
    tex.rotate = 0;
    
	list.push_back(tex);
    //	LOGI("-- %s -------", tex.name.c_str());
    //	LOGI("Rect (%f, %f, %f, %f)", tex.rect.left, tex.rect.top, tex.rect.right, tex.rect.bottom);
    //	LOGI("Padding (%f, %f, %f, %f)", tex.padding.left, tex.padding.top, tex.padding.right, tex.padding.bottom);
	return cnt + 1;
}
