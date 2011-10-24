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

#ifndef GL_FIGURE_H
#define GL_FIGURE_H

#include "glcommon.h"

#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

class Joint;

// attribute index
enum {
	ATTRIB_VERTEX,
	ATTRIB_NORMAL,
	ATTRIB_TEXCOORD,
//	ATTRIB_MASKCOORD,
//	ATTRIB_LIGHTMAP,
	ATTRIB_INDEX,
	ATTRIB_JOINT_1,
	ATTRIB_JOINT_2,
	ATTRIB_WEIGHT_1,
	ATTRIB_WEIGHT_2,
	NUM_ATTRIBUTES
};

enum {
	VBO_VERTEX,
	VBO_NORMAL,
	VBO_TEXCOORD,
//	VBO_MASKCOORD,
//	VBO_LIGHTMAP,
	VBO_INDEX,
	VBO_ELEMENT,
	VBO_JOINT_1,
	VBO_JOINT_2,
	VBO_WEIGHT_1,
	VBO_WEIGHT_2,
	NUM_VBO
};

/**
 * フィギュアクラス.
 */
class Figure {
private:
	/**
	 *
	 */
	GLuint buildVBO(void *data, int size, GLenum buffer);

public:
	std::vector<GLfloat> *vertices;			//!< 頂点データ.
	std::vector<GLfloat> *normals;			//!< 法線データ.
	std::vector<GLfloat> *textureCoords;	//!< uvデータ.
//	std::vector<GLfloat> *maskCoords;		//!< マスク用uvデータ.
//	std::vector<GLfloat> *lightMap;			//!< ライトマップデータ.
	std::vector<GLushort> *joint1;			//!< joint1データ.
	std::vector<GLushort> *joint2;			//!< joint2データ.
	std::vector<GLfloat> *weight1;			//!< weight1データ.
	std::vector<GLfloat> *weight2;			//!< weight2データ.
	std::vector<short> *vertexIndexes;	//!< 頂点インデックスデータ.
	Joint *joint;							//!< root joint.
	bool hasNormals;
	bool hasTexture;
//	bool hasMask;
//	bool hasLightmap;
	bool hasJoint;

	GLuint vaoName;				//!< VAOの名前.
	GLuint vboNames[NUM_VBO];	//!< VBOの名前リスト.

	Matrix3D *transForm;	//!< フィギュアのマトリクス.
//	Material *material;

	/**
	 * コンストラクタ.
	 */
	Figure();

	/**
	 * デストラクタ.
	 */
	~Figure();

	/**
	 * フィギュアの描画を行います.
	 */
	void draw();
	void drawLines(GLfloat width);
	void drawPoints();

	/**
	 * VAO (Vertex Array Object)を作成します.
	 */
	void buildVAO();

	/**
	 * VAOを破棄します.
	 */
	void destroyVAO();

	/**
	 * VBO (Vertex Buffer Object)をバインドします.
	 */
	void bindVBO();

	/**
	 * Attribute の設定をONにします.
	 * <br><br>
	 * @param attrib ONにする属性
	 */
	void enableAttribute(GLuint attrib);

	/**
	 * 頂点座標を追加します.
	 * @param[in] v 追加する頂点
	 * @param[in] len 追加する頂点の個数
	 */
	void addVertices(const GLfloat *v, int len);

	/**
	 * 法線ベクトルを追加します.
	 * @param[in] v 追加する法線ベクトル
	 * @param[in] len 追加する法線ベクトルの個数
	 */
	void addNormal(const GLfloat *v, int len);

	/**
	 * uvを追加します.
	 * @param[in] v 追加するuv
	 * @param[in] len 追加するuvの個数
	 */
	void addTextureCoords(const GLfloat *v, int len);

	/**
	 * マスク用のUVを追加します.
	 * @param[in] v 追加するuv
	 * @param[in] len 追加するuvの個数
	 */
	void addMaskCoords(const GLfloat *v, int len);

	/**
	 * ライトマップ座標を追加します.
	 * @param[in] v 追加するライトマップ座標
	 * @param[in] len 追加するライトマップ座標の個数
	 */
	void addLightMap(const GLfloat *v, int len);

	void addJoints(const GLushort *j1, const GLushort *j2, int len);
	void addWeights(const GLfloat *w1, const GLfloat *w2, int len);

	/**
	 * 面データを追加します.
	 * @param[in] v 追加する面データ
	 * @param[in] len 追加する面データの個数
	 */
	void addVertexIndexes(const GLushort *v, int len);
};

#endif // end of GL_FIGURE_H
