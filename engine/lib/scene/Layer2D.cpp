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
 * Layer2D.cpp
 *
 *  Created on: 2011/09/15
 *      Author: GClue, Inc.
 */
#include "Layer2D.h"
#include "Camera.h"
#include "SimpleShader.h"

Layer2D::Layer2D(GCContext *context) : Layer(context){
	root = NULL;
	dialog = NULL;
	aspect = 1.0;
}

Layer2D::~Layer2D() {
	DELETE(root);
	DELETE(dialog);
}

View *Layer2D::findViewByID(int id) {
	if (root) {
		return root->findViewByID(id);
	}
	return NULL;
}

void Layer2D::setContentView(View *view) {
	DELETE(root);
	root = view;
}

View *Layer2D::getContentView() {
	return root;
}

void Layer2D::openDialog(View *view) {
	closeDialog();
	dialog = view;
}

void Layer2D::closeDialog() {
	DELETE(dialog);
}

bool Layer2D::isDialog() {
	return (dialog != NULL);
}

//////////////////////////////////////////////////////////
// Layer の実装
//////////////////////////////////////////////////////////

/**
 * セットアップを行います.
 */
void Layer2D::setup() {
}

/**
 * 画面のリサイズを行います.
 * @param aspect 画面のアスペクト比
 */
void Layer2D::resize(float aspect) {
	this->aspect = aspect;
}

/**
 * 画面の描画を行います.
 * @param dt 前回描画からの差分時間
 */
void Layer2D::render(double dt) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	if (root) {
		if (context->shader) {
			context->shader->useProgram();
		}
		root->render(dt);
	}
	if (dialog) {
		dialog->render(dt);
	}
}

/**
 * タッチイベント.
 * @param event タッチイベント
 */
bool Layer2D::onTouch(TouchEvent &event) {
	if (dialog) {
		return dialog->onTouch(event);
	} else if (root) {
		return root->onTouch(event);
	}
	return false;
}
