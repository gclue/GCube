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

Figure::Figure() {
	vaoName = 0;
	for (int i = 0; i < NUM_VBO; i++) {
		vboNames[i] = 0;
	}
	transForm = NULL;
	//	material = NULL;
	joint = NULL;
	hasNormals = false;
	hasTexture = false;
//	hasMask = false;
//	hasLightmap = false;
	hasJoint = false;

	vertices = new std::vector<GLfloat>();
	normals = new std::vector<GLfloat>();
	textureCoords = new std::vector<GLfloat>();
//	maskCoords = new std::vector<GLfloat>();
//	lightMap = new std::vector<GLfloat>();
	vertexIndexes = new std::vector<short>();
	joint1 = new std::vector<GLushort>();
	joint2 = new std::vector<GLushort>();
	weight1 = new std::vector<GLfloat>();
	weight2 = new std::vector<GLfloat>();
	transForm = new Matrix3D();
}

Figure::~Figure() {
	for (int i = 0; i < NUM_VBO; i++) {
		glDeleteBuffers(1, &vboNames[i]);
		vboNames[i] = 0;
	}

	if (vertices) {
		delete vertices;
	}
	vertices = NULL;

	if (normals) {
		delete normals;
	}
	normals = NULL;

	if (textureCoords) {
		delete textureCoords;
	}
	textureCoords = NULL;

//	if (maskCoords) {
//		delete maskCoords;
//	}
//	maskCoords = NULL;
//
//	if (lightMap) {
//		delete lightMap;
//	}
//	lightMap = NULL;

	if (vertexIndexes) {
		delete vertexIndexes;
	}
	vertexIndexes = NULL;

	if (transForm) {
		delete transForm;
	}
	transForm = NULL;

	if (joint1) {
		delete joint1;
	}
	joint1 = NULL;

	if (joint2) {
		delete joint2;
	}
	joint2 = NULL;

	if (weight1) {
		delete weight1;
	}
	weight1 = NULL;

	if (weight2) {
		delete weight2;
	}
	weight2 = NULL;

	delete joint;
}

void Figure::addVertices(const GLfloat *v, int len) {
	for (int i = 0; i < len; i++) {
		vertices->push_back(v[i]);
	}
}

void Figure::addNormal(const GLfloat *v, int len) {
	for (int i = 0; i < len; i++) {
		normals->push_back(v[i]);
	}
}

void Figure::addTextureCoords(const GLfloat *v, int len) {
	for (int i = 0; i < len; i++) {
		textureCoords->push_back(v[i]);
	}
}

void Figure::addMaskCoords(const GLfloat *v, int len) {
//	for (int i = 0; i < len; i++) {
//		maskCoords->push_back(v[i]);
//	}
}

void Figure::addLightMap(const GLfloat *v, int len) {
//	for (int i = 0; i < len; i++) {
//		lightMap->push_back(v[i]);
//	}
}

void Figure::addVertexIndexes(const GLushort *v, int len) {
	for (int i = 0; i < len; i++) {
		vertexIndexes->push_back(v[i]);
	}
}

void Figure::addJoints(const GLushort *j1, const GLushort *j2, int len) {
	for (int i = 0; i < len; i++) {
		joint1->push_back(j1[i]);
		joint2->push_back(j2[i]);
	}
}

void Figure::addWeights(const GLfloat *w1, const GLfloat *w2, int len) {
	for (int i = 0; i < len; i++) {
		weight1->push_back(w1[i]);
		weight2->push_back(w2[i]);
	}
}


void Figure::draw() {
	// VAOをバインド
#ifdef USE_VAO
	glBindVertexArrayOES(vaoName);
#endif

	// 描画
	GLshort indexCount = vertexIndexes->size();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL);
}

void Figure::drawLines(GLfloat width) {
	// VAOをバインド
#ifdef USE_VAO
	glBindVertexArrayOES(vaoName);
#endif

	// 描画
	glLineWidth(width);
	GLshort indexCount = vertexIndexes->size();
	glDrawElements(GL_LINE_STRIP, indexCount, GL_UNSIGNED_SHORT, NULL);
}

void Figure::drawPoints() {

	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_SHORT, NULL);

}

GLuint Figure::buildVBO(void *data, int size, GLenum buffer) {
	GLuint buffName;

	// Create a vertex buffer object (VBO)
	glGenBuffers(1, &buffName);
	glBindBuffer(buffer, buffName);

	// Allocate and load data into the VBO
	glBufferData(buffer, size, data, GL_STATIC_DRAW);
	return buffName;
}

void Figure::bindVBO() {
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

//	// マスク用テクスチャ
//	if (hasMask) {
//		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_MASKCOORD]);
//		enableAttribute(ATTRIB_MASKCOORD);
//	}
//
//	// ライトマップ
//	if (hasLightmap) {
//		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_LIGHTMAP]);
//		enableAttribute(ATTRIB_LIGHTMAP);
//	}

	if (hasJoint) {
		// ジョイント
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_JOINT_1]);
		enableAttribute(ATTRIB_JOINT_1);
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_JOINT_2]);
		enableAttribute(ATTRIB_JOINT_2);
		// ウェイト
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_WEIGHT_1]);
		enableAttribute(ATTRIB_WEIGHT_1);
		glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_WEIGHT_2]);
		enableAttribute(ATTRIB_WEIGHT_2);
	}

	// インデックス
	glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_INDEX]);
	enableAttribute(ATTRIB_INDEX);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboNames[VBO_ELEMENT]);
}

void Figure::buildVAO() {
	if (vaoName) {
		return;
	}

	// Create a vertex array object (VAO)
#ifdef USE_VAO
	glGenVertexArraysOES(1, &vaoName);
	glBindVertexArrayOES(vaoName);
#else
	vaoName = 1;
#endif

	// 頂点
	vboNames[VBO_VERTEX] = buildVBO(&vertices->front(),
			vertices->size() * sizeof(GLfloat), GL_ARRAY_BUFFER);
	enableAttribute(ATTRIB_VERTEX);

	// 法線
	if (!normals->empty()) {
		vboNames[VBO_NORMAL] = buildVBO(&normals->front(),
				normals->size() * sizeof(GLfloat), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_NORMAL);
		hasNormals = true;
	}

	// テクスチャ
	if (!textureCoords->empty()) {
		vboNames[VBO_TEXCOORD] = buildVBO(&textureCoords->front(),
				textureCoords->size() * sizeof(GLfloat), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_TEXCOORD);
		hasTexture = true;
	}

//	// マスク用のテクスチャ
//	if (!maskCoords->empty()) {
//		vboNames[VBO_MASKCOORD] = buildVBO(&maskCoords->front(),
//				maskCoords->size() * sizeof(GLfloat), GL_ARRAY_BUFFER);
//		enableAttribute(ATTRIB_MASKCOORD);
//		hasMask = true;
//	}
//
//	// ライトマップ
//	if (!lightMap->empty()) {
//		vboNames[VBO_LIGHTMAP] = buildVBO(&lightMap->front(),
//				lightMap->size() * sizeof(GLfloat), GL_ARRAY_BUFFER);
//		enableAttribute(ATTRIB_LIGHTMAP);
//		hasLightmap = true;
//	}

	// ジョイント
	if (!joint1->empty()) {
		vboNames[VBO_JOINT_1] = buildVBO(&joint1->front(),
				joint1->size() * sizeof(GLushort), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_JOINT_1);
		vboNames[VBO_JOINT_2] = buildVBO(&joint2->front(),
				joint2->size() * sizeof(GLushort), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_JOINT_2);
		hasJoint = true;
	}

	// ウェイト
	if (!weight1->empty()) {
		vboNames[VBO_WEIGHT_1] = buildVBO(&weight1->front(),
				weight1->size() * sizeof(GLfloat), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_WEIGHT_1);
		vboNames[VBO_WEIGHT_2] = buildVBO(&weight2->front(),
				weight2->size() * sizeof(GLfloat), GL_ARRAY_BUFFER);
		enableAttribute(ATTRIB_WEIGHT_2);
	}

	// 頂点の番号
	std::vector<short> count;
	for (int i = 0; i < vertices->size() / 3; i++) {
		count.push_back(i);
	}
	vboNames[VBO_INDEX] = buildVBO(&count.front(), count.size() * sizeof(GLushort), GL_ARRAY_BUFFER);

	// インデックス
	enableAttribute(ATTRIB_INDEX);
	vboNames[VBO_ELEMENT] = buildVBO(&vertexIndexes->front(),
			vertexIndexes->size() * sizeof(GLushort), GL_ELEMENT_ARRAY_BUFFER);

	// データクリア
//	vertices->clear();
//	normals->clear();
//	textureCoords->clear();
//	maskCoords->clear();
//	lightMap->clear();
//	joint1->clear();
//	joint2->clear();
//	weight1->clear();
//	weight2->clear();
}

void Figure::destroyVAO() {
	for (int i = 0; i < NUM_VBO; i++) {
		glDeleteBuffers(1, &vboNames[i]);
		vboNames[i] = 0;
	}
	vaoName = 0;
}

// Attribute設定
void Figure::enableAttribute(GLuint attrib) {
	// Enable an attribute
	glEnableVertexAttribArray(attrib);

	int elements = 3;
	GLenum type = GL_FLOAT;
	switch (attrib) {
	case ATTRIB_TEXCOORD:
//	case ATTRIB_MASKCOORD:
//	case ATTRIB_LIGHTMAP:
		elements = 2;
		break;
	case ATTRIB_WEIGHT_1:
	case ATTRIB_WEIGHT_2:
		elements = 1;
		break;
	case ATTRIB_INDEX:
	case ATTRIB_JOINT_1:
	case ATTRIB_JOINT_2:
		type = GL_SHORT;
		elements = 1;
		break;
	}

	// Set up parmeters for attribute in the VAO including,
	// size, type, stride, and offset in the currenly bound VAO
	// This also attaches the position VBO to the VAO
	glVertexAttribPointer(attrib, // What attibute index will this array feed in the vertex shader (see buildProgram)
			elements, // How many elements are there per position?
			type, // What is the type of this data?
			GL_FALSE, // Do we want to normalize this data (0-1 range for fixed-pont types)
			0, // What is the stride (i.e. bytes between positions)?
			0); // What is the offset in the VBO to the position data?
}
