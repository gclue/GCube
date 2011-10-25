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

#ifndef GCube_Shader_h
#define GCube_Shader_h

#include "glcommon.h"

/**
 * OpenGLES2.0のシェーダを扱う為の基底クラス.
 */
class Shader {
private:
	/**
	 * Shaderのプログラムをコンパイルし、バインドしたプログラムを返却します.
	 * @param shaderType シェーダタイプ
	 * @param pSource ソース
	 * @return プログラム
	 */
	GLuint compileShaderFromFile(GLenum shaderType, const char* fileName);
	
	/**
	 * Shaderをファイルから読み込みコンパイルします.
	 * @param shaderType シェーダタイプ(GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
	 * @param fileName ファイル名
	 * @return Shaderをコンパイルした後のID
	 */
	GLuint compileShader(GLenum shaderType, const char* fileName);
	
	/**
	 * シェーダからprogramを作成します.
	 * @param vertShader 頂点シェーダ
	 * @param fragShader ピクセルシェーダ
	 * @param name 識別するための
	 * @param user 識別するためのID
	 * @return program
	 */
	GLuint createProgram(GLuint vertShader, GLuint fragShader, const char* name, int user);
	
	/**
	 * プログラムをリンクします.
	 * @param prog リンクするプログラム
	 * @return
	 */
	bool linkProgram(GLuint prog);
	
public:
	
	/**
	 * コンストラクタ.
	 */
	Shader();
	
	/**
	 * デストラクタ.
	 */
	virtual ~Shader();
	
	/**
	 * 指定された名前のシェーダを読み込み、Programを作成します.
	 * <br><br>
	 * @param[in] name シェーダ名
	 * @param[in] user 識別ID
	 * @return ProgramのIDを返却します。シェーダの読み込みに失敗した場合にはNULLを返却します。
	 */
	GLuint loadShader(const char* name, int user);
	
	/**
	 * 指定されたVertexShaderのソースとFragmentShaderのソースからProgramを作成します.
	 * @param vertexShader 頂点シェーダ
	 * @param fragmentShader ピクセルシェーダ
	 * @param user bindAttribute、getUniformで識別するためのタグ
	 * @return Program
	 */
	GLuint loadShader(const char* vertexShader, const char* fragmentShader, int user);
	
	/**
	 * 指定されたprogramに合うシェーダのAttributeのバインド処理を行ってください.
	 * <br><br>
	 *
	 * このES2Rendererを継承したクラスで、このメソッドを実装します。
	 * このメソッドは、loadShaderが実行されたときに呼び出されます。
	 *
	 * @param[in] program シェーダプログラムのID
	 * @param[in] name シェーダのファイル名
	 * @param[in] user 識別ID
	 */
	virtual void bindAttribute(GLuint program, const char *name, int user) = 0;
	
	/**
	 * 指定されたprogrameに合うシェーダのUniformを取得します.
	 * <br><br>
	 *
	 * このES2Rendererを継承したクラスで、このメソッドを実装します。
	 * このメソッドは、loadShaderが実行されたときに呼び出されます。
	 *
	 * @param[in] program シェーダプログラムのID
	 * @param[in] name シェーダのファイル名
	 * @param[in] user 識別ID
	 */
	virtual void getUniform(GLuint program, const char *name, int user) = 0;
};

#endif
