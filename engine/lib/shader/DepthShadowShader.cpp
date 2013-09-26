//
//  DepthShadowShader.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/08/09.
//
//

#include "DepthShadowShader.h"
#include "Joint.h"
#include "matrixUtil.h"


#define STRINGIFY(x) #x

/**
 * バーテックスシェーダ.
 */
static const char shadow_vsh[] = STRINGIFY(
	attribute vec3 a_position;
	attribute vec3 a_normal;
	attribute vec2 a_texcoord;
	attribute vec2 a_texcoord_mlt;
	attribute vec4 a_joints;
	attribute vec4 a_color;
										   
	uniform mat4 u_mMatrix;
	uniform mat4 u_mvpMatrix;
	uniform mat4 u_lgtMatrix;
	uniform mat4 u_tMatrix;
	uniform mat4 u_skinningMatrix[50];
	uniform bool u_useSkinning;
	uniform bool u_edge;
	uniform float u_edgeSize;
										   
	varying vec3 v_position;
	varying vec3 v_normal;
	varying vec2 v_texcoord;
	varying vec2 v_texcoord_mlt;
	varying vec4 v_shadow_texcoord;
	varying vec4 v_depth;
										   
	void main(void) {
		vec3 pos = a_position;
		if (u_edge) {
			pos += a_normal * u_edgeSize;
		}
		
		vec4 apos;
		if (u_useSkinning && a_joints[0] < 65535.0) {
			mat4 m1 = u_skinningMatrix[int(a_joints[0])] * a_joints[1];
			mat4 m2 = u_skinningMatrix[int(a_joints[2])] * a_joints[3];
			vec4 p1 = m1 * vec4(pos, 1.0);
			vec4 p2 = m2 * vec4(pos, 1.0);
			apos = p1 + p2;
			gl_Position = u_mvpMatrix * apos;
		} else {
			apos = vec4(pos, 1.0);
			gl_Position = u_mvpMatrix * apos;
		}
		
		v_position  = (u_mMatrix * apos).xyz;
		v_normal    = a_normal;
		v_texcoord  = a_texcoord;
		v_texcoord_mlt  = a_texcoord_mlt;
		v_shadow_texcoord  = u_tMatrix * vec4(v_position, 1.0);
		v_depth     = u_lgtMatrix * apos;
	}
);

/**
 * フラグメントシェーダ.
 */
static const char shadow_vfsh[] = STRINGIFY(
	precision mediump float;
 
	uniform mat4      u_invMatrix;
	uniform vec3      u_lightPosition;
	uniform sampler2D u_texture;
	uniform sampler2D u_texture_mlt;
	uniform sampler2D u_shadow_texture;
	uniform bool      u_useShadowFlag;
	uniform bool      u_useColorFlag;
	uniform bool      u_useMultiFlag;
	uniform vec4      u_edgeColor;
	uniform vec4      u_color;
	uniform float     u_alpha;
	uniform float     u_near;
	uniform float     u_far;
	
	varying vec3      v_position;
	varying vec3      v_normal;
	varying vec2      v_texcoord;
	varying vec2      v_texcoord_mlt;
	varying vec4      v_shadow_texcoord;
	varying vec4      v_depth;
	
	float restDepth(vec4 RGBA) {
		const float rMask = 1.0;
		const float gMask = 1.0 / 255.0;
		const float bMask = 1.0 / (255.0 * 255.0);
		const float aMask = 1.0 / (255.0 * 255.0 * 255.0);
		float depth = dot(RGBA, vec4(rMask, gMask, bMask, aMask));
		return depth;
	}
	
	void main(void) {
		if (u_edgeColor.a > 0.0) {
			gl_FragColor = vec4(vec3(u_edgeColor), 1.0) * vec4(vec3(1.0), u_alpha);
		} else {
			vec4 color;
			if (u_useColorFlag) {
				color = u_color * vec4(vec3(1.0), u_alpha);
			} else {
				color = texture2D(u_texture, v_texcoord.st) * vec4(vec3(1.0), u_alpha);
			}
			
			if (u_useShadowFlag && v_depth.w > 0.0) {
				vec4 tx = texture2DProj(u_shadow_texture, v_shadow_texcoord);
				float shadow = restDepth(tx);
				float near = u_near;
				float far  = u_far;
				float linerDepth = 1.0 / (far - near);
				linerDepth *= length(v_depth);
				if (linerDepth - 0.001 > shadow) {
					color *= vec4(0.7, 0.7, 0.7, 1.0);
				}
			}
			
			if (u_useMultiFlag) {
				color *= vec4(texture2D(u_texture_mlt, v_texcoord_mlt.st));
			}
			
//			vec3  light = u_lightPosition - v_position;
//			vec3  invLight = normalize(u_invMatrix * vec4(light, 0.0)).xyz;
//			float diffuse = clamp(dot(v_normal, invLight), 0.8, 1.0);
//			color *= vec4(vec3(diffuse), 1.0);
			
			gl_FragColor = color;
		}
	}
);

//"		gl_FragColor = texture2D(u_texture, v_texcoord.st) * vec4(vec3(diffuse), 1.0) * depthColor;"

// uniform index
enum {
	UNIFORM_MODELVIEW_PROJECTION_MATRIX,	//!< モデルのマトリクスとカメラのマトリクスをかけた変数へのユニフォーム
	UNIFORM_MODELVIEW_MATRIX,				//!< モデルのマトリクス
	UNIFORM_LIGHT_PROJECTION_MATRIX,		//!< ライトから見たプロジェクションマッピング
	UNIFORM_INVERSE_MODEL_MATRIX,			//!< モデルのマトリクスの逆行列
	UNIFORM_TEXTURE_MATRIX,					//!< 影テクスチャのマトリクス
	UNIFORM_LIGHT_POSITION,					//!< ライトの位置
	UNIFORM_TEXTURE,						//!< テクスチャ
	UNIFORM_TEXTURE_MLT,					//!< マルチテクスチャ
	UNIFORM_SHADOW_TEXTURE,					//!< 影のテクスチャ
	UNIFORM_USE_SKINNING,					//!< スキニングを使用するフラグ
	UNIFORM_SKINNING_MATRIX,				//!< スキニングマトリクスのユニフォーム
	UNIFORM_USE_EDGE,						//!< エッジを使用するフラグ
	UNIFORM_EDGE_COLOR,						//!< エッジの色
	UNIFORM_EDGE_SIZE,						//!< エッジの色
	UNIFORM_USE_SHADOW,						//!< 影を使用するフラグ
	UNIFORM_ALPHA,							//!< アルファ値へのユニフォーム
	UNIFORM_USE_COLOR,						//!< カラー使用フラグへのユニフォーム
	UNIFORM_COLOR,							//!< カラー値へのユニフォーム
	UNIFORM_USE_MULTI_TEXTURE,				//!< マルチテクスチャフラグのユニフォーム
	UNIFORM_NEAR,							//!< ニアの位置
	UNIFORM_FAR,							//!< ファーの位置
	NUM_UNIFORMS							//!< ユニフォーム数
};
static GLint uniforms[NUM_UNIFORMS];

DepthShadowShader::DepthShadowShader() {
	gProgram = loadShader(shadow_vsh, shadow_vfsh, 0);
}

DepthShadowShader::~DepthShadowShader() {
	if (gProgram) {
		if (glIsProgram(gProgram)==GL_TRUE) {
			glDeleteProgram(gProgram);
		}
	}
}
void DepthShadowShader::useProgram() {
	glUseProgram(gProgram);
	setAlpha(1.0);
}

void DepthShadowShader::bindTexture(int texname) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texname);
	glUniform1i(uniforms[UNIFORM_TEXTURE], 0);
}

void DepthShadowShader::bindTextureMlt(int texname) {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texname);
	glUniform1i(uniforms[UNIFORM_TEXTURE_MLT], 2);
	glUniform1i(uniforms[UNIFORM_USE_MULTI_TEXTURE], texname>0 ? 1 : 0);
}

void DepthShadowShader::bindShadowTexture(int texname) {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texname);
	glUniform1i(uniforms[UNIFORM_SHADOW_TEXTURE], 1);
}

void DepthShadowShader::setMVPMatrix(Camera *camera, Matrix3D *matrix) {
	GLfloat mvp[16];
	camera->modelViewProjectionMatrix(matrix, mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, mvp);
}

void DepthShadowShader::setModelMatrix(Matrix3D *matrix) {
	GLfloat mvp[16];
	matrix->getElements(mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_MATRIX], 1, GL_FALSE, mvp);
}

void DepthShadowShader::setTextureMatrix(Camera *camera) {
	GLfloat mvp[16];
	GLfloat bias[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};
	camera->modelViewProjectionMatrix(mvp);
	mtxMultiply(mvp, bias, mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_TEXTURE_MATRIX], 1, GL_FALSE, mvp);
}

void DepthShadowShader::setLightMatrix(Camera *camera, Matrix3D *matrix) {
	GLfloat mvp[16];
	camera->modelViewProjectionMatrix(matrix, mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_LIGHT_PROJECTION_MATRIX], 1, GL_FALSE, mvp);
}

void DepthShadowShader::setInverseMatrix(Matrix3D *matrix) {
	GLfloat mvp[16];
	//	matrix->getInvertElements(mvp);
	matrix->getElements(mvp);
	glUniformMatrix4fv(uniforms[UNIFORM_INVERSE_MODEL_MATRIX], 1, GL_FALSE, mvp);
}

void DepthShadowShader::setLightPosition(Light *light) {
	GLfloat lightPosition[] = {
		light->getX(), light->getY(), light->getZ()
	};
	glUniform3fv(uniforms[UNIFORM_LIGHT_POSITION], 1, lightPosition);
}

void DepthShadowShader::setNear(float near) {
	glUniform1f(uniforms[UNIFORM_NEAR], near);
}

void DepthShadowShader::setFar(float far) {
	glUniform1f(uniforms[UNIFORM_FAR], far);
}

void DepthShadowShader::setSkinningMatrix(Matrix3D **matrix, int len) {
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

void DepthShadowShader::setSkinningMatrix(Figure *fig) {
	if (fig->joint && fig->animation) {
		std::vector<Matrix3D*> visitor;
		int count = fig->joint->applyMatrix(NULL, &visitor);
		this->setSkinningMatrix(&visitor[0], count);
	} else {
		this->setSkinningMatrix(NULL, 0);
	}
}

void DepthShadowShader::setUseShadow(bool flag) {
	glUniform1i(uniforms[UNIFORM_USE_SHADOW], flag ? 1 : 0);
}

void DepthShadowShader::setEdgeColor(float r, float g, float b, float a) {
	GLfloat color[4] = {r, g, b, a};
	glUniform4fv(uniforms[UNIFORM_EDGE_COLOR], 1, color);
}

void DepthShadowShader::setUseEdge(bool use) {
	glUniform1i(uniforms[UNIFORM_USE_EDGE], use ? 1 : 0);
}

void DepthShadowShader::setEdgeSize(float size) {
	glUniform1f(uniforms[UNIFORM_EDGE_SIZE], size);
}

void DepthShadowShader::setAlpha(float alpha) {
	glUniform1f(uniforms[UNIFORM_ALPHA], alpha);
}

void DepthShadowShader::setColor(float r, float g, float b, float a) {
	GLfloat color[4] = {r, g, b, a};
	glUniform4fv(uniforms[UNIFORM_COLOR], 1, color);
}

void DepthShadowShader::setUseColor(bool flag) {
	glUniform1i(uniforms[UNIFORM_USE_COLOR], flag ? 1 : 0);
}

void DepthShadowShader::bindAttribute(GLuint program, const char *name, int user) {
	glBindAttribLocation(program, ATTRIB_VERTEX, "a_position");
	glBindAttribLocation(program, ATTRIB_NORMAL, "a_normal");
	glBindAttribLocation(program, ATTRIB_TEXCOORD, "a_texcoord");
	glBindAttribLocation(program, ATTRIB_TEXCOORD_MLT, "a_texcoord_mlt");
	glBindAttribLocation(program, ATTRIB_JOINTS, "a_joints");
}

void DepthShadowShader::getUniform(GLuint program, const char *name, int user) {
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_LIGHT_PROJECTION_MATRIX] = glGetUniformLocation(program, "u_lgtMatrix");
	uniforms[UNIFORM_INVERSE_MODEL_MATRIX] = glGetUniformLocation(program, "u_invMatrix");
	uniforms[UNIFORM_LIGHT_POSITION] = glGetUniformLocation(program, "u_lightPosition");
	uniforms[UNIFORM_TEXTURE] = glGetUniformLocation(program, "u_texture");
	uniforms[UNIFORM_SHADOW_TEXTURE] = glGetUniformLocation(program, "u_shadow_texture");
	uniforms[UNIFORM_MODELVIEW_MATRIX] = glGetUniformLocation(program, "u_mMatrix");
	uniforms[UNIFORM_TEXTURE_MATRIX] = glGetUniformLocation(program, "u_tMatrix");
	uniforms[UNIFORM_USE_SKINNING] = glGetUniformLocation(program, "u_useSkinning");
	uniforms[UNIFORM_SKINNING_MATRIX] = glGetUniformLocation(program, "u_skinningMatrix");
	uniforms[UNIFORM_USE_SHADOW] = glGetUniformLocation(program, "u_useShadowFlag");	
	uniforms[UNIFORM_USE_EDGE] = glGetUniformLocation(program, "u_edge");
	uniforms[UNIFORM_EDGE_COLOR] = glGetUniformLocation(program, "u_edgeColor");
	uniforms[UNIFORM_EDGE_SIZE] = glGetUniformLocation(program, "u_edgeSize");
	uniforms[UNIFORM_ALPHA] = glGetUniformLocation(program, "u_alpha");
	uniforms[UNIFORM_USE_COLOR] = glGetUniformLocation(program, "u_useColorFlag");
	uniforms[UNIFORM_COLOR] = glGetUniformLocation(program, "u_color");
	uniforms[UNIFORM_TEXTURE_MLT] = glGetUniformLocation(program, "u_texture_mlt");
	uniforms[UNIFORM_USE_MULTI_TEXTURE] = glGetUniformLocation(program, "u_useMultiFlag");
	uniforms[UNIFORM_NEAR] = glGetUniformLocation(program, "u_near");
	uniforms[UNIFORM_FAR] = glGetUniformLocation(program, "u_far");
}
