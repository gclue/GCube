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
void WFObjLoader::scanLine(const std::string &line, std::vector<float> &outupt, int max, bool rightHanded) {
	std::vector<std::string> value;
	WFObjLoader::split(line, ' ', value);
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
Figure* WFObjLoader::loadFile(const char *fileName, bool rightHanded) {
	std::vector<char>* data = GCLoadAsset(fileName);
	Figure *out = WFObjLoader::loadData(data);
	out->name = fileName;
	delete data;
	return out;
}

// データから読み込み
Figure* WFObjLoader::loadData(std::vector<char>* data, bool rightHanded) {
	
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
			WFObjLoader::split(line, ' ', value);
			if (value.size() > 3) {
				for (int i=0; i < 3; i++) {
					// '/'で分解
					std::vector<std::string> face;
					WFObjLoader::split(value[i+1], '/', face);
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
			WFObjLoader::scanLine(line, vertices, 3, rightHanded);
		} else
		// 法線
		if (strncmp(line.c_str(), "vn ", 3) == 0) {
			WFObjLoader::scanLine(line, normals, 3, rightHanded);
		} else
		// テクスチャ
		if (strncmp(line.c_str(), "vt ", 3) == 0) {
			WFObjLoader::scanLine(line, textureCoords, 2, rightHanded);
		}
		
	}
	
	return fig;
}






// バイナリを読み込むためのストリーム
class BinaryStream {
private:
	std::vector<char>::iterator it;
	std::vector<char>* data;
	
public:
	BinaryStream(std::vector<char>* data) {
		this->data = data;
		this->it = data->begin();
	}
	
	~BinaryStream() {
		data = NULL;
	}
	
	char readByte() {
		return (*it++);
	}
	
	short readShort() {
		short f;
		char t[2];
		t[1] = (*it++);
		t[0] = (*it++);
		memcpy(&f, t, 2);
		return f;
	}
	
	int readInt() {
		int f;
		char t[4];
		t[3] = (*it++);
		t[2] = (*it++);
		t[1] = (*it++);
		t[0] = (*it++);
		memcpy(&f, t, 4);
		return f;
	}
	
	float readFloat() {
		float f;
		char t[4];
		t[3] = (*it++);
		t[2] = (*it++);
		t[1] = (*it++);
		t[0] = (*it++);
		memcpy(&f, t, 4);
		return f;
	}
	
	bool isEnd() {
		return it == data->end();
	}
};


// 頂点座標のデータ群を表すタグ
#define TYPE_VERTEX 1
// 法線ベクトルのデータ群を表すタグ
#define TYPE_NORMAL 2
// テクスチャのデータ群を表すタグ
#define TYPE_TEXCOOD 3
// インデックスのデータ群を表すタグ
#define TYPE_INDEX 4
// ジョイントのデータ群を表すタグ
#define TYPE_NODE 5
// ジョイントのウェイト群を表すタグ
#define TYPE_WEIGHT 6
// カラーのデータ群を表すタグ
#define TYPE_COLOR 7
// マルチテクスチャ
#define TYPE_TEXCOOD_MLT 8


Figure* WFObjLoader::loadGCBFile(const char *fileName)
{
	std::vector<char>* data = GCLoadAsset(fileName);
	Figure *out = WFObjLoader::loadGCBData(data);
	if (out) {
		out->name = fileName;
		out->build();
	}
	delete data;
	return out;
}

Figure* WFObjLoader::loadGCBData(std::vector<char>* data)
{
	BinaryStream stream(data);
	
	// マジックナンバーチェック
	if (stream.readByte() != 'g' ||
		stream.readByte() != 'c' ||
		stream.readByte() != 'u' ||
		stream.readByte() != 'b') {
		return NULL;
	}
	
	Figure *fig = new Figure();
	
	while (!stream.isEnd()) {
		short type = stream.readShort();
		switch (type) {
			case TYPE_VERTEX:
			{
				int size = stream.readInt();
				for (int i = 0; i < size; i++) {
					float v = stream.readFloat();
					fig->addVertices(&v, 1);
				}
			}	break;
			case TYPE_NORMAL:
			{
				int size = stream.readInt();
				for (int i = 0; i < size; i++) {
					float v = stream.readFloat();
					fig->addNormal(&v, 1);
				}
			}	break;
			case TYPE_TEXCOOD:
			{
				int size = stream.readInt();
				for (int i = 0; i < size; i++) {
					float v = stream.readFloat();
					fig->addTextureCoords(&v, 1);
				}
			}	break;
			case TYPE_INDEX:
			{
				int size = stream.readInt();
				for (int i = 0; i < size; i++) {
					unsigned short v = stream.readInt();
					fig->addVertexIndexes(&v, 1);
				}
			}	break;
			case TYPE_COLOR:
			{
				int size = stream.readInt();
				for (int i = 0; i < size; i++) {
					float v = stream.readFloat();
					fig->addColors(&v, 1);
				}
			}	break;
			case TYPE_WEIGHT:
			{
				loadWeight(stream, fig);
			}	break;
			case TYPE_NODE:
			{
				fig->joint = loadJoint(stream);
			}	break;
			case TYPE_TEXCOOD_MLT:
			{
				int size = stream.readInt();
				for (int i = 0; i < size; i++) {
					float v = stream.readFloat();
					fig->addTextureCoordsMlt(&v, 1);
				}
			}	break;
			default:
			{
				LOGE("Unknown Type. [type=%d]", type);
				return NULL;
			}	break;
		}
	}
	return fig;
}

void WFObjLoader::loadWeight(BinaryStream& stream, Figure *fig)
{
	int weightIndexSize = stream.readInt();
	int weightIndex[weightIndexSize];
	for (int i = 0; i < weightIndexSize; i++) {
		weightIndex[i] = stream.readInt();
	}
	
	int weightsSize = stream.readInt();
	float weights[weightsSize];
	for (int i = 0; i < weightIndexSize; i++) {
		weights[i] = stream.readFloat();
	}
	
	int vcount = stream.readInt();
	GLushort joint1Array;
	GLushort joint2Array;
	GLfloat weight1Array;
	GLfloat weight2Array;
	
	int jidx = 0;
	for (int i = 0; i < vcount; i++) {
		short v = stream.readInt();
		joint1Array = weightIndex[jidx];
		weight1Array = weights[jidx];
		if (v > 1) {
			joint2Array = weightIndex[jidx + 1];
			weight2Array = weights[jidx + 1];
		} else {
			joint2Array = 0;
			weight2Array = 0;
		}
		jidx += v;
		
		// FIXEDME: Figureが2つのウェイトしか持てないので、それ以上のデータが入っていても無視する
		
		fig->addJoints(&joint1Array, &weight1Array, &joint2Array, &weight2Array, 1);
	}
}

Joint* WFObjLoader::loadJoint(BinaryStream& stream)
{
	Joint *jt = new Joint();
	
	bool hasSid = stream.readByte() == 1 ? true : false;
	if (hasSid) {
		short sidSize = stream.readShort();
		for (int i = 0; i < sidSize; i++) {
			jt->sid.append(1, stream.readByte());
		}
	}
	
	bool hasMatrix = stream.readByte() == 1 ? true : false;
	if (hasMatrix) {
		float m[16];
		for (int i = 0; i < 16; i++) {
			m[i] = stream.readFloat();
		}
		jt->baseMatrix->setElements(m);
		jt->transForm->setElements(m);
	}
	
	bool hasBindPoses = stream.readByte() == 1 ? true : false;
	if (hasBindPoses) {
		float m[16];
		for (int i = 0; i < 16; i++) {
			m[i] = stream.readFloat();
		}
		jt->invBindMatrix->setElements(m);
	} else {
		jt->hasMesh = false;
	}
	
	int childrenSize = stream.readShort();
	for (int i = 0; i < childrenSize; i++) {
		short type = stream.readShort();
		if (type != TYPE_NODE) {
			LOGE("Illegal type. [type=%d]", type);
		}
		jt->addChild(loadJoint(stream));
	}
	
	return jt;
}


JointAnimation* WFObjLoader::loadGAVFile(const char *fileName)
{
	std::vector<char>* data = GCLoadAsset(fileName);
	JointAnimation* result = WFObjLoader::loadGAVData(data);
	delete data;
	return result;	
}

JointAnimation* WFObjLoader::loadGAVData(std::vector<char>* data)
{
	BinaryStream stream(data);
	
	// マジックナンバーチェック
	if (stream.readByte() != 'g' ||
		stream.readByte() != 'a' ||
		stream.readByte() != 'n' ||
		stream.readByte() != 'i') {
		return NULL;
	}
	
	JointAnimation *animation = new JointAnimation();
	
	int animationCount = stream.readInt();
	
	for (int i = 0; i < animationCount; i++) {
		JointKeyFrame *frame = new JointKeyFrame();
		
		short sidSize = stream.readShort();
		for (int i = 0; i < sidSize; i++) {
			frame->sid.append(1, stream.readByte());
		}
		
		int matrixSize = stream.readInt();
		for (int j = 0; j < matrixSize; j++) {
			float t = stream.readFloat();
			frame->addTime(&t, 1);
			
			float m[16];
			for (int k = 0; k < 16; k++) {
				m[k] = stream.readFloat();
			}
			frame->addMatrix(m, 1);
		}
		
		animation->animations.push_back(frame);
	}
	
	return animation;
}

