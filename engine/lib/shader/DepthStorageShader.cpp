//
//  DepthStorageShader.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/08/09.
//
//

#include "DepthStorageShader.h"
#include "Joint.h"

static const char vsh[] = ""
"attribute vec3 a_position;"
"attribute vec4 a_joints;"
""
"uniform mat4 u_mvpMatrix;"
"uniform mat4 u_skinningMatrix[30];"
"uniform bool u_useSkinning;"
""
"varying vec4 v_position;"
""
"void main(void) {"
"	mat4 skmtx = mat4(1);"
"	if (u_useSkinning) {"
"		mat4 m1 = u_skinningMatrix[int(a_joints[0])] * a_joints[1];"
"		mat4 m2 = u_skinningMatrix[int(a_joints[2])] * a_joints[3];"
"		skmtx = m1 + m2;"
"		vec4 p1 = m1 * vec4(a_position, 1.0);"
"		vec4 p2 = m2 * vec4(a_position, 1.0);"
"		v_position = u_mvpMatrix * (p1 + p2);"
"	} else {"
"		v_position = u_mvpMatrix * vec4(a_position, 1.0);"
"	}"
""
"    gl_Position = v_position;"
"}";

static const char vfsh[] = ""
"precision mediump float;"
""
"uniform bool u_depthBuffer;"
""
"varying vec4 v_position;"
""
"vec4 convRGBA(float depth) {"
"	float r = depth;"
"	float g = fract(r * 255.0);"
"	float b = fract(g * 255.0);"
"	float a = fract(b * 255.0);"
"	float coef = 1.0 / 255.0;"
"	r -= g * coef;"
"	g -= b * coef;"
"	b -= a * coef;"
"	return vec4(r, g, b, a);"
"}"
""
"void main(void) {"
"    vec4 convColor = vec4(1.0, 1.0, 1.0, 1.0);"
"    if (u_depthBuffer) {"
"    } else {"
"        float near = 0.1;"
"        float far  = 150.0;"
"        float linerDepth = 1.0 / (far - near);"
"        linerDepth *= length(v_position);"
"        convColor = convRGBA(linerDepth);"
"    }"
"    gl_FragColor = convColor;"
"}";


// uniform index
enum {
	UNIFORM_MODELVIEW_PROJECTION_MATRIX,	//!< モデルのマトリクスとカメラのマトリクスをかけた変数へのユニフォーム
	UNIFORM_DEPTH_BUFFER,					//!< depthBufferフラグ
	UNIFORM_USE_SKINNING,					//!< スキニングを使用するフラグ
	UNIFORM_SKINNING_MATRIX,				//!< スキニングマトリクスのユニフォーム
	NUM_UNIFORMS							//!< ユニフォーム数
};
static GLint uniforms[NUM_UNIFORMS];

DepthStorageShader::DepthStorageShader() {
	gProgram = loadShader(vsh, vfsh, 0);
}

DepthStorageShader::~DepthStorageShader() {
	if (gProgram) {
		if (glIsProgram(gProgram)==GL_TRUE) {
			glDeleteProgram(gProgram);
		}
	}
}

void DepthStorageShader::useProgram() {
	glUseProgram(gProgram);
}

void DepthStorageShader::setMVPMatrix(Camera *camera, Matrix3D *matrix) {
	GLfloat mvp[16];
	camera->modelViewProjectionMatrix(matrix, mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, mvp);
}

void DepthStorageShader::setDepthBuffer(bool flag) {
	glUniform1i(uniforms[UNIFORM_DEPTH_BUFFER], flag ? GL_TRUE : GL_FALSE);
}

void DepthStorageShader::setSkinningMatrix(Matrix3D **matrix, int len) {
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

void DepthStorageShader::setSkinningMatrix(Figure *fig) {
	if (fig->joint && fig->animation) {
		std::vector<Matrix3D*> visitor;
		int count = fig->joint->applyMatrix(NULL, &visitor);
		this->setSkinningMatrix(&visitor[0], count);
	} else {
		this->setSkinningMatrix(NULL, 0);
	}
}

void DepthStorageShader::bindAttribute(GLuint program, const char *name, int user) {
	glBindAttribLocation(program, ATTRIB_VERTEX, "a_position");
	glBindAttribLocation(program, ATTRIB_JOINTS, "a_joints");
}

void DepthStorageShader::getUniform(GLuint program, const char *name, int user) {
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_DEPTH_BUFFER] = glGetUniformLocation(program, "u_depthBuffer");
	uniforms[UNIFORM_USE_SKINNING] = glGetUniformLocation(program, "u_useSkinning");
	uniforms[UNIFORM_SKINNING_MATRIX] = glGetUniformLocation(program, "u_skinningMatrix");
}
