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
 * Layer3D.cpp
 *
 *  Created on: 2011/09/15
 *      Author: GClue, Inc.
 */
#include "Layer3D.h"

#include <memory>
#include "Camera.h"
#include "BoneShader.h"
#include "Joint.h"

// コンストラクタ
Layer3D::Layer3D(GCContext *context) : Layer(context) {
	LOGD("**Layer3D");
	camera = new Camera();
	Vector3D eye = Vector3D(5,5,-10);
	Vector3D at = Vector3D(0,0,0);
	camera->transForm.lookAt(&eye, &at);
}

// デストラクタ
Layer3D::~Layer3D() {
	LOGD("**~Layer3D");
	delete camera;
	
	// addしたオブジェクトを解放
	std::vector<Figure*> figs;
	std::vector<Texture*> texs;
	std::vector<Matrix3D*> mtxs;
	std::map<int, FigureSet>::iterator it = figures.begin();
	while (it != figures.end()) {
		FigureSet set = (*it).second;
		// 同じオブジェクトを２回処理しないように。。。
		if (set.fig) {
			std::vector<Figure*>::iterator itf = std::find(figs.begin(), figs.end(), set.fig);
			if (itf == figs.end()) {
				figs.push_back(set.fig);
				delete set.fig;
			}
		}
		if (set.tex) {
			std::vector<Texture*>::iterator its = std::find(texs.begin(), texs.end(), set.tex);
			if (its == texs.end()) {
				texs.push_back(set.tex);
				delete set.tex;
			}
		}
		if (set.mtx) {
			std::vector<Matrix3D*>::iterator itm = std::find(mtxs.begin(), mtxs.end(), set.mtx);
			if (itm == mtxs.end()) {
				mtxs.push_back(set.mtx);
				delete set.mtx;
			}
		}
		it++;
	}
}

// Figure追加
void Layer3D::addFigure(int id, Figure *fig, Texture *tex, Matrix3D *mtx) {
	FigureSet set;
	set.fig = fig;
	set.tex = tex;
	set.mtx = mtx;
	figures[id] = set;
}

// Furure検索
Figure* Layer3D::findFigureByID(int id) {
	std::map<int, FigureSet>::iterator it = figures.find(id);
	if (it!=figures.end()) {
		return (*it).second.fig;
	} else {
		return NULL;
	}
}

// Texture検索
Texture* Layer3D::findTextureByID(int id) {
	std::map<int, FigureSet>::iterator it = figures.find(id);
	if (it!=figures.end()) {
		return (*it).second.tex;
	} else {
		return NULL;
	}
}

// Matrix検索
Matrix3D* Layer3D::findMatrixByID(int id) {
	std::map<int, FigureSet>::iterator it = figures.find(id);
	if (it!=figures.end()) {
		return (*it).second.mtx;
	} else {
		return NULL;
	}
}

//////////////////////////////////////////////////////////
// Layer の実装
//////////////////////////////////////////////////////////

/**
 * セットアップを行います.
 */
void Layer3D::setup() {
	LOGD("**Layer3D::setup");
}

/**
 * 画面のリサイズを行います.
 * @param aspect 画面のアスペクト比
 */
void Layer3D::resize(float aspect) {
	LOGD("**Layer3D::resize:%f", aspect);
	camera->aspect = aspect;
	camera->loadPerspective();
}

/**
 * 画面の描画を行います.
 * @param dt 前回描画からの差分時間
 */
void Layer3D::render(double dt) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	BoneShader *shader = context->shader3d;
	shader->useProgram();
	std::map<int, FigureSet>::iterator it = figures.begin();
	while (it != figures.end()) {
		FigureSet set = (*it).second;
			
		// マトリックス設定
		if (set.mtx) {
			shader->setNormalMatrix(set.mtx);
			shader->setMVPMatrix(camera, set.mtx);
		} else {
			shader->setNormalMatrix(set.fig->transForm);
			shader->setMVPMatrix(camera, set.fig->transForm);
		}
		
		// テクスチャ設定
		if (set.tex) {
			shader->bindTexture(set.tex->texName);
		} else {
			shader->bindTexture(0);
		}
		
		// ジョイント設定
		if (set.fig->joint) {
			set.fig->joint->setSkinningMatrix(shader);
		}
		
		// 描画
		set.fig->bind();
		set.fig->draw();
		
		it++;
	}
}

/**
 * タッチイベント.
 * @param event タッチイベント
 */
bool Layer3D::onTouch(TouchEvent &event) {
	LOGD("**Layer3D::onTouch:(%f,%f)", event.x, event.y);
	return false;
}

/**
 * コンテキストが切り替わったことを通知します.
 */
void Layer3D::onContextChanged() {
	LOGD("**Layer3D::onContextChanged:()");
	
	// FigureとTextureを再構築
	std::vector<Figure*> figs;
	std::vector<Texture*> texs;
	std::map<int, FigureSet>::iterator it = figures.begin();
	while (it != figures.end()) {
		FigureSet set = (*it).second;
		// 同じオブジェクトを２回処理しないように。。。
		if (set.fig) {
			std::vector<Figure*>::iterator itf = std::find(figs.begin(), figs.end(), set.fig);
			if (itf == figs.end()) {
				figs.push_back(set.fig);
				set.fig->build();
			}
		}
		if (set.tex) {
			std::vector<Texture*>::iterator its = std::find(texs.begin(), texs.end(), set.tex);
			if (its == texs.end()) {
				texs.push_back(set.tex);
				set.tex->reload();
			}
		}
		
		it++;
	}
}


