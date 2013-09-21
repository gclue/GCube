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

#include "Figure.h"
#include "Joint.h"
#include "JointAnimation.h"
#include <float.h>

// コンストラクタ
Figure::Figure() {
	
	vaoName = 0;
	for (int i = 0; i < NUM_VBO; i++) {
		vboNames[i] = 0;
	}
	
	transForm = NULL;
	joint = NULL;
	animation = NULL;
	
	hasNormals = false;
	hasTexture = false;
	hasJoint = false;
	hasColor = false;
	useIndex = false;
	visible = true;

	transForm = new Matrix3D();
}

// デストラクタ
Figure::~Figure() {
	destroy();
	delete transForm;
	delete joint;
}

// 頂点追加
void Figure::addVertices(const float *v, int len) {
	std::vector<float> tmp(v, v+len);
	vertices.insert(vertices.end(), tmp.begin(), tmp.end());
}

// 法線追加
void Figure::addNormal(const float *v, int len) {
	std::vector<float> tmp(v, v+len);
	normals.insert(normals.end(), tmp.begin(), tmp.end());
}

// uv追加
void Figure::addTextureCoords(const float *v, int len) {
	std::vector<float> tmp(v, v+len);
	textureCoords.insert(textureCoords.end(), tmp.begin(), tmp.end());
}

// uv追加
void Figure::addTextureCoordsMlt(const float *v, int len) {
	std::vector<float> tmp(v, v+len);
	textureCoordsMlt.insert(textureCoordsMlt.end(), tmp.begin(), tmp.end());
}

// インデックス追加
void Figure::addVertexIndexes(const unsigned short *v, int len) {
	std::vector<unsigned short> tmp(v, v+len);
	vertexIndexes.insert(vertexIndexes.end(), tmp.begin(), tmp.end());
}

// カラー追加
void Figure::addColors(const float *c, int len) {
	std::vector<float> tmp(c, c + len);
	colors.insert(colors.end(), tmp.begin(), tmp.end());
}

// ジョイント追加
void Figure::addJoints(const unsigned short *j1, const float *w1, const unsigned short *j2, const float *w2, int len) {
	for (int i = 0; i < len; i++) {
		float max = w1[i] + w2[i];
		jointData.push_back(j1[i]);
		jointData.push_back(w1[i]/max);
		jointData.push_back(j2[i]);
		jointData.push_back(w2[i]/max);
	}
}

// 描画
void Figure::draw() {
	if (!visible) return;
	GLushort indexCount = vertexIndexes.size();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL);
}

// 描画（線）
void Figure::drawLines(float width) {
	if (!visible) return;
	glLineWidth(width);
	GLushort indexCount = vertexIndexes.size();
	glDrawElements(GL_LINE_LOOP, indexCount, GL_UNSIGNED_SHORT, NULL);
}

// 描画（点）
void Figure::drawPoints() {
	if (!visible) return;
	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_SHORT, NULL);
}

// バインド
void Figure::bind() {
	if (!visible) return;
	
#ifdef USE_VAO
	// VAOをバインド
	glBindVertexArrayOES(vaoName);
	return;
#endif
	
	// VBOをバインド
	// 頂点
	glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_VERTEX]);
	enableAttribute(ATTRIB_VERTEX);

	// 法線
	if (hasNormals) {
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_NORMAL]);
		enableAttribute(ATTRIB_NORMAL);
	}

	// テクスチャ
	if (hasTexture) {
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_TEXCOORD]);
		enableAttribute(ATTRIB_TEXCOORD);
	}
	
	if (hasTextureMlt) {
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_TEXCOORD_MLT]);
		enableAttribute(ATTRIB_TEXCOORD_MLT);
	}
	
	// カラー
	if (hasColor) {
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_COLOR]);
		enableAttribute(ATTRIB_COLOR);
	}

	// ジョイント
	if (hasJoint) {
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_JOINTS]);
		enableAttribute(ATTRIB_JOINTS);
	}

	// インデックス
	if (useIndex) {
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_INDEX]);
		enableAttribute(ATTRIB_INDEX);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboNames[VBO_ELEMENT]);
}

// ビルド
void Figure::build() {
//	LOGD("Figure::build");
	// ビルド済みの場合は無視
//	if (vaoName) {
//		return;
//	}

	// Create a vertex array object (VAO)
#ifdef USE_VAO
	glGenVertexArraysOES(1, &vaoName);
	glBindVertexArrayOES(vaoName);
#else
	vaoName = 1;
#endif

	// 頂点
	vboNames[VBO_VERTEX] = buildVBO(&vertices.front(),
			vertices.size() * sizeof(float), GL_ARRAY_BUFFER);
	enableAttribute(ATTRIB_VERTEX);

	// 法線
	if (!normals.empty()) {
		vboNames[VBO_NORMAL] = buildVBO(&normals.front(),
				normals.size() * sizeof(float), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_NORMAL);
		hasNormals = true;
	}

	// テクスチャ
	if (!textureCoords.empty()) {
		vboNames[VBO_TEXCOORD] = buildVBO(&textureCoords.front(),
				textureCoords.size() * sizeof(float), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_TEXCOORD);
		hasTexture = true;
	}
	
	// マルチテクスチャ
	if (!textureCoordsMlt.empty()) {
		vboNames[VBO_TEXCOORD_MLT] = buildVBO(&textureCoordsMlt.front(),
										  textureCoordsMlt.size() * sizeof(float), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_TEXCOORD_MLT);
		hasTextureMlt = true;
	}
	
	// カラー
	if (!colors.empty()) {
		vboNames[VBO_COLOR] = buildVBO(&colors.front(),
				colors.size() * sizeof(float), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_COLOR);
		hasColor = true;
	}
	
	// ジョイント
	if (!jointData.empty()) {
		vboNames[VBO_JOINTS] = buildVBO(&jointData.front(),
				jointData.size() * sizeof(float), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_JOINTS);
		hasJoint = true;
	}

	// 頂点の番号
	if (useIndex) {
		std::vector<short> count;
		for (int i = 0; i < vertices.size() / 3; i++) {
			count.push_back(i);
		}
		vboNames[VBO_INDEX] = buildVBO(&count.front(), count.size() * sizeof(unsigned short), GL_ARRAY_BUFFER);
	}

	// インデックス
	enableAttribute(ATTRIB_INDEX);
	vboNames[VBO_ELEMENT] = buildVBO(&vertexIndexes.front(),
			vertexIndexes.size() * sizeof(unsigned short), GL_ELEMENT_ARRAY_BUFFER);


	// サイズ計算
	int cnt = vertices.size()/3;
	float maxx = -FLT_MAX, minx = FLT_MAX;
	float maxy = -FLT_MAX, miny = FLT_MAX;
	float maxz = -FLT_MAX, minz = FLT_MAX;
	for (int i = 0; i < cnt; i++) {
		int idx = i * 3;
		float x = vertices[idx];
		float y = vertices[idx+1];
		float z = vertices[idx+2];
		if (maxx < x) maxx = x;
		if (minx > x) minx = x;
		if (maxy < y) maxy = y;
		if (miny > y) miny = y;
		if (maxz < z) maxz = z;
		if (minz > z) minz = z;
	}
	size.x = maxx - minx;
	size.y = maxy - miny;
	size.z = maxz - minz;
	
//	LOGD("size:%f,%f,%f", size.x, size.y, size.z);
//	LOGD("Figure::build:end");
}

// 内部データをクリア
void Figure::clear() {
	vertices.clear();
	normals.clear();
	textureCoords.clear();
	jointData.clear();
	vertexIndexes.clear();
}

// バッファ破棄
void Figure::destroy() {
	if (!vaoName) return;
	
#ifdef USE_VAO
	GLuint index;
	GLuint bufName;
	
	//Bind the VAO so we can get data from it
	glBindVertexArrayOES(vaoName);
	
	// For every possible attribute set in the VAO
	for(index = 0; index < 16; index++)
	{
		// Get the VBO set for that attibute
		glGetVertexAttribiv(index , GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, (GLint*)&bufName);
		
		// If there was a VBO set...
		if(bufName)
		{
			//...delete the VBO
			glDeleteBuffers(1, &bufName);
		}
	}
	
	// Get any element array VBO set in the VAO
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&bufName);
	
	// If there was a element array VBO set in the VAO
	if(bufName)
	{
		//...delete the VBO
		glDeleteBuffers(1, &bufName);
	}
	
	// Finally, delete the VAO
	glDeleteVertexArraysOES(1, &vaoName);
#else
	for (int i = 0; i < NUM_VBO; i++) {
		glDeleteBuffers(1, &vboNames[i]);
		vboNames[i] = 0;
	}
#endif
	
	// 初期化
	vaoName = 0;
}

// VBO作成
GLuint Figure::buildVBO(void *data, int size, GLenum buffer) {
	GLuint buffName;
	
	// Create a vertex buffer object (VBO)
	glGenBuffers(1, &buffName);
	glBindBuffer(buffer, buffName);
	
	// Allocate and load data into the VBO
	glBufferData(buffer, size, data, GL_STATIC_DRAW);
	return buffName;
}


// Attribute設定
void Figure::enableAttribute(GLuint attrib) {
	glEnableVertexAttribArray(attrib);

	int elements = 3;
	GLenum type = GL_FLOAT;
	
	switch (attrib) {
		case ATTRIB_TEXCOORD:
		case ATTRIB_TEXCOORD_MLT:
			elements = 2;
			break;
		case ATTRIB_JOINTS:
			elements = 4;
			break;
		case ATTRIB_INDEX:
			type = GL_UNSIGNED_SHORT;
			elements = 1;
			break;
	}

	glVertexAttribPointer(attrib, elements, type, GL_FALSE, 0, 0);
}

void Figure::setAnimation(JointAnimation *animation) {
	this->animation = animation;
}

void Figure::draw(float dt) {
	if (animation) {
		animation->animationTime += dt;
		
		// アニメーション時間を超えたらリピート
		if (animation->repeat &&
			animation->animationTime > animation->getTotalAnimationTime()) {
			animation->animationTime -= animation->getTotalAnimationTime();
		}
		
		for (int i = 0; i < animation->animations.size(); i++) {
			JointKeyFrame *frame = animation->animations.at(i);
			Joint *jj = joint->findJointBySID(frame->sid);
			if (jj) {
				jj->transForm->setElements(frame->getMatrix(animation->animationTime)->matrix);
			}
		}
	}
	draw();
}
