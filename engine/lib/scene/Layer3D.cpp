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

/**
 * 3Dオブジェクト情報構造体.
 */
struct FigureSet {
	int id;
	Figure *fig;
	Texture *tex;
	Matrix3D *mtx;
	btRigidBody* body;
	GCObject *userObj;
	
	FigureSet() {
		fig = NULL;
		tex = NULL;
		mtx = NULL;
		body = NULL;
		userObj = NULL;
		id = -1;
	}
	
	FigureSet(const FigureSet& obj) {
		this->set(obj.id, obj.fig, obj.tex, obj.mtx, obj.userObj);
		this->body = obj.body;
	}
	
	virtual ~FigureSet() {
		if (mtx) mtx->release();
		if (fig) fig->release();
		if (tex) tex->release();
		if (userObj) userObj->release();
	}
	
	void set(int id, Figure *fig, Texture *tex, Matrix3D *mtx, GCObject *userObj) {
		this->id = id;
		this->fig = fig;
		this->tex = tex;
		this->mtx = mtx;
		this->userObj = userObj;
		if (mtx) {mtx->retain();}
		if (fig) {fig->retain();}
		if (tex) {tex->retain();}
		if (userObj) {userObj->retain();}
	}
	
	void setInfoData(FigureInfo &info) {
		info.id = id;
		info.fig = fig;
		info.tex = tex;
		info.mtx = mtx;
		info.userObj = userObj;
	}

	Matrix3D *getMatrix() {
		if (mtx) {
			return mtx;
		} else {
			return fig->transForm;
		}
	}
};


// コンストラクタ
Layer3D::Layer3D(GCContext *context) : Layer(context) {
	LOGD("**Layer3D");
	camera = new Camera();
	Vector3D eye = Vector3D(5,5,10);
	Vector3D at = Vector3D(0,0,0);
	camera->transForm.lookAt(&eye, &at);
	bullet = NULL;
	handler = NULL;
	gravity = -9.8;
	objcount = 0;
}

// デストラクタ
Layer3D::~Layer3D() {
	LOGD("**~Layer3D");
	delete camera;
	delete bullet;
	std::map<int, FigureSet*>::iterator it = figures.begin();
	while (it != figures.end()) {
		FigureSet *set = (*it).second;
		delete set;
		it++;
	}
}


// Figure追加
int Layer3D::addFigure(FigureInfo &info, RigidBodyOption option) {
	
	objcount++;
	FigureSet *set = new FigureSet();
	if (info.fig || info.mtx) {
		set->set(objcount, info.fig, info.tex, info.mtx, info.userObj);
	} else {
		info.mtx = new Matrix3D();
		set->set(objcount, info.fig, info.tex, info.mtx, info.userObj);
		info.mtx->release();
	}

	if (option.type != RigidBodyType_None) {
		if (!bullet) {
			bullet = new BulletWorld(gravity);
			bullet->setEventHandler(this);
		}
		//
		switch (option.type) {
			case RigidBodyType_None:
				break;
			case RigidBodyType_Box:
				set->body = bullet->addRigidBoxShape(option.x, option.y, option.z, option.sizeX, option.sizeY, option.sizeZ, option.mass, option.restitution, option.friction, option.isKinematic);
				break;
			case RigidBodyType_Cylinder:
				set->body = bullet->addRigidCylinderShape(option.x, option.y, option.z, option.sizeX, option.sizeY, option.sizeZ, option.mass, option.restitution, option.friction, option.isKinematic);
				break;
			case RigidBodyType_Ground:
				bullet->addGround(option.x, option.y, option.z, option.restitution, option.friction);
				break;
			case RigidBodyType_Sphere:
				set->body = bullet->addRigidSphereShape(option.x, option.y, option.z, option.radius, option.mass, option.restitution, option.friction, option.isKinematic);
				break;
			case RigidBodyType_Mesh:
				set->body = bullet->addMeshShape(option.x, option.y, option.z, info.fig, option.mass, option.restitution, option.friction, option.isKinematic);
				break;
		}
		if (set->body) {
			UserObj *user = (UserObj*)set->body->getUserPointer();
			user->user = set;
		}
	}
	
	figures[objcount] = set;
	return objcount;
}

// Fugure検索
Figure* Layer3D::findFigureByID(int id) {
	std::map<int, FigureSet*>::iterator it = figures.find(id);
	if (it!=figures.end()) {
		return (*it).second->fig;
	} else {
		return NULL;
	}
}

// Texture検索
Texture* Layer3D::findTextureByID(int id) {
	std::map<int, FigureSet*>::iterator it = figures.find(id);
	if (it!=figures.end()) {
		return (*it).second->tex;
	} else {
		return NULL;
	}
}

// Matrix検索
Matrix3D* Layer3D::findMatrixByID(int id) {
	std::map<int, FigureSet*>::iterator it = figures.find(id);
	if (it!=figures.end()) {
		return (*it).second->getMatrix();
	} else {
		return NULL;
	}
}

// UserObject検索
GCObject* Layer3D::findUserObjectByID(int id) {
	std::map<int, FigureSet*>::iterator it = figures.find(id);
	if (it!=figures.end()) {
		return (*it).second->userObj;
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
		std::map<int, FigureSet*>::iterator it = figures.begin();
		while (it != figures.end()) {
			FigureSet set = *(*it).second;
			if( set.getMatrix()->dirtyflag && set.body ){
				btTransform transform;
				set.getMatrix()->getElements(mat);
				transform.setFromOpenGLMatrix(mat);
				if (set.body->isStaticOrKinematicObject()) {
					set.body->getMotionState()->setWorldTransform(transform);
				} else {
					set.body->setWorldTransform(transform);
				}
				set.body->activate(true);
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
	std::map<int, FigureSet*>::iterator it = figures.begin();
	Figure *fig = NULL;
	while (it != figures.end()) {
		FigureSet *set = (*it).second;
		
		// 削除処理
		if (handler) {
			FigureInfo info = {NULL};
			set->setInfoData(info);
			if (handler->handleFigure(this, info)) {
				if (set->body) {
					if (bullet) {
						bullet->dynamicsWorld->removeRigidBody(set->body);
						delete set->body;
					}
				}
				delete set;
				figures.erase(it++);
				continue;
			} else {
				if (set->body) {
					// 外力がかかっている場合
					float fx = info.force.x;
					float fy = info.force.y;
					float fz = info.force.z;
					if (fx>0 || fy>0 || fz>0) {
						// 位置
						float px = info.rel_pos.x;
						float py = info.rel_pos.y;
						float pz = info.rel_pos.z;
						set->body->applyForce(btVector3(fx,fy,fz), btVector3(px,py,pz));
						set->body->activate(true);
					}
				}
			}
		}

		// 描画対象がないので無視
		if (!set->fig) {
			it++;
			continue;
		}
		
		// マトリックス設定
		Matrix3D *mtx = set->getMatrix();
		shader->setNormalMatrix(mtx);
		shader->setMVPMatrix(camera, mtx);
		
		// テクスチャ設定
		if (set->tex) {
			shader->bindTexture(set->tex->texName);
		} else {
			shader->bindTexture(0);
		}
		
		// ジョイント設定
		if (set->fig->joint) {
			set->fig->joint->setSkinningMatrix(shader);
		} else {
			shader->setSkinningMatrix(NULL, 0);
		}
		
		// 描画
		if (fig != set->fig) {
			set->fig->bind();
			fig = set->fig;
		}
		set->fig->draw();
		
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
	std::map<int, FigureSet*>::iterator it = figures.begin();
	while (it != figures.end()) {
		FigureSet set = *(*it).second;
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
	UserObj *user = (UserObj*)obj->getUserPointer();
	FigureSet *set = (FigureSet*)user->user;
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
		set->getMatrix()->dirtyflag = false;
	}
}

/**
 * 各オブキェクトの衝突処理.
 */
void Layer3D::contactBulletObject(BulletWorld *world, btRigidBody *obj0, btRigidBody *obj1) {
	if (handler) {
		UserObj *user0 = (UserObj*)obj0->getUserPointer();
		UserObj *user1 = (UserObj*)obj1->getUserPointer();
		FigureSet *set0 = (FigureSet*)user0->user;
		FigureSet *set1 = (FigureSet*)user1->user;
		
		FigureInfo info0 = {NULL};
		if (set0) {
			set0->setInfoData(info0);
		}
		
		FigureInfo info1 = {NULL};
		if (set1) {
			set1->setInfoData(info1);
		}
		
		handler->contactFigure(this, info0, info1);
	}
	
	
}



