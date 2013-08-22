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

#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include "glcommon.h"

/**
 * カメラ.
 */
class Camera {
public:
	GLfloat zNear;			//!< ニアクリップ.
	GLfloat zFar;			//!< ファークリップ.
	GLfloat fieldOfView;	//!< 画角.
	GLfloat aspect;			//!< アスペクト比.
	Matrix3D transForm; 	//!< 視点座標系マトリクス.
	Matrix3D projection;	//!< クリップ座標系マトリクス.

	/**
	 * コンストラクタ.
	 */
	Camera();

	/**
	 * デストラクタ.
	 */
	~Camera();

	/**
	 * 透視投影.
	 */
	void loadPerspective();

	/**
	 * 平行投影.
	 */
	void loadOrthographic(bool fitWidth);

	/**
	 * カメラのLookAtを設定します.
	 * @param[in] eye 目の位置
	 * @param[in] at 目視点の位置
	 * @param[in] up カメラの上方向
	 */
	void lookAt(Vector3D &eye, Vector3D &at, Vector3D &up);
	
	/**
	 * カメラのLookAtを設定します.
	 * @param[in] eyex 目の位置のx座標
	 * @param[in] eyey 目の位置のy座標
	 * @param[in] eyez 目の位置のz座標
	 * @param[in] atx 目視点の位置のx座標
	 * @param[in] aty 目視点の位置のy座標
	 * @param[in] atz 目視点の位置のz座標
	 * @param[in] upx カメラの上方向のx座標
	 * @param[in] upy カメラの上方向のy座標
	 * @param[in] upz カメラの上方向のz座標
	 */
	void lookAt(float eyex, float eyey, float eyez,
				float atx, float aty, float atz,
				float upx, float upy, float upz);
	
	/**
	 * 指定されたモデルのマトリクスを視点座標に変換します.
	 * @param[in] modelMtx 視点座標系に変換するマトリクス
	 * @param[out] 視点座標系に変換されたマトリクス(4x4の配列)
	 */
	void modelViewMatrix(Matrix3D *modelMtx, float *outMtx);

	/**
	 * 指定されたモデルのマトリクスをクリップ座標に変換します.
	 * @param[in] modelMtx クリップ座標系に変換するマトリクス
	 * @param[out] クリップ座標系に変換されたマトリクス(4x4の配列)
	 */
	void modelViewProjectionMatrix(Matrix3D *modelMtx, float *outMtx);
	
	/**
	 * プロジェクションマッピングのマトリクスを取得します.
	 * @param[out] プロジェクションマッピングのマトリクス(4x4の配列)
	 */
	void modelViewProjectionMatrix(float *outMtx);

	/**
	 * 指定された座標からスクリーンの座標に変換します.
	 * @param[in] modelMtx クリップ座標系に変換するマトリクス
	 * @param[in] localPos 変換する座標
	 * @return クリップ座標に変換された座標
	 */
	Vector2D* positionOnScreen(Matrix3D *modelView, Vector3D *localPos);
};

#endif // end of GL_CAMERA_H
