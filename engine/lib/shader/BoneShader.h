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

#ifndef BONESHADER_H_
#define BONESHADER_H_

#include "glcommon.h"
#include "Camera.h"
#include "Shader.h"
#include "Figure.h"

/**
 * ボーンアニメーションシェーダークラス.
 */
class BoneShader : public Shader {
private:
	GLuint gProgram;	//!< プログラム
	int texname;		//!< 使用しているtextureのID

public:
	/**
	 * コンストラクタ.
	 */
	BoneShader();

	/**
	 * デストラクタ.
	 */
	virtual ~BoneShader();

	/**
	 * シェーダの使用を開始します.
	 */
	void useProgram();

	/**
	 * テクスチャをバインドします.
	 * <br>
	 * @param[in] texname テクスチャ名
	 */
	void bindTexture(int texname);

	/**
	 * カメラと変換行列からMVPをシェーダに設定します.
	 * <br>
	 * @param[in] camera カメラオブジェクト
	 * @param[in] matrix 変換マトリックス
	 */
	void setMVPMatrix(Camera *camera, Matrix3D *matrix);

	void setSkinningMatrix(Matrix3D **matrix, int len);
	void setNormalMatrix(Matrix3D *matrix);

	/**
	 * シェーダのAttributeへのバインド処理を行います.
	 * <br><br>
	 * ES2Rendererから継承した関数.
	 * ES2Renderer内部から呼び出されるので、実装しないとエラーになります.
	 * <br>
	 * @param[in] program シェーダプログラム
	 * @param[in] name シェーダファイルへの名前
	 * @param[in] user ユーザ識別ID
	 */
	void bindAttribute(GLuint program, const char *name, int user);

	/**
	 * シェーダのUniformを取得します.
	 * <br><br>
	 * ES2Rendererから継承した関数.
	 * ES2Renderer内部から呼び出されるので、実装しないとエラーになります.
	 * <br>
	 * @param[in] program シェーダプログラム
	 * @param[in] name シェーダファイルへの名前
	 * @param[in] user ユーザ識別ID
	 */
	void getUniform(GLuint program, const char *name, int user);
};

#endif /* BONESHADER_H_ */
