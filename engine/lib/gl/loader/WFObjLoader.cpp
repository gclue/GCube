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

#include "WFObjLoader.h"
#include "APIGlue.h"
#include "Log.h"

// コンストラクタ
WFObjLoader::WFObjLoader() {
	rightHanded = true;
}

// デストラクタ
WFObjLoader::~WFObjLoader() {
}

// 文字列分解
std::vector<std::string>& WFObjLoader::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// v/vn/vt用の行分割
void WFObjLoader::scanLine(const std::string &line, std::vector<float> &outupt, int max) {
	std::vector<std::string> value;
	this->split(line, ' ', value);
	if (value.size() > max) {
		for (int i=0; i < max; i++) {
			float f = atof(value[i+1].c_str());
			if (!rightHanded && i==2) f = -f;
			outupt.push_back(f);
			//LOGD("%s", value[i].c_str());
		}
	}
}

// ファイルから読み込み
Figure* WFObjLoader::loadFile(const char *fileName) {
	std::vector<char>* data = GCLoadAsset(fileName);
	return this->loadData(data);
}

// データから読み込み
Figure* WFObjLoader::loadData(std::vector<char>* data) {
	
	std::vector<float> vertices;		//!< 頂点
	std::vector<float> normals;			//!< 法線
	std::vector<float> textureCoords;	//!< テクスチャ座標
	
	Figure *fig = new Figure();
	std::istringstream stream(std::string(data->begin(), data->end()));

	// １行毎に処理
	unsigned short faceCount = 0;
	std::string line;
	while(std::getline(stream, line)) {
		//LOGD("line:%s", line.c_str());
		// 改行コードを削除
		if (line[line.size()-1] == '\n') line.erase(line.size()-1);
		if (line[line.size()-1] == '\r') line.erase(line.size()-1);
		
		// ポリゴン
		if (strncmp(line.c_str(), "f ", 2) == 0) {
			// ' 'で分解
			std::vector<std::string> value;
			this->split(line, ' ', value);
			if (value.size() > 3) {
				for (int i=0; i < 3; i++) {
					// '/'で分解
					std::vector<std::string> face;
					this->split(value[i+1], '/', face);
					if (value.size() > 3) {
						for (int j=0; j < 3; j++) {
							//LOGD("%s", face[i].c_str());
							// インデックスは１スタートなので、１引く
							int idx = atoi(face[j].c_str()) -1;
							switch (j) {
								case 0:
									// 頂点座標
									//LOGD("ff:%d,%f,%f,%f", idx, vertices[idx*3], vertices.at(idx*3+1), vertices.at(idx*3+2));
									fig->addVertices(&vertices[idx*3], 3);
									break;
								case 1:
									// テクスチャ座標
									if(idx<0) continue;
									fig->addTextureCoords(&textureCoords[idx*2], 2);
									break;
								case 2:
									// 法線ベクトル
									if(idx<0) continue;
									fig->addNormal(&normals[idx*3], 3);
									break;
							}
						}
						// インデックス作成
						fig->addVertexIndexes(&faceCount, 1);
						faceCount++;
					}
				}
			}
		} else
		// 頂点
		if (strncmp(line.c_str(), "v ", 2) == 0) {
			this->scanLine(line, vertices, 3);
		} else
		// 法線
		if (strncmp(line.c_str(), "vn ", 3) == 0) {
			this->scanLine(line, normals, 3);
		} else
		// テクスチャ
		if (strncmp(line.c_str(), "vt ", 3) == 0) {
			this->scanLine(line, textureCoords, 2);
		}
		
	}
	
	return fig;
}
