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
 * NumberView.cpp
 *
 *  Created on: 2011/08/15
 *      Author: GClue, Inc.
 */
#include "NumberView.h"
#include "Animation.h"
#include "AnimationSet.h"
#include "Figure.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "SharedTexture.h"
#include "Texture.h"
#include "TextureManager.h"
#include "ApplicationController.h"

NumberView::NumberView() : View() {
	initNumberView();
}

NumberView::NumberView(GCContext *context) : View(context) {
	initNumberView();
}

NumberView::~NumberView() {
}

void NumberView::initNumberView() {
	zeroFill = false;
	value = 0;
	digit = 8;
	aligned = ALIGN_RIGHT;
	w = 0;
	h = 0;
	space = 0;
	
	memset(figure, 0x0, sizeof(figure));
	memset(adjustX, 0x0, sizeof(adjustX));
	memset(adjustY, 0x0, sizeof(adjustY));
}

void NumberView::drawFigure(ViewContext *context, int index, float x, float y) {
	float alpha = 1.0;
	float bright = 1.0;
	Matrix3D mtx;

	if (!figure[index]) {
		return;
	}

	mtx.loadIdentity();
	mtx.translate(x, y, 0);
	testMatrix3D(&mtx, &alpha, &bright);

	context->shader->setMVPMatrix(context->camera, &mtx);
	context->shader->setAlpha(alpha);
	context->shader->setBright(bright);

	figure[index]->bind();
	figure[index]->draw();
}

void NumberView::draw(double dt, ViewContext *context) {
	float width = size.x - w / 2;

	long long v = value;
	float x = width;
	float y = 0;
	float dx = w + space;
	bool first = false;

	switch (aligned) {
	case ALIGN_LEFT:
		x = -width;
		dx = w+space;
		if(value == 0) {
			drawFigure(context, 0, x + adjustX[0], y+adjustY[0]);
			break;
		}
		for (int i = digit - 1; i >= 0 ; i--) {
			
			v = value;
			for (int j = 0; j < i; j++) {
				v /= 10;
			}
			int a = v % 10;
			if (zeroFill || first || v != 0) {
				drawFigure(context, a, x + adjustX[a], y + adjustY[a]);
				x += dx;
			}
			if (!zeroFill && v != 0) {
				first = true;
			}
		}
		break;
	case ALIGN_RIGHT:
		x = width;
		dx = -w-space;
		for (int i = 0; i < digit; i++) {
			int a = v % 10;
			drawFigure(context, a, x + adjustX[a], y + adjustY[a]);
			v /= 10;
			x += dx;
			if (!zeroFill && v == 0) {
				// 値がなくなったらこれ以上書く必要がないのでbreak.
				break;
			}
		}
		break;
	}
}

void NumberView::setValue(long long value) {
	this->value = value;
}

int NumberView::getValue() {
	return value;
}

void NumberView::setDigit(int digit) {
	this->digit = digit;
}

int NumberView::getDigit() {
	return digit;
}

void NumberView::setZeroFill(bool flag) {
	zeroFill = flag;
}

bool NumberView::isZeroFill() {
	return zeroFill;
}

void NumberView::setAlign(int aligned) {
	this->aligned = aligned;
}

int NumberView::getAlign() {
	return aligned;
}

void NumberView::setFigure(int index, Figure *f) {
	figure[index] = f;

	if (f && w == 0 && h == 0) {
		w = f->size.x;
		h = f->size.y;
	}
}

void NumberView::setFigure(int index, const char *fname) {
	ApplicationController *ctl = ApplicationController::getInstance();
	TextureManager *texMgr = ctl->texMgr;
	SharedTexture *tex = NULL;
	if ((tex = texMgr->findSharedTexture(fname))) {
		this->setFigure(index, tex->makePlate(fname));
		this->setTexture(tex);
	}
}

void NumberView::setFigure(const char *fname[]) {
	for (int i = 0; i < 10; i++) {
		setFigure(i, fname[i]);
	}
}

void NumberView::setFontSize(float w, float h) {
	this->w = w;
	this->h = h;
}

void NumberView::setAdjustPositionEachNumbers(int index, float x, float y){
	adjustX[index] = x;
	adjustY[index] = y;
}

void NumberView::setSpace(float space) {
	this->space = space;
}
