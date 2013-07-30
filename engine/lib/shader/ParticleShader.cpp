//
//  ParticleShader.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/07/24.
//
//

#include "ParticleShader.h"
#include "Figure.h"

// uniform index
enum {
	UNIFORM_MODELVIEW_PROJECTION_MATRIX,	//!< モデルのマトリクスとカメラのマトリクスをかけた変数へのユニフォーム
	UNIFORM_TEXTURE_BASE,					//!< テクスチャへのユニフォーム
//	UNIFORM_COLOR,							//!< アルファ値へのユニフォーム
	NUM_UNIFORMS							//!< ユニフォーム数
};
static GLint uniforms[NUM_UNIFORMS];

ParticleShader::ParticleShader()
{
	gProgram = loadShader("shader/particleShader", 0);
	texname = -1;
}

ParticleShader::~ParticleShader()
{
	if (gProgram) {
		if (glIsProgram(gProgram)==GL_TRUE) {
			glDeleteProgram(gProgram);
		}
	}
}

void ParticleShader::useProgram() {
	glUseProgram(gProgram);
	this->texname = -1;
}

void ParticleShader::bindTexture(int texname) {
	if (this->texname == texname) {
		// 前回bindしたテクスチャと同じ場合には何も処理を行わない
		return;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texname);
	glUniform1i(uniforms[UNIFORM_TEXTURE_BASE], 0);
	
	this->texname = texname;
}

void ParticleShader::setMVPMatrix(Camera *camera, Matrix3D *matrix) {
	GLfloat mvp[16];
	camera->modelViewProjectionMatrix(matrix, mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, mvp);
}


void ParticleShader::bindAttribute(GLuint program, const char *name, int user) {
	glBindAttribLocation(program, PARTICLE_ATTR_VERT, "a_position");
	glBindAttribLocation(program, PARTICLE_ATTR_TEXCOORD, "a_texcoord");
	glBindAttribLocation(program, PARTICLE_ATTR_COLOR, "a_color");
}

void ParticleShader::getUniform(GLuint program, const char *name, int user) {
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_TEXTURE_BASE] = glGetUniformLocation(program, "u_diffuseTexture");
}
