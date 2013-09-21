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
#include "defines.h"
#include "GCObject.h"

#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

class Joint;
class JointAnimation;

// attribute index
enum {
	ATTRIB_VERTEX,
	ATTRIB_NORMAL,
	ATTRIB_TEXCOORD,
	ATTRIB_TEXCOORD_MLT,
	ATTRIB_INDEX,
	ATTRIB_JOINTS,
	ATTRIB_COLOR,
	NUM_ATTRIBUTES
};

// vbo index
enum {
	VBO_VERTEX,
	VBO_NORMAL,
	VBO_TEXCOORD,
	VBO_TEXCOORD_MLT,
	VBO_INDEX,
	VBO_ELEMENT,
	VBO_JOINTS,
	VBO_COLOR,
	NUM_VBO
};

/**
 * フィギュアクラス.
 */
class Figure : public GCObject {
private:
	GLuint vaoName;				//!< VAOの名前.
	GLuint vboNames[NUM_VBO];	//!< VBOの名前リスト.
	
	bool hasNormals;	//!< 法線データあり
	bool hasTexture;	//!< uvデータあり
	bool hasTextureMlt;	//!< uv2データあり
	bool hasJoint;		//!< jointデータあり
	bool hasColor;		//!< カラーデータあり
	
	/**
	 * Attribute変数を設定します.
	 * @param attrib 設定するattribute
	 */
	void enableAttribute(GLuint attrib);
	
	/**
	 * VBOを構築します.
	 */
	GLuint buildVBO(void *data, int size, GLenum buffer);

public:
	std::vector<float> vertices;		//!< 頂点データ.
	std::vector<float> normals;			//!< 法線データ.
	std::vector<float> textureCoords;	//!< uvデータ.
	std::vector<float> jointData;		//!< jointデータ.（内容はjoint1, weight1, joint2, weight2の順）
	std::vector<short> vertexIndexes;	//!< 頂点インデックスデータ.
	std::vector<float> colors;			//!< 色データ.
	std::vector<float> textureCoordsMlt;//!< uvデータ.
	
	Matrix3D *transForm;	//!< フィギュアのマトリクス.
	Joint *joint;			//!< ルートジョイント
	Point3f size;			//!< フィギュアのサイズ
	bool useIndex;			//!< インデックスのAttributeを使用する場合はtrue
	bool visible;			//!< 表示する場合はtrue
	const char* name;		//!< フィギュア名
	
	JointAnimation *animation;	//!< アニメーションデータ

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
	
	/**
	 * フィギュアを線で描画します.
	 */
	void drawLines(float width);
	
	/**
	 * フィギュアを点で描画します.
	 */
	void drawPoints();

	/**
	 * 設定したデータを基にフィギュアを構築します.
	 */
	void build();

	/**
	 * バインドします.
	 */
	void bind();
	
	/**
	 * 内部データをクリアします.
	 * （描画はできます、buildをするにはもう一度データの登録が必要になります）
	 */
	void clear();
	
	/**
	 * バッファを破棄します.
	 * （描画はできなくなります）
	 */
	void destroy();

	/**
	 * 頂点座標を追加します.
	 * @param[in] v 追加する頂点
	 * @param[in] len 追加する頂点の個数
	 */
	void addVertices(const float *v, int len);

	/**
	 * 法線ベクトルを追加します.
	 * @param[in] v 追加する法線ベクトル
	 * @param[in] len 追加する法線ベクトルの個数
	 */
	void addNormal(const float *v, int len);

	/**
	 * uvを追加します.
	 * @param[in] v 追加するuv
	 * @param[in] len 追加するuvの個数
	 */
	void addTextureCoords(const float *v, int len);

	/**
	 * uvを追加します.
	 * @param[in] v 追加するuv
	 * @param[in] len 追加するuvの個数
	 */
	void addTextureCoordsMlt(const float *v, int len);
	
	/**
	 * Jointを追加します.
	 * @param[in] j1 １番目jointインデックス
	 * @param[in] w1 １番目Weight
	 * @param[in] j2 ２番目jointインデックス
	 * @param[in] w2 ２番目Weight
	 * @param[in] len Joint個数
	 */
	void addJoints(const unsigned short *j1, const float *w1, const unsigned short *j2, const float *w2, int len);

	/**
	 * インデックスを追加します.
	 * @param[in] v 追加するインデックス
	 * @param[in] len 追加するインデックスの個数
	 */
	void addVertexIndexes(const unsigned short *v, int len);
	
	/**
	 * カラーを追加します.
	 *
	 * @param[in] c 追加する色
	 * @param[in] len 追加する色の個数
	 */
	void addColors(const float *c, int len);
	
	/**
	 * アニメーションを設定します.
	 * @param[in] animation アニメーションデータ
	 */
	void setAnimation(JointAnimation *animation);
	
	/**
	 * アニメーション付きで描画を行います.
	 * @param[in] dt 経過時間(1.0で1秒)
	 */
	void draw(float dt);
};

#endif // end of GL_FIGURE_H
