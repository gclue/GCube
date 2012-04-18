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
 * View.cpp
 *
 *  Created on: 2011/07/22
 *      Author: GClue, Inc.
 */
#include "View.h"
#include "IAnimation.h"
#include "SimpleShader.h"
#include "Texture.h"

#include <math.h>

View::View(GCContext *context):GCObject() {
	this->context = context;
	animation = NULL;
	parent = NULL;
	texture = NULL;
	visible = true;
	clickable = true;
	absolute = false;
	rotate = 0.0;
	alpha = 1.0;
	bright = 1.0;
	aspect = 1.0;
	scale.x = 1.0;
	scale.y = 1.0;
	point.x = 0.0;
	point.y = 0.0;
	userObj = NULL;
	userID = -1;
	
	blendType = BLEND_TYPE_ALPHA;
	touchListener = NULL;
}

View::~View() {
	context = NULL;
	parent = NULL;
	texture = NULL;
	DELETE(animation);
	
}

void View::setPosition(Pointf point) {
	this->point = point;
}

void View::setPosition(float x, float y) {
	this->point.x = x;
	this->point.y = y;
}

void View::setSize(Pointf size) {
	this->size = size;
}

void View::setSize(float w, float h) {
	this->size.x = w;
	this->size.y = h;
}

void View::setScale(float sx, float sy) {
	scale.x = sx;
	scale.y = sy;
}

void View::setRotate(float rotate) {
	this->rotate = rotate;
}

void View::setAlpha(float alpha) {
	this->alpha = alpha;
}

void View::setTexture(Texture *texture) {
	this->texture = texture;
}

void View::startAnimation(IAnimation *animation) {
	DELETE(this->animation);
	this->animation = animation;
	this->animation->reset();
}

void View::stopAnimation() {
	DELETE(this->animation);
	this->animation = NULL;
}

bool View::isAnimation() {
	return animation && !animation->isFinish();
}

void View::setVisible(bool v) {
	visible = v;
}

void View::setClickable(bool c) {
	clickable = c;
}

bool View::onTouch(TouchEvent &event) {
	if(!clickable || !visible) {
		return false;
	}
	
	switch(event.type) {
		case touchDown:
			if(isBound(event.x, event.y)) {
				if(touchListener) {
					touchListener->onViewTouchEvent(this);
				}
				return true;
			}
			
			break;
			
	}
	
	
	
	return false;
}

void View::setUserID(int id) {
	userID = id;
}

int View::getUserID() {
	return userID;
}

void View::setUserObj(void *obj) {
	userObj = obj;
}

void* View::getUserObj() {
	return userObj;
}

View* View::findViewByID(int id) {
	if (userID == id) {
		return this;
	}
	return NULL;
}

View* View::findViewByUserObj(void *userObj, COMPARE_FUNC_PTR *func) {
	if (func) {
		if (this->userObj == userObj) {
			return this;
		}
	} else {
		if ((*func)(this->userObj, userObj) == 0) {
			return this;
		}
	}
	return NULL;
}

bool View::isBound(float x, float y) {
	Pointf p = {0.0, 0.0};
    Pointf tScale;
    float tRotate;
    
	testMatrix2D(&p, &tScale, &tRotate);
    
    //LOG("before point:%f %f",x,y);
    if(tRotate != 0){
        float rad = -tRotate * (M_PI / 180.0f);
        float cosrad = cosf(rad);
        float sinrad = sinf(rad);
    
        float px = x - p.x;
        float py = y - p.y;
    
        float rx = px * cosrad - py * sinrad;
        float ry = px * sinrad + py * cosrad;

        x = rx + p.x;
        y = ry + p.y;
    }
    
    //LOG("after point:%f %f",x,y);
    
    
    
    
	//float px = p.x;
	//float py = p.y;
    
	float xsize = size.x * tScale.x + TOUCH_MARGIN;
	float ysize = size.y * tScale.y + TOUCH_MARGIN;//ちょっと大きくする
	if (p.x - xsize < x && x < p.x + xsize) {
		if (p.y - ysize < y && y < p.y + ysize) {
			return true;
		}
	}
	return false;
}


void View::render(double dt) {
	if (visible) {
		if (animation && !animation->isFinish()) {
			animation->step(dt);
		}
		// Viewで使用するテクスチャをバインド
		if (texture) {
			context->shader->bindTexture(texture->texName);
		}
		draw(dt);
		if (animation) {
			if (animation->isFinish()) {
				// アニメーション終了
			}
		}
	}
}

void View::testMatrix3D(Matrix3D *m, float *a, float *b) {
	if (animation) {
		animation->multiply(m);
		float alpha = animation->getAlpha();
		if (alpha != 1.0) {
			*a = (*a) * alpha;
		}
		float bright = animation->getBright();
		if (bright != 1.0) {
			*b = (*b) * bright;
		}
	}
	if (scale.x != 1.0 || scale.y != 1.0) {
		m->scale(scale.x, scale.y, 1.0);
	}
	if (rotate != 0.0) {
		m->rotate(rotate, RotateDirZ);
	}
	m->translate(point.x, point.y, 0);
	if (alpha != 1.0) {
		*a = (*a) * alpha;
	}
	if (bright != 1.0) {
		*b = (*b) * bright;
	}
	if (!absolute && parent) {
		parent->testMatrix3D(m, a, b);
	}
}

void View::testMatrix2D(Pointf *p, Pointf *s, float *r) {
	float rad = rotate * (M_PI / 180.0f);

	float cosrad = cosf(rad);
	float sinrad = sinf(rad);

	float px = p->x * scale.x;
	float py = p->y * scale.y;
	float x = px * cosrad - py * sinrad;
	float y = px * sinrad + py * cosrad;

	p->x = x + point.x;
	p->y = y + point.y;

	if (!absolute && parent) {
		parent->testMatrix2D(p,s,r);
        
        s->x *= scale.x;
        s->y *= scale.y;
        
        *r += rotate;
	}
    else {
        s->x = scale.x;
        s->y = scale.y;
        
        *r = rotate;
    }
}

void View::setBlendType(int type) {
	this->blendType = type;
}


void View::setOnTouchEventListener(IViewTouchListener *listener){
	this->touchListener = listener;
}
