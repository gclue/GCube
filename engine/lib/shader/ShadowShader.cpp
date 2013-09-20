//
//  ShadowShader.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/08/09.
//
//

#include "ShadowShader.h"
#include "Joint.h"

static const char shadow_vsh[] = ""
"attribute vec3 a_position;"
"attribute vec3 a_normal;"
"attribute vec2 a_texcoord;"
"attribute vec4 a_joints;"
""
"uniform mat4 u_mMatrix;"
"uniform mat4 u_mvpMatrix;"
"uniform mat4 u_lgtMatrix;"
"uniform mat4 u_tMatrix;"
"uniform mat4 u_skinningMatrix[50];"
"uniform bool u_useSkinning;"
"uniform bool u_edge;"
"uniform float u_edgeSize;"
""
"varying vec3 v_position;"
"varying vec3 v_normal;"
"varying vec2 v_texcoord;"
"varying vec4 v_shadow_texcoord;"
"varying vec4 v_depth;"
""
"const mat4 bias = mat4("
"					   0.5, 0.0, 0.0, 0.0,"
"					   0.0, 0.5, 0.0, 0.0,"
"					   0.0, 0.0, 0.5, 0.0,"
"					   0.5, 0.5, 0.5, 1.0"
"					   );"
""
"void main(void) {"
"	vec3 pos = a_position;"
"	if (u_edge) {"
"		pos += a_normal * u_edgeSize;"
"	}"
""
"	mat4 skmtx = mat4(1);"
"	if (u_useSkinning && a_joints[0] < 65535.0) {"
"		mat4 m1 = u_skinningMatrix[int(a_joints[0])] * a_joints[1];"
"		mat4 m2 = u_skinningMatrix[int(a_joints[2])] * a_joints[3];"
"		skmtx = m1 + m2;"
"		vec4 p1 = m1 * vec4(pos, 1.0);"
"		vec4 p2 = m2 * vec4(pos, 1.0);"
"		gl_Position = u_mvpMatrix * (p1 + p2);"
"	} else {"
"		gl_Position = u_mvpMatrix * vec4(pos, 1.0);"
"	}"
""
"    v_position  = (u_mMatrix * vec4(a_position, 1.0)).xyz;"
"    v_normal    = a_normal;"
"    v_texcoord  = a_texcoord;"
"    v_shadow_texcoord  = bias * u_tMatrix * vec4(v_position, 1.0);"
"    v_depth     = u_lgtMatrix * vec4(a_position, 1.0);"
"}";


static const char shadow_vfsh[] = ""
"precision mediump float;"
""
"uniform mat4      u_invMatrix;"
"uniform vec3      u_lightPosition;"
"uniform sampler2D u_texture;"
"uniform sampler2D u_shadow_texture;"
"uniform bool      u_depthBuffer;"
"uniform bool      u_useShadowFlag;"
"uniform vec4      u_edgeColor;"
""
"varying vec3      v_position;"
"varying vec3      v_normal;"
"varying vec2      v_texcoord;"
"varying vec4      v_shadow_texcoord;"
"varying vec4      v_depth;"
""
"float restDepth(vec4 RGBA) {"
"    const float rMask = 1.0;"
"    const float gMask = 1.0 / 255.0;"
"    const float bMask = 1.0 / (255.0 * 255.0);"
"    const float aMask = 1.0 / (255.0 * 255.0 * 255.0);"
"    float depth = dot(RGBA, vec4(rMask, gMask, bMask, aMask));"
"    return depth;"
"}"
""
"void main(void) {"
"	vec3  light = u_lightPosition - v_position;"
"	vec3  invLight = normalize(u_invMatrix * vec4(light, 0.0)).xyz;"
"	float diffuse = clamp(dot(v_normal, invLight), 0.2, 1.0);"
"	vec4 depthColor = vec4(1.0);"
"	if (v_depth.w > 0.0) {"
"       vec4 tx = texture2DProj(u_shadow_texture, v_shadow_texcoord);"
//"       if (tx.r < 1.0 && tx.g < 1.0 && tx.b < 1.0 && tx.a < 1.0) {"
"			float shadow = restDepth(tx);"
"			if (u_depthBuffer) {"
"				vec4 lightCoord = v_depth / v_depth.w;"
"				if (lightCoord.z - 0.0001 > shadow) {"
"					depthColor  = vec4(0.5, 0.5, 0.5, 1.0);"
"				}"
"			} else {"
"				float near = 0.1;"
"				float far  = 300.0;"
"				float linerDepth = 1.0 / (far - near);"
"				linerDepth *= length(v_depth);"
"				if (linerDepth - 0.004 > shadow) {"
"					depthColor = vec4(linerDepth, linerDepth, linerDepth, 1.0);"
"				}"
"			}"
//"		}"
"	}"
""
"	if (u_edgeColor.a > 0.0) {"
"		gl_FragColor = vec4(vec3(u_edgeColor), 1.0);"
"	} else {"
//"		gl_FragColor = texture2D(u_texture, v_texcoord.st) * vec4(vec3(diffuse), 1.0) * depthColor;"
"		gl_FragColor = texture2D(u_texture, v_texcoord.st) * depthColor;"
"	}"
"}";


// uniform index
enum {
	UNIFORM_MODELVIEW_PROJECTION_MATRIX,	//!< モデルのマトリクスとカメラのマトリクスをかけた変数へのユニフォーム
	UNIFORM_MODELVIEW_MATRIX,				//!< モデルのマトリクス
	UNIFORM_LIGHT_PROJECTION_MATRIX,		//!< ライトから見たプロジェクションマッピング
	UNIFORM_INVERSE_MODEL_MATRIX,			//!< モデルのマトリクスの逆行列
	UNIFORM_TEXTURE_MATRIX,					//!< 影テクスチャのマトリクス
	UNIFORM_LIGHT_POSITION,					//!< ライトの位置
	UNIFORM_TEXTURE,						//!< テクスチャ
	UNIFORM_SHADOW_TEXTURE,					//!< 影のテクスチャ
	UNIFORM_DEPTH_BUFFER,					//!< depthBufferフラグ
	UNIFORM_USE_SKINNING,					//!< スキニングを使用するフラグ
	UNIFORM_SKINNING_MATRIX,				//!< スキニングマトリクスのユニフォーム
	UNIFORM_USE_EDGE,						//!< エッジを使用するフラグ
	UNIFORM_EDGE_COLOR,						//!< エッジの色
	UNIFORM_EDGE_SIZE,						//!< エッジの色
	UNIFORM_USE_SHADOW,						//!< 影を使用するフラグ
	NUM_UNIFORMS							//!< ユニフォーム数
};
static GLint uniforms[NUM_UNIFORMS];

ShadowShader::ShadowShader() {
	gProgram = loadShader(shadow_vsh, shadow_vfsh, 0);
}

ShadowShader::~ShadowShader() {
	if (gProgram) {
		if (glIsProgram(gProgram)==GL_TRUE) {
			glDeleteProgram(gProgram);
		}
	}
}
void ShadowShader::useProgram() {
	glUseProgram(gProgram);
}

void ShadowShader::bindTexture(int texname) {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texname);
	glUniform1i(uniforms[UNIFORM_TEXTURE], 1);
}

void ShadowShader::bindShadowTexture(int texname) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texname);
	glUniform1i(uniforms[UNIFORM_SHADOW_TEXTURE], 0);
}

void ShadowShader::setDepthBuffer(bool flag) {
	glUniform1i(uniforms[UNIFORM_DEPTH_BUFFER], flag ? GL_TRUE : GL_FALSE);
}

void ShadowShader::setMVPMatrix(Camera *camera, Matrix3D *matrix) {
	GLfloat mvp[16];
	camera->modelViewProjectionMatrix(matrix, mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, mvp);
}

void ShadowShader::setModelMatrix(Matrix3D *matrix) {
	GLfloat mvp[16];
	matrix->getElements(mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_MATRIX], 1, GL_FALSE, mvp);
}

void ShadowShader::setTextureMatrix(Camera *camera) {
	// TODO: バイアス行列のかけ算をシェーダではなく、ここで行った方が速いかもしれない。
	GLfloat mvp[16];
	camera->modelViewProjectionMatrix(mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_TEXTURE_MATRIX], 1, GL_FALSE, mvp);
}

void ShadowShader::setLightMatrix(Camera *camera, Matrix3D *matrix) {
	GLfloat mvp[16];
	camera->modelViewProjectionMatrix(matrix, mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_LIGHT_PROJECTION_MATRIX], 1, GL_FALSE, mvp);
}

void ShadowShader::setInverseMatrix(Matrix3D *matrix) {
	GLfloat mvp[16];
//	matrix->getInvertElements(mvp);
	matrix->getElements(mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_INVERSE_MODEL_MATRIX], 1, GL_FALSE, mvp);
}

void ShadowShader::setLightPosition(Light *light) {
	GLfloat lightPosition[] = {
		light->getX(), light->getY(), light->getZ()
	};
	glUniform3fv(uniforms[UNIFORM_LIGHT_POSITION], 1, lightPosition);
}

void ShadowShader::setSkinningMatrix(Matrix3D **matrix, int len) {
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

void ShadowShader::setSkinningMatrix(Figure *fig) {
	if (fig->joint && fig->animation) {
		std::vector<Matrix3D*> visitor;
		int count = fig->joint->applyMatrix(NULL, &visitor);
		this->setSkinningMatrix(&visitor[0], count);
	} else {
		this->setSkinningMatrix(NULL, 0);
	}
}

void ShadowShader::setUseShadow(bool flag) {
	glUniform1i(uniforms[UNIFORM_USE_SHADOW], flag ? GL_TRUE : GL_FALSE);
}

void ShadowShader::setEdgeColor(float r, float g, float b, float a) {
	GLfloat color[4] = {r, g, b, a};
	glUniform4fv(uniforms[UNIFORM_EDGE_COLOR], 1, color);
}

void ShadowShader::setUseEdge(bool use) {
	glUniform1i(uniforms[UNIFORM_USE_EDGE], use ? 1 : 0);
}

void ShadowShader::setEdgeSize(float size) {
	glUniform1f(uniforms[UNIFORM_EDGE_SIZE], size);
}

void ShadowShader::bindAttribute(GLuint program, const char *name, int user) {
	glBindAttribLocation(program, ATTRIB_VERTEX, "a_position");
	glBindAttribLocation(program, ATTRIB_NORMAL, "a_normal");
	glBindAttribLocation(program, ATTRIB_TEXCOORD, "a_texcoord");
	glBindAttribLocation(program, ATTRIB_JOINTS, "a_joints");
}

void ShadowShader::getUniform(GLuint program, const char *name, int user) {
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_LIGHT_PROJECTION_MATRIX] = glGetUniformLocation(program, "u_lgtMatrix");
	uniforms[UNIFORM_INVERSE_MODEL_MATRIX] = glGetUniformLocation(program, "u_invMatrix");
	uniforms[UNIFORM_LIGHT_POSITION] = glGetUniformLocation(program, "u_lightPosition");
	uniforms[UNIFORM_TEXTURE] = glGetUniformLocation(program, "u_texture");
	uniforms[UNIFORM_SHADOW_TEXTURE] = glGetUniformLocation(program, "u_shadow_texture");
	uniforms[UNIFORM_DEPTH_BUFFER] = glGetUniformLocation(program, "u_depthBuffer");
	uniforms[UNIFORM_MODELVIEW_MATRIX] = glGetUniformLocation(program, "u_mMatrix");
	uniforms[UNIFORM_TEXTURE_MATRIX] = glGetUniformLocation(program, "u_tMatrix");
	uniforms[UNIFORM_USE_SKINNING] = glGetUniformLocation(program, "u_useSkinning");
	uniforms[UNIFORM_SKINNING_MATRIX] = glGetUniformLocation(program, "u_skinningMatrix");
	uniforms[UNIFORM_USE_SHADOW] = glGetUniformLocation(program, "u_useShadowFlag");	
	uniforms[UNIFORM_USE_EDGE] = glGetUniformLocation(program, "u_edge");
	uniforms[UNIFORM_EDGE_COLOR] = glGetUniformLocation(program, "u_edgeColor");
	uniforms[UNIFORM_EDGE_SIZE] = glGetUniformLocation(program, "u_edgeSize");
}
