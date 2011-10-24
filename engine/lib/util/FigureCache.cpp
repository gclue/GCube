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
 * FigureCache.cpp
 *
 *  Created on: 2011/08/22
 *      Author: GClue, Inc.
 */
#include "FigureCache.h"
#include "Figure.h"
#include "defines.h"
/**
 * 同じサイズと認識するための閾値を定義.
 */
#define THRESHOLD 0.0001

FigureObj::FigureObj() {
	figure = NULL;
}

FigureObj::~FigureObj() {
	DELETE(figure);
}

FigureCache::FigureCache() {
}

FigureCache::~FigureCache() {
	for (int i = 0; i < cache.size(); i++) {
		FigureObj *obj = cache.at(i);
		DELETE(obj);
	}
	cache.clear();
}

Figure *FigureCache::searchFigure(int id, int subId, float w, float h) {
	for (int i = 0; i < cache.size(); i++) {
		FigureObj *obj = cache.at(i);
		if (obj->id == id && obj->subId == subId &&
			fabs(obj->w - w) < THRESHOLD && fabs(obj->h - h) < THRESHOLD) {
			return obj->figure;
		}
	}
	return NULL;
}

Figure *FigureCache::searchFigure(const char *name, float w, float h) {
	for (int i = 0; i < cache.size(); i++) {
		FigureObj *obj = cache.at(i);
		if (strcmp(obj->name.c_str(), name) == 0 &&
			fabs(obj->w - w) < THRESHOLD && fabs(obj->h - h) < THRESHOLD) {
			return obj->figure;
		}
	}
	return NULL;
}

void FigureCache::putFigure(int id, int subId, float w, float h, Figure *figure) {
	FigureObj *obj = new FigureObj();
	obj->id = id;
	obj->subId = subId;
	obj->w = w;
	obj->h = h;
	obj->figure = figure;
	cache.push_back(obj);
}

void FigureCache::putFigure(const char *name, float w, float h, Figure *figure) {
	FigureObj *obj = new FigureObj();
	obj->name.append(name);
	obj->w = w;
	obj->h = h;
	obj->figure = figure;
	cache.push_back(obj);
}

void FigureCache::destroyVAO() {
	for (int i = 0; i < cache.size(); i++) {
		FigureObj *obj = cache.at(i);
		if (obj && obj->figure) {
			obj->figure->destroyVAO();
		}
	}
}

void FigureCache::rebuild() {
	for (int i = 0; i < cache.size(); i++) {
		FigureObj *obj = cache.at(i);
		if (obj && obj->figure) {
			obj->figure->buildVAO();
		}
	}
}
