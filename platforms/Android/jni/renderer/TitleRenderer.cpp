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
 * TitleRenderer.cpp
 *
 *  Created on: 2011/08/11
 *      Author: GClue, Inc.
 */
#include "TitleRenderer.h"
#include "Animation.h"
#include "Figure.h"
#include "Texture.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "ViewGroup.h"
#include "Button.h"
#include "ImageView.h"
#include "ScrollView.h"
#include "GridView.h"
#include "RadioGroup.h"
#include "RadioButton.h"
#include "NumberView.h"
#include "PackerTexture.h"
#include "Application.h"
#include "AssetManager.h"


TitleRenderer::TitleRenderer() {
}

TitleRenderer::~TitleRenderer() {
}

void TitleRenderer::createCalender(GridView *gv) {
}

RadioButton* TitleRenderer::createRadioButton() {
	return NULL;
}


void TitleRenderer::createTextTexture() {
//	// 文字列を登録
//	JNIDrawText("あいうえおあいうえお", 16, 1.0, 0.0, 0.0);
//	JNIDrawText("かきくけこ", 24, 0.0, 1.0, 0.0);
//	JNIDrawText("さしすせそ", 32, 0.0, 0.0, 1.0);
//
//	// 登録した文字列をテクスチャに変換
//	DELETE(texture->textTexture);
//	texture->textTexture = JNIGetTextTexture();
}


void TitleRenderer::setup() {
}

void TitleRenderer::resize(float aspect) {
	this->aspect = aspect;
}

void TitleRenderer::onButtonClick(Button *btn) {
}

void TitleRenderer::onButtonTouchStart(Button *btn) {
}

void TitleRenderer::onCheckedChanged(RadioGroup *group, RadioButton *btn) {
}
