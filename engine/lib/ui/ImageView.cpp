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
 * ImageView.cpp
 *
 *  Created on: 2011/07/25
 *      Author: GClue, Inc.
 */
#include "ImageView.h"
#include "Animation.h"
#include "AnimationSet.h"
#include "Figure.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "Texture.h"
#include "TextureManager.h"
#include "SharedTexture.h"
#include "ApplicationController.h"

ImageView::ImageView() : View() {
	figure = NULL;
	clickable = false;
}

ImageView::ImageView(const char *name, float w, float h) : View() {
	figure = NULL;
	clickable = false;
	
	ApplicationController *ctl = ApplicationController::getInstance();
	TextureManager *texMgr = ctl->texMgr;
	Texture *tex = NULL;
	SharedTexture *stex = NULL;
	if ((stex = texMgr->findSharedTexture(name))) {
		if (w > 0 && h > 0) {
			this->setFigure(stex->makePlateWithSize(name, w, h));
		} else if (w > 0) {
			this->setFigure(stex->makeFixPlate(name, w));
		} else {
			this->setFigure(stex->makePlate(name));
		}
		this->setTexture(stex);
	} else if ((tex = texMgr->loadTexture(name))) {
		this->setFigure(tex->makePlate());
		this->setTexture(tex);
	}
}

ImageView::ImageView(GCContext *context) : View(context) {
	figure = NULL;
	clickable = false;
}

ImageView::~ImageView() {
}

void ImageView::setFigure(Figure *figure) {
	this->figure = figure;

	if (figure) {
		size.x = figure->size.x / 2.0;
		size.y = figure->size.y / 2.0;
	}
}

void ImageView::draw(double dt, ViewContext *context) {
	if (animation && !animation->isStarted()) {
		return;
	}

	if (!figure) {
		return;
	}
	
	//ブレンドタイプに合わせて合成を変える
	//現在はImageViewにしかつけてません
	switch(blendType) {
		case BLEND_TYPE_ADD:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case BLEND_TYPE_MULTIPLE:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		case BLEND_TYPE_REVERSE:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
			break;
		default:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
	}
	
	float alpha = 1.0;
	float bright = 1.0;
	Matrix3D mtx;
	testMatrix3D(&mtx, &alpha, &bright);

	context->shader->setMVPMatrix(context->camera, &mtx);
	context->shader->setAlpha(alpha);
	context->shader->setBright(bright);

	figure->bind();
	figure->draw();
	
	//最後にアルファ合成に戻す
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

