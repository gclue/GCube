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

#include "SharedTexture.h"
#include "Figure.h"
#include "FigureCache.h"
#include "Texture.h"
#include "PrimitiveObjectBuilder.h"

SharedTexture::SharedTexture() {
}

SharedTexture::~SharedTexture() {
	LOGD("SharedTexture destructor");
}

// テクスチャを貼る板ポリゴン取得
Figure* SharedTexture::makePlate(int ID, int sub) {
	// キャッシュにFigureが存在しないか検索
	Figure *figure = cache->searchFigure(ID, sub, 0, 0);
	if (figure) {
		return figure;
	}
    
    TexData tex = this->getTexData(ID, sub);
	Rectf rect = normalize(tex.rect);
    normalize(rect);
	float w = (rect.right - rect.left) * this->width / dispW;
	float p = (rect.bottom - rect.top) / (rect.right - rect.left);
    if (tex.rotate == 1) {
        figure = createPlateWithRotateTexpos(w*p, w, rect.left, rect.top, rect.right, rect.bottom);
    } else {
        figure = createPlateWithTexpos(w, w*p, rect.left, rect.top, rect.right, rect.bottom);
    }
	figure->build();
    
	// キャッシュに登録
	cache->putFigure(ID, sub, 0, 0, figure);
	return figure;
}

// テクスチャを貼る板ポリゴン取得
Figure* SharedTexture::makeFixPlate(int ID, int sub, float w) {
	// キャッシュにFigureが存在しないか検索
	Figure *figure = cache->searchFigure(ID, sub, w, 0);
	if (figure) {
		return figure;
	}
    
    TexData tex = this->getTexData(ID, sub);
	Rectf rect = normalize(tex.rect);
	float p = (rect.bottom - rect.top) / (rect.right - rect.left);
    if (tex.rotate == 1) {
        figure = createPlateWithRotateTexpos(w*p, w, rect.left, rect.top, rect.right, rect.bottom);
    } else {
        figure = createPlateWithTexpos(w, w*p, rect.left, rect.top, rect.right, rect.bottom);
    }
	figure->build();
    
	// キャッシュに登録
	cache->putFigure(ID, sub, w, 0, figure);
	return figure;
}

// テクスチャを貼る板ポリゴン取得
Figure* SharedTexture::makeFixHeightPlate(int ID, int sub, float h) {
	// キャッシュにFigureが存在しないか検索
	Figure *figure = cache->searchFigure(ID, sub, 0, h);
	if (figure) {
		return figure;
	}
    
    TexData tex = this->getTexData(ID, sub);
	Rectf rect = normalize(tex.rect);
	float p = (rect.right - rect.left) / (rect.bottom - rect.top);
    if (tex.rotate == 1) {
        figure = createPlateWithRotateTexpos(h, h*p, rect.left, rect.top, rect.right, rect.bottom);
    } else {
        figure = createPlateWithTexpos(h*p, h, rect.left, rect.top, rect.right, rect.bottom);
    }
	figure->build();
    
	// キャッシュに登録
	cache->putFigure(ID, sub, 0, h, figure);
	return figure;
}

Figure* SharedTexture::makePlateWithSize(int ID, int sub, float w, float h) {
	// キャッシュにFigureが存在しないか検索
	Figure *figure = cache->searchFigure(ID, sub, w, h);
	if (figure) {
		return figure;
	}
    
    TexData tex = this->getTexData(ID, sub);
	Rectf rect = normalize(tex.rect);
    
    if (tex.rotate == 1) {
        figure = createPlateWithRotateTexpos(h, w, rect.left, rect.top, rect.right, rect.bottom);
    } else {
        figure = createPlateWithTexpos(w, h, rect.left, rect.top, rect.right, rect.bottom);
    }
	figure->build();
    
	// キャッシュに登録
	cache->putFigure(ID, sub, w, h, figure);
	return figure;
}

Figure* SharedTexture::makePlate(const char *name) {
	
    //	return fig;
	// キャッシュにFigureが存在しないか検索
	Figure *figure = cache->searchFigure(name, 0, 0);
	if (figure) {
		return figure;
	}
    
    TexData tex = this->getTexData(name);
	Rectf rect = normalize(tex.rect);
	float w = (rect.right - rect.left) * this->width / dispW;
	float p = (rect.bottom - rect.top) / (rect.right - rect.left);
    if (tex.rotate == 1) {
        figure = createPlateWithRotateTexpos(w*p, w, rect.left, rect.top, rect.right, rect.bottom);
    } else {
        figure = createPlateWithTexpos(w, w*p, rect.left, rect.top, rect.right, rect.bottom);
    }
    
	figure->build();
    
	// キャッシュに登録
	cache->putFigure(name, 0, 0, figure);
	
	return figure;
}

Figure* SharedTexture::makeFixPlate(const char *name, float w) {
	// キャッシュにFigureが存在しないか検索
	Figure *figure = cache->searchFigure(name, w, 0);
	if (figure) {
		return figure;
	}
    
    TexData tex = this->getTexData(name);
	Rectf rect = normalize(tex.rect);
	float p = (rect.bottom - rect.top) / (rect.right - rect.left);
    
    if (tex.rotate == 1) {
        figure = createPlateWithRotateTexpos(w*p, w, rect.left, rect.top, rect.right, rect.bottom);
    } else {
        figure = createPlateWithTexpos(w, w*p, rect.left, rect.top, rect.right, rect.bottom);
    }
	figure->build();
    
	// キャッシュに登録
	cache->putFigure(name, w, 0, figure);
	return figure;
}

Figure* SharedTexture::makeFixHeightPlate(const char *name, float h) {
	// キャッシュにFigureが存在しないか検索
	Figure *figure = cache->searchFigure(name, 0, h);
	if (figure) {
		return figure;
	}
    
    TexData tex = this->getTexData(name);
    Rectf rect = normalize(tex.rect);
	float p = (rect.right - rect.left) / (rect.bottom - rect.top);
    
    if (tex.rotate == 1) {
        figure = createPlateWithRotateTexpos(h, h*p, rect.left, rect.top, rect.right, rect.bottom);
    } else {
        figure = createPlateWithTexpos(h*p, h, rect.left, rect.top, rect.right, rect.bottom);
    }
	figure->build();
    
	// キャッシュに登録
	cache->putFigure(name, 0, h, figure);
	return figure;
}

Figure* SharedTexture::makePlateWithSize(const char *name, float w, float h) {
	// キャッシュにFigureが存在しないか検索
	Figure *figure = cache->searchFigure(name, w, h);
	if (figure) {
		return figure;
	}
    
    TexData tex = this->getTexData(name);
	Rectf rect = normalize(tex.rect);
    
    if (tex.rotate == 1) {
        figure = createPlateWithRotateTexpos(h, w, rect.left, rect.top, rect.right, rect.bottom);
    } else {
        figure = createPlateWithTexpos(w, h, rect.left, rect.top, rect.right, rect.bottom);
    }
	figure->build();
    
	// キャッシュに登録
	cache->putFigure(name, w, h, figure);
	return figure;
}
