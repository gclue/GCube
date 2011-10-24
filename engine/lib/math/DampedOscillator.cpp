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

/*
 * DampedOscillator.cpp
 *
 *  Created on: 2011/04/27
 *      Author: haga
 */

#include "DampedOscillator.h"

DampedOscillator::DampedOscillator() {
	//初期値の設定.
	//このパラメータで減衰振動が取得できる.
	this->ak = -1;
	this->alpha = M_PI / 2;
	this->h = 0.2;
	this->n = 1;
}
DampedOscillator::~DampedOscillator() {

}

float DampedOscillator::dampedOscillate() {
	return ak * exp(-h * n * thisTime) * sin(sqrt(1 - h * h) * n * thisTime + alpha);
}
float DampedOscillator::dampedOscillate(float t) {
	return ak * exp(-h * n * t) * sin(sqrt(1 - h * h) * n * t + alpha);
}
void DampedOscillator::setParameters(float ak, float h, float n, float alpha){
	this->ak = ak;
	this->h = h;
	this->n = n;
	this->alpha = alpha;
}
