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
 * PointSpriteShader.cpp
 *
 *  Created on: 2011/08/03
 *      Author: GClue, Inc.
 */
#include "PointSpriteShader.h"

// uniform index
enum {
	UNIFORM_MODELVIEW_PROJECTION_MATRIX,
	UNIFORM_TEXTURE_BASE,
	UNIFORM_ALPHA,
	UNIFORM_SIZE,
	UNIFORM_BRIGHT,
	NUM_UNIFORMS
};
static GLint uniforms[NUM_UNIFORMS];

PointSpriteShader::PointSpriteShader() {
	gProgram = loadShader("shader/pointShader", 0);
	baseAlpha = 1.0;
	texname = -1;
}

PointSpriteShader::~PointSpriteShader() {
	if (gProgram) {
		glDeleteProgram(gProgram);
	}
}

void PointSpriteShader::useProgram() {
	glUseProgram(gProgram);
	this->setAlpha(1.0);
	this->texname = -1;
}

void PointSpriteShader::bindTexture(int texname) {
	if (this->texname == texname) {
		// 前回bindしたテクスチャと同じ場合には何も処理を行わない
		return;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texname);
	glUniform1i(uniforms[UNIFORM_TEXTURE_BASE], 0);

	this->texname = texname;
}

void PointSpriteShader::setAlpha(float a) {
	glUniform1f(uniforms[UNIFORM_ALPHA], a * baseAlpha);
	glUniform1f(uniforms[UNIFORM_BRIGHT], a);
}

void PointSpriteShader::setBright(float b) {
	glUniform1f(uniforms[UNIFORM_BRIGHT], b);
}

void PointSpriteShader::setPointSize(float s) {
	glUniform1f(uniforms[UNIFORM_SIZE], s);
}

void PointSpriteShader::setMVPMatrix(Camera *camera, Matrix3D *matrix) {
	GLfloat mvp[16];
	camera->modelViewProjectionMatrix(matrix, mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, mvp);
}

void PointSpriteShader::bindAttribute(GLuint program, const char *name, int user) {
	glBindAttribLocation(program, ATTRIB_VERTEX, "a_position");
}

void PointSpriteShader::getUniform(GLuint program, const char *name, int user) {
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_TEXTURE_BASE] = glGetUniformLocation(program, "u_diffuseTexture");
	uniforms[UNIFORM_ALPHA] = glGetUniformLocation(program, "u_alpha");
	uniforms[UNIFORM_BRIGHT] = glGetUniformLocation(program, "u_bright");
	uniforms[UNIFORM_SIZE] = glGetUniformLocation(program, "u_size");
}

float PointSpriteShader::getBaseAlpha() const {
	return baseAlpha;
}

void PointSpriteShader::setBaseAlpha(float baseAlpha) {
	this->baseAlpha = baseAlpha;
}

