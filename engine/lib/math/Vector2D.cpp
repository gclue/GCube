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

#include "Vector2D.h"

#include <math.h>


Vector2D::Vector2D() {

}

Vector2D::~Vector2D() {

}

void Vector2D::add(Vector2D *vec) {
	x += vec->x;
	y += vec->y;
}

void Vector2D::subtract(Vector2D* vec) {
	x -= vec->x;
	y -= vec->y;
}

void Vector2D::multiply(float val) {
	x *= val;
	y *= val;
}

void Vector2D::normalize() {
	float len = sqrt(x*x + y*y);
	x /= len;
	y /= len;
}

float Vector2D::dotProduct(Vector2D* vec) {
	return x * vec->x + y * vec->y;
}

float Vector2D::crossProduct(Vector2D* vec) {
	return x * vec->y - y * vec->x;
}
