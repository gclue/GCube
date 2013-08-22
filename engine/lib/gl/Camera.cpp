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

#include "Camera.h"
#include "matrixUtil.h"

Camera::Camera() {
	zNear = 0.1;
	zFar = 1000.0;
	fieldOfView = 50.0;
	aspect = 0.67; // 320x480
}

Camera::~Camera() {
}

// 透視投影を設定
void Camera::loadPerspective() {
	mtxLoadPerspective(projection.matrix, fieldOfView, aspect, zNear, zFar);
}

// 平行投影を設定
void Camera::loadOrthographic(bool fitWidth) {
	float left = -1.0;
	float right = 1.0;
	float bottom = -1.0;
	float top = 1.0;

	if (fitWidth) {
		bottom /= aspect;
		top /= aspect;
	} else {
		left *= aspect;
		right *= aspect;
	}
	
	mtxLoadOrthographic(projection.matrix, left, right, bottom, top, zNear, zFar);
}

void Camera::modelViewMatrix(Matrix3D *modelMtx, float *outMtx) {
	mtxMultiply(outMtx, transForm.matrix, modelMtx->matrix);
}

void Camera::modelViewProjectionMatrix(Matrix3D *modelMtx, float *outMtx) {
	float lookatMtx[16];
	modelViewMatrix(modelMtx, lookatMtx);
	mtxMultiply(outMtx, projection.matrix, lookatMtx);
}

void Camera::modelViewProjectionMatrix(float *outMtx) {
	mtxMultiply(outMtx, projection.matrix, transForm.matrix);
}

void Camera::lookAt(Vector3D &eye, Vector3D &at, Vector3D &up) {
	transForm.lookAt(&eye, &at, &up);
}

void Camera::lookAt(float eyex, float eyey, float eyez, float atx, float aty, float atz, float upx, float upy, float upz) {
	Vector3D eye(eyex, eyey, eyez);
	Vector3D at(atx, aty, atz);
	Vector3D up(upx, upy, upz);
	transForm.lookAt(&eye, &at, &up);
}

Vector2D* Camera::positionOnScreen(Matrix3D *modelView, Vector3D *localPos) {
//	Matrix3D *mv = modelViewMatrix(modelView);
//	Vector3D *vec = mv->transformVector3D(localPos);
//	vec = projection->transformVector3D(vec);
//
//	CGPoint pos;
//	pos.x = vec.x / vec.w;
//	pos.y = vec.y / vec.w;
//
//	return pos;
	return NULL;
}

