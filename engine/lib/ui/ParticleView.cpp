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
 * ParticleView.cpp
 *
 *  Created on: 2011/08/16
 *      Author: GClue, Inc.
 */
#include "ParticleView.h"
#include "Animation.h"
#include "AnimationSet.h"
#include "Figure.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "Texture.h"

ParticleView::ParticleView() : View() {
	figure = NULL;
	clickable = false;
}

ParticleView::ParticleView(GCContext *context) : View(context) {
	figure = NULL;
	clickable = false;
}

ParticleView::~ParticleView() {
}

void ParticleView::draw(double dt, ViewContext *context) {
	if (animation && !animation->isStarted()) {
		return;
	}

	if (!figure) {
		return;
	}

//	float alpha = 1.0;
//	float bright = 1.0;
	Matrix3D mtx;
}
