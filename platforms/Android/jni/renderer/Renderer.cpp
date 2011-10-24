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
 * Renderer.cpp
 *
 *  Created on: 2011/05/27
 *      Author: haga
 */
#include "Renderer.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "View.h"
#include "TextureManager.h"
#include "defines.h"
#include "Application.h"

Renderer::Renderer() {
	root = NULL;
	dialog = NULL;
	camera = NULL;
	shader = NULL;
	aspect = 1.0;
}

Renderer::~Renderer() {
	DELETE(root);
}

View *Renderer::findViewByID(int id) {
	if (root) {
		return root->findViewByID(id);
	}
	return NULL;
}

View* Renderer::findViewByID(const char *name) {
	if (root) {
	}
	return NULL;
}

void Renderer::render(double dt) {
	if (root) {
		if (shader) {
			shader->useProgram();
		}
		root->render(dt);
	}
	if (dialog) {
		dialog->render(dt);
	}
}

void Renderer::onTouch(TouchEvent &event) {
	if (dialog) {
		dialog->onTouch(event);
	} else if (root) {
		root->onTouch(event);
	}
}

void Renderer::setContentView(View *view) {
	DELETE(root);
	root = view;
}

View *Renderer::getContentView() {
	return root;
}

void Renderer::openDialog(View *view) {
	closeDialog();
	dialog = view;
}

void Renderer::closeDialog() {
	DELETE(dialog);
}

bool Renderer::isDialog() {
	return (dialog != NULL);
}

void Renderer::setCamera(Camera *camera) {
	this->camera = camera;
}

void Renderer::setShader(SimpleShader *shader) {
	this->shader = shader;
}
