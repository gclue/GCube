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

#include "Vector3D.h"

#include <math.h>

Vector3D::Vector3D() {
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

Vector3D::Vector3D(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	w = 1;
}

Vector3D::Vector3D(Vector3D *vec) {
	x = vec->x;
	y = vec->y;
	z = vec->z;
	w = vec->w;
}

Vector3D::~Vector3D() {
}

void Vector3D::add(Vector3D *vec) {
	x += vec->x;
	y += vec->y;
	z += vec->z;
	w = 0;
}

void Vector3D::subtract(Vector3D *vec) {
	x -= vec->x;
	y -= vec->y;
	z -= vec->z;
	w = 0;
}

void Vector3D::multiply(float val) {
	x *= val;
	y *= val;
	z *= val;
}

void Vector3D::normalize() {
	float len = sqrt(x*x + y*y + z*z);
	x /= len;
	y /= len;
	z /= len;
}

void Vector3D::crossProduct(Vector3D *vec) {
	float out[3];
//	out[0] = y * vec->z - vec->y * z;
//	out[1] = x * vec->z - vec->x * z;
//	out[2] = y * vec->y - vec->y * y;
	out[0] = y * vec->z - vec->y * z;
	out[1] = z * vec->x - vec->z * x;
	out[2] = x * vec->y - vec->x * y;

	x = out[0];
	y = out[1];
	z = out[2];
}
