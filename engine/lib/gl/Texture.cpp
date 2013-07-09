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

#include "Texture.h"
#include "APIGlue.h"
#include "Figure.h"
#include "FigureCache.h"

#define MAX_FIGURE_ID 100000
#define MAX_FIGURE_SUB_ID 100001

Texture::Texture(const char *fname):GCObject() {
	cache = new FigureCache();
	dispW = 640.0;
	load(fname);
}

Texture::Texture() {
	cache = new FigureCache();
	dispW = 640.0;
}

Texture::~Texture() {
	if (texName) {
		glDeleteTextures(1 , &texName);
	}
	DELETE(cache);
}

void Texture::load(const char *fname) {
	filename.assign(fname);
	GCLoadTexture(this, fname);
}

void Texture::setImageData(unsigned char *imageData, int width, int height) {
	glGenTextures(1, &texName);
	if (!texName) {
		LOGE("** ERROR (create texture) **");
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glGenerateMipmap(GL_TEXTURE_2D);

	this->width = width;
	this->height = height;
}

void Texture::clamp() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


// 座標を正規化します
Rectf Texture::normalize(Rectf rect) {
	int width = this->width;
	int height = this->height;
	rect.top /= height;
	rect.left /= width;
	rect.right /= width;
	rect.bottom /= height;
	return rect;
}

Figure* Texture::makePlate() {
	// キャッシュにFigureが存在しないか検索
	Figure *figure = cache->searchFigure(MAX_FIGURE_ID, MAX_FIGURE_SUB_ID, 0, 0);
	if (figure) {
		return figure;
	}
	
	Rectf rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = 1;
	rect.bottom = 1;
	float w = (rect.right - rect.left) * this->width / dispW;
	float p = (rect.bottom - rect.top) / (rect.right - rect.left);
	figure = createPlate(w, w*p);
	figure->build();
	
	// キャッシュに登録
	cache->putFigure(MAX_FIGURE_ID, MAX_FIGURE_SUB_ID, 0, 0, figure);

	return figure;
}

void Texture::setSize(float w) {
	dispW = w;
}

void Texture::reload() {
	if (!GCLoadTexture(this, filename.c_str())) {
		LOGD("***********ERROR*Texture::reload**********");
	}
	if (cache) {
		cache->rebuild();
	}
}
