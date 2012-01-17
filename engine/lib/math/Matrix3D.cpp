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

#include "Matrix3D.h"
#include "Vector3D.h"

#include <math.h>
#include <stdlib.h>
#include <memory.h>

#include "Log.h"

#include "matrixUtil.h"

Matrix3D::Matrix3D() {
	loadIdentity();
	dirtyflag = false;
}

Matrix3D::~Matrix3D() {
}

// デバッグログ
void Matrix3D::debugPrint() {
	LOGD("matrix:[%f,%f,%f,%f][%f,%f,%f,%f][%f,%f,%f,%f]]", matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5], matrix[6], matrix[7], matrix[8], matrix[9], matrix[10], matrix[11], matrix[12], matrix[13], matrix[14], matrix[15]);
}

// 等しいか
bool Matrix3D::equals(Matrix3D *mtx) {
	for (int i=0; i<16; i++) {
		if (matrix[i]!=mtx->matrix[i]) return false;
	}
	return true;
}

// 要素の取得
void Matrix3D::getElements(float *outElements) {
    memcpy(outElements, matrix, sizeof(float) * 16);
}

// 単位行列を設定
void Matrix3D::loadIdentity() {
	mtxLoadIdentity(matrix);
	dirtyflag = true;
}

// 要素の設定
void Matrix3D::setElements(const float *inElements) {
    memcpy(matrix, inElements, sizeof(float) * 16);
	dirtyflag = true;
}

// 乗算
void Matrix3D::multiply(Matrix3D *mtx) {
	float ret[16];
	mtxMultiply(ret, matrix, mtx->matrix);
	memcpy(matrix, ret, sizeof(float)*16);
	dirtyflag = true;
}

// 平行移動
void Matrix3D::translate(float x, float y, float z, bool apply) {
	if (apply) {
		mtxTranslateApply(matrix, x, y, z);
	} else {
		mtxTranslateMatrix(matrix, x, y, z);
	}
	dirtyflag = true;
}

// 回転
void Matrix3D::rotate(float deg, RotateDir dir, bool apply) {
	if (apply) {
		switch (dir) {
			case RotateDirX:
				mtxRotateXApply(matrix, deg);
				break;
			case RotateDirY:
				mtxRotateYApply(matrix, deg);
				break;
			case RotateDirZ:
				mtxRotateZApply(matrix, deg);
				break;
		}
	} else {
		float rad = deg * (M_PI / 180.0f);
		switch (dir) {
			case RotateDirX:
				mtxRotateXMatrix(matrix, rad);
				break;
			case RotateDirY:
				mtxRotateYMatrix(matrix, rad);
				break;
			case RotateDirZ:
				mtxRotateZMatrix(matrix, rad);
				break;
		}
	}
	dirtyflag = true;
}

// 回転
void Matrix3D::rotate(float deg, float x, float y, float z, bool apply) {
	if (apply) {
		mtxRotateApply(matrix, deg, x, y, z);
	} else {
		mtxRotateMatrix(matrix, deg, x, y, z);
	}
	dirtyflag = true;
}

// 拡大縮小
void Matrix3D::scale(float x, float y, float z, bool apply) {
	if (apply) {
		mtxScaleApply(matrix, x, y, z);
	} else {
		mtxScaleMatrix(matrix, x, y, z);
	}
	dirtyflag = true;
}

// 法線ベクトル取得
void Matrix3D::normalMatrix(float *mtxout) {
	float tmp1[9];
	float tmp2[9];
	float normal[9];
	mtx3x3FromTopLeftOf4x4(tmp1, matrix);
	mtx3x3Invert(tmp2, tmp1);
	mtx3x3Transpose(normal, tmp2);
	memcpy(mtxout, normal, sizeof(float) * 9);
}

// 変換行列を適用したVector3Dを返します
void Matrix3D::transformVector3D(Vector3D *outVec, const Vector3D *inVec) {
	
	bool flg = false;
	if (!inVec) {
		inVec = new Vector3D();
		flg = true;
	}
	
	float tmp[4];
	for (int i=0; i<4; i++) {
		tmp[i] = (matrix[i   ] * inVec->x) +
				 (matrix[i+ 4] * inVec->y) +
				 (matrix[i+ 8] * inVec->z) +
				 (matrix[i+12] * inVec->w);
	}
	outVec->x = tmp[0];
	outVec->y = tmp[1];
	outVec->z = tmp[2];
	outVec->w = tmp[3];
	
	if (flg) {
		delete inVec;
	}
}

// 指定の方向を向かせる
void Matrix3D::lookAt(Vector3D* eye, Vector3D* at, Vector3D* up) {
	
	Vector3D ra = Vector3D(at);
	ra.multiply(-1);
	
	Vector3D f = Vector3D(eye);
	f.multiply(-1.0);
	f.subtract(&ra);
	f.normalize();
	
	Vector3D upactual;
	if (up==NULL) {
		upactual = Vector3D(0, 1, 0);
	} else {
		upactual = Vector3D(up);
	}
	upactual.normalize();
	
	Vector3D s = Vector3D(f);
	s.crossProduct(&upactual);
	s.normalize();
	
	Vector3D u = new Vector3D(s);
	u.crossProduct(&f);
	u.normalize();
	
	matrix[ 0] = s.x;
	matrix[ 1] = u.x;
	matrix[ 2] = -f.x;
	matrix[ 3] = 0;
	
	matrix[ 4] = s.y;
	matrix[ 5] = u.y;
	matrix[ 6] = -f.y;
	matrix[ 7] = 0;
	
	matrix[ 8] = s.z;
	matrix[ 9] = u.z;
	matrix[10] = -f.z;
	matrix[11] = 0;
	
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;

	this->translate(eye->x, eye->y, eye->z, true);
}


