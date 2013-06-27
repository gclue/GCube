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

#include "Shader.h"
#include "AssetManager.h"
#include <stdio.h>
#include <stdlib.h>

static GLuint static_program = -1;

Shader::Shader() {
}

Shader::~Shader() {
}

GLuint Shader::loadShader(const char* vertexShader, const char* fragmentShader, int user) {
	GLuint vertShader = NULL;
	GLuint fragShader = NULL;
	
	// 頂点(Vertex)シェーダ
	vertShader = compileShader(GL_VERTEX_SHADER, vertexShader);
	if (!vertShader) {
		LOGE("Failed to compile vertex shader");
		goto ERROR;
	}
	
	// ピクセル(Fragment)シェーダ
	fragShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	if (!fragShader) {
		LOGE("Failed to compile fragment shader");
		goto ERROR;
	}
	
	// Create shader program
	return createProgram(vertShader, fragShader, "abc", user);
	
ERROR:	// エラー時の処理
	if (vertShader) {
		glDeleteShader(vertShader);
	}
	if (fragShader) {
		glDeleteShader(fragShader);
	}
	return NULL;
}

GLuint Shader::loadShader(const char* name, int user) {
	GLuint vertShader = NULL;
	GLuint fragShader = NULL;
	//	GLuint program = NULL;
	
	char fileName[512];
	
	// 頂点(Vertex)シェーダ
	sprintf(fileName, "%s.vsh", name);
	vertShader = compileShaderFromFile(GL_VERTEX_SHADER, fileName);
	if (!vertShader) {
		LOGE("Failed to compile vertex shader");
		goto ERROR;
	}
	
	// ピクセル(Fragment)シェーダ
	sprintf(fileName, "%s.fsh", name);
	fragShader = compileShaderFromFile(GL_FRAGMENT_SHADER, fileName);
	if (!fragShader) {
		LOGE("Failed to compile fragment shader");
		goto ERROR;
	}
	
	return createProgram(vertShader, fragShader, name, user);
	
ERROR:	// エラー時の処理
	if (vertShader) {
		glDeleteShader(vertShader);
	}
	if (fragShader) {
		glDeleteShader(fragShader);
	}
	return NULL;
}

void Shader::useProgram() {
	if (static_program != gProgram) {
		glUseProgram(gProgram);
		static_program = gProgram;
	}
}


////////////////////////////////////////////////////////////////
///
/// ここからはprivate関数
///
////////////////////////////////////////////////////////////////

GLuint Shader::createProgram(GLuint vertShader, GLuint fragShader, const char* name, int user) {
	GLuint program = NULL;
	
	// Create shader program
	program = glCreateProgram();
	// Attach vertex shader to program
	glAttachShader(program, vertShader);
	// Attach fragment shader to program
	glAttachShader(program, fragShader);
	
	// attributeを設定
	// この関数は、virtual関数なので、継承したクラスの
	// メソッドを実行します。
	bindAttribute(program, name, user);
	
	// Link program
	if (!linkProgram(program)) {
		goto ERROR;
	}
	
	// Get uniform locations
	// この関数は、virtual関数なので、継承したクラスの
	// メソッドを実行します。
	getUniform(program, name, user);
	
	// 後始末
	if (vertShader) {
		glDeleteShader(vertShader);
	}
	if (fragShader) {
		glDeleteShader(fragShader);
	}
	return program;
	
ERROR:	// エラー時の処理
	if (vertShader) {
		glDeleteShader(vertShader);
	}
	if (fragShader) {
		glDeleteShader(fragShader);
	}
	if (program) {
		glDeleteProgram(program);
	}
	LOGE("Failed to createProgram");
	return NULL;
}

GLuint Shader::compileShaderFromFile(GLenum shaderType, const char* fileName) {
	// assetからファイルの読み込み
	AssetManager mgr = AssetManager::getInstance();
	std::vector<char> *fdata = mgr.open(fileName);
	if (fdata == NULL) {
		return NULL;
	}
	fdata->push_back('\0');
	const char *data = (const char *) &(*fdata)[0];
    
    // ログ
//    LOGD("[%s]:\nFile Size: [%d]\n%s\n", fileName, fdata->size(), data);
	
	// シェーダのコンパイル処理
	GLint status;
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &data, NULL);
	glCompileShader(shader);
	
	// メモリを解放
	delete fdata;
	
	// コンパイル結果を取得
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == 0) {
		glDeleteShader(shader);
		LOGE("%s compile error.", fileName);
		return NULL;
	}
	return shader;
}


GLuint Shader::compileShader(GLenum shaderType, const char* source) {
	// シェーダのコンパイル処理
	GLint status;
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	
	// コンパイル結果を取得
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == 0) {
		glDeleteShader(shader);
		LOGE("%s compile error.", source);
		return NULL;
	}
	return shader;
}

bool Shader::linkProgram(GLuint prog) {
	GLint status;
	// プログラムをリンク
	glLinkProgram(prog);
	// リンク結果を取得
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == 0) {
		return false;
	}
	return true;
}

