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

#include "BoneShader.h"
#include "Joint.h"

// uniform index
enum {
	UNIFORM_MODELVIEW_PROJECTION_MATRIX,	//!< モデルのマトリクスとカメラのマトリクスをかけた変数へのユニフォーム
	UNIFORM_MODELVIEW_MATRIX,				//!< モデルのマトリクス変数へのユニフォーム
	UNIFORM_TEXTURE_BASE,					//!< テクスチャへのユニフォーム
	UNIFORM_USE_TEXTURE,					//!< テクスチャを使用するフラグ
	UNIFORM_ALPHA,							//!< アルファ値へのユニフォーム
	UNIFORM_NORMAL_MATRIX,					//!< 法線マトリックス
	UNIFORM_USE_SKINNING,					//!< スキニングを使用するフラグ
	UNIFORM_SKINNING_MATRIX,				//!< スキニングマトリクスのユニフォーム
	UNIFORM_USE_LIGHTING,					//!< ライトを使用するフラグ
	UNIFORM_LIGHT_STATE_POS,				//!< ライトの状態（位置）
	NUM_UNIFORMS							//!< ユニフォーム数
};
static GLint uniforms[NUM_UNIFORMS];

BoneShader::BoneShader() {
	gProgram = loadShader("shader/boneShader", 0);
	baseAlpha = 1.0;
	texname = -1;
}

BoneShader::~BoneShader() {
	if (gProgram) {
		if (glIsProgram(gProgram)==GL_TRUE) {
			glDeleteProgram(gProgram);
		}
	}
}

void BoneShader::useProgram() {
	glUseProgram(gProgram);
	this->setAlpha(1.0);
	this->texname = -1;
}

void BoneShader::setLight(Light *light) {
	if (light) {
		GLfloat lightpos[3] = {light->position.x, light->position.y, light->position.z};
		glUniform3fv(uniforms[UNIFORM_LIGHT_STATE_POS], 1, lightpos);
		glUniform1i(uniforms[UNIFORM_USE_LIGHTING], 1);
	} else {
		glUniform1i(uniforms[UNIFORM_USE_LIGHTING], 0);
	}
}

void BoneShader::bindTexture(int texname) {
	if (this->texname == texname && texname>0) {
		// 前回bindしたテクスチャと同じ場合には何も処理を行わない
		return;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texname);
	glUniform1i(uniforms[UNIFORM_TEXTURE_BASE], 0);
	glUniform1f(uniforms[UNIFORM_USE_TEXTURE], texname>0?0:1.0);

	this->texname = texname;
}

void BoneShader::setMVPMatrix(Camera *camera, Matrix3D *matrix) {
	GLfloat mvp[16];
	camera->modelViewProjectionMatrix(matrix, mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, mvp);
}

void BoneShader::setSkinningMatrix(Matrix3D **matrix, int len) {
	if (matrix) {
		GLfloat mtx[16*len];
		for (int i = 0; i < len; ++i) {
			matrix[i]->getElements(&mtx[16*i]);
		}
		glUniform1i(uniforms[UNIFORM_USE_SKINNING], 1);
		glUniformMatrix4fv(uniforms[UNIFORM_SKINNING_MATRIX], len, GL_FALSE, mtx);
	} else {
		glUniform1i(uniforms[UNIFORM_USE_SKINNING], 0);
	}
}

void BoneShader::setSkinningMatrix(Figure *fig) {
	if (fig->joint && fig->animation) {
		std::vector<Matrix3D*> visitor;
		int count = fig->joint->applyMatrix(NULL, &visitor);
		this->setSkinningMatrix(&visitor[0], count);
	} else {
		this->setSkinningMatrix(NULL, 0);
	}
}

void BoneShader::setNormalMatrix(Matrix3D *matrix) {
	GLfloat normalMtx[9];
	matrix->normalMatrix(normalMtx);
	glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, GL_FALSE, normalMtx);
}

void BoneShader::bindAttribute(GLuint program, const char *name, int user) {
	glBindAttribLocation(program, ATTRIB_VERTEX, "a_position");
	glBindAttribLocation(program, ATTRIB_TEXCOORD, "a_texcoord");
	glBindAttribLocation(program, ATTRIB_NORMAL, "a_normal");
	glBindAttribLocation(program, ATTRIB_JOINTS, "a_joints");
}

void BoneShader::getUniform(GLuint program, const char *name, int user) {
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_MODELVIEW_MATRIX] = glGetUniformLocation(program, "u_mvMatrix");
	uniforms[UNIFORM_USE_TEXTURE] = glGetUniformLocation(program, "u_useTexture");
	uniforms[UNIFORM_TEXTURE_BASE] = glGetUniformLocation(program, "u_diffuseTexture");
	uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(program, "u_nMatrix");
	uniforms[UNIFORM_SKINNING_MATRIX] = glGetUniformLocation(program, "u_skinningMatrix");
	uniforms[UNIFORM_USE_SKINNING] = glGetUniformLocation(program, "u_useSkinning");
	uniforms[UNIFORM_ALPHA] = glGetUniformLocation(program, "u_alpha");
	uniforms[UNIFORM_USE_LIGHTING] = glGetUniformLocation(program, "u_useLighting");
	uniforms[UNIFORM_LIGHT_STATE_POS] = glGetUniformLocation(program, "u_lightState.position");
}

void BoneShader::setAlpha(float a) {
	glUniform1f(uniforms[UNIFORM_ALPHA], a * baseAlpha);
}

void BoneShader::setBaseAlpha(float baseAlpha) {
	this->baseAlpha = baseAlpha;
}

