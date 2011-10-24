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
 * Oscilator.cpp
 *
 *  Created on: 2011/04/27
 *      Author: haga
 */

#include "Oscillator.h"

const float Oscillator::timeStep = 0.1;

Oscillator::Oscillator() {
	//初期設定をします.
	//振幅が1で周期がPI, 位相は０の振動をします.
	this->amplitude = 1;
	this->period = M_PI;
	this->phase = 0;
	this->thisTime = 0;
	this->dynamicAmpAmp = 0.5f;
}
Oscillator::~Oscillator() {

}

void Oscillator::setAmplitude(float amp) {
	this->amplitude = amp;
}
void Oscillator::setPeriod(float period) {
	this->period = period;
}
void Oscillator::setPhase(float phase) {
	this->phase = phase;
}

float Oscillator::sinOscillate(float t) {
	return this->amplitude * sin(t);
}
float Oscillator::sinOscillate() {
	return this->amplitude * sin(this->thisTime);
}
float Oscillator::getDynamicSinAmplitude(int size, int index) {
	return dynamicAmpAmp * sin(M_PI / (size - 1) * index);
}
float Oscillator::getDynamicCosAmplitude(int size, int index) {
	return dynamicAmpAmp * cos(M_PI / (size - 1) * index);
}


void Oscillator::stepTime(float step) {
	this->thisTime += step;
}
void Oscillator::stepTime() {
	this->thisTime += timeStep;
}
