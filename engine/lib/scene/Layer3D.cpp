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
#include "BulletWorld.h"

// コンストラクタ
Layer3D::Layer3D(GCContext *context) : Layer(context) {
	LOGD("**Layer3D");
	camera = new Camera();
	Vector3D eye = Vector3D(5,5,10);
	Vector3D at = Vector3D(0,0,0);
	camera->transForm.lookAt(&eye, &at);
	bullet = NULL;
}

// デストラクタ
Layer3D::~Layer3D() {
	LOGD("**~Layer3D");
	delete camera;
	delete bullet;
	
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
	
	// 追加したLightを解放
	std::map<int, Light*>::iterator it2 = lights.begin();
	while (it2 != lights.end()) {
		delete (*it2).second;
		it2++;
	}

}

// Figure追加
void Layer3D::addFigure(int id, Figure *fig, Texture *tex, Matrix3D *mtx, RigidBodyType type, RigidBodyOption option) {
	// 
	FigureSet set;
	set.fig = fig;
	set.tex = tex;
	set.mtx = mtx;
	set.body = NULL;

	if (type != RigidBodyType_None) {
		if (!bullet) {
			bullet = new BulletWorld();
			bullet->setEventHandler(this);
		}
		//
		switch (type) {
			case RigidBodyType_None:
				break;
			case RigidBodyType_Box:
				set.body = bullet->addRigidBoxShape(option.x, option.y, option.z, option.sizeX, option.sizeY, option.sizeZ, option.mass, option.restitution, option.friction, option.isKinematic);
				break;
			case RigidBodyType_Cylinder:
				set.body = bullet->addRigidCylinderShape(option.x, option.y, option.z, option.sizeX, option.sizeY, option.sizeZ, option.mass, option.restitution, option.friction, option.isKinematic);
				break;
			case RigidBodyType_Ground:
				bullet->addGround(option.x, option.y, option.z, option.restitution, option.friction);
				break;
			case RigidBodyType_Sphere:
				set.body = bullet->addRigidSphereShape(option.x, option.y, option.z, option.radius, option.mass, option.restitution, option.friction, option.isKinematic);
				break;
			case RigidBodyType_Mesh:
				set.body = bullet->addMeshShape(option.x, option.y, option.z, fig, option.mass, option.restitution, option.friction, option.isKinematic);
				break;
		}
		if (set.body) set.body->setUserPointer(&figures[id]);
	}
	
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

// ライト追加
void Layer3D::addLight(int id, Light *light) {
	lights[id] = light;
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

	if (bullet) {
		// 位置変更があった場合はBulletに反映（TODO:スケーリングの考慮も必要）
		float mat[16];
		std::map<int, FigureSet>::iterator it = figures.begin();
		while (it != figures.end()) {
			FigureSet set = (*it).second;
			if( set.getMatrix()->dirtyflag && set.body && set.body->getMotionState() ){
				btTransform transform;
				set.getMatrix()->getElements(mat);
				transform.setFromOpenGLMatrix(mat);
				set.body->setWorldTransform(transform);
				set.getMatrix()->dirtyflag = false;
			}
			it++;
		}
		
		// 物理演算
		bullet->step(dt);
	}
	
	// ライト
	BoneShader *shader = context->shader3d;
	shader->useProgram();
	if (lights.size()>0) {
		std::map<int, Light*>::iterator itl = lights.begin();
		while (itl != lights.end()) {
			shader->setLight((*itl).second);
			itl++;
		}
	} else {
		shader->setLight(NULL);
	}
	
	// 描画
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
		} else {
			shader->setSkinningMatrix(NULL, 0);
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

//////////////////////////////////////////////////////////
// IBulletWorldEventHandler の実装
//////////////////////////////////////////////////////////

/**
 * 各オブキェクトの処理.
 */
void Layer3D::stepBulletObject(BulletWorld *world, btCollisionObject *obj) {
	// オブジェクトに物理演算の結果を設定
	FigureSet *set = (FigureSet*)obj->getUserPointer();
	if (set) {
		float worldMat[16];
		btRigidBody* body = btRigidBody::upcast(obj);
		if( body && body->getMotionState() ){
			btDefaultMotionState* motion = (btDefaultMotionState*)body->getMotionState();
			motion->m_graphicsWorldTrans.getOpenGLMatrix(worldMat);
		} else {
			obj->getWorldTransform().getOpenGLMatrix(worldMat);
		}
		set->getMatrix()->setElements(worldMat);
	}
}



