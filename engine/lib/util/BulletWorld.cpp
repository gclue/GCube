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

#include "BulletWorld.h"
#include "Log.h"

/**
 * Bulletの当たり判定用コールバック関数.
 */
static bool ContactCallBack(btManifoldPoint& cp, void* body0, void* body1) {
	btRigidBody* rbody0 = (btRigidBody*) body0;
	btRigidBody* rbody1 = (btRigidBody*) body1;
	
	UserObj *user = (UserObj*)rbody0->getUserPointer();
	BulletWorld *world = (BulletWorld*)user->world;

	if (world && world->handler) world->handler->contactBulletObject(world, rbody0, rbody1);

	return true;
};

// コンストラクタ
BulletWorld::BulletWorld(float g) {
	LOGD("*BulletWorld");
	collisionConfig = new btDefaultCollisionConfiguration();
	broadphase = new btDbvtBroadphase();
	collisionDispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0, g, 0));
	linearDamping = 0.5;
	angularDamping = 0.5;
	handler = NULL;
	gContactProcessedCallback = ContactCallBack;
}

// デストラクタ
BulletWorld::~BulletWorld() {
	LOGD("*~BulletWorld");
	clear();
	delete collisionConfig;
	delete collisionDispatcher;
	delete broadphase;
	delete solver;
}

// 初期化
void BulletWorld::clear() {
	LOGD("*BulletWorld::clear");
	if (dynamicsWorld) {
		btCollisionObjectArray array = dynamicsWorld->getCollisionObjectArray();
		for (int i = array.size() - 1; i >= 0; i--) {
			btCollisionObject* obj = array[i];
			if (obj) {
				btRigidBody* body = btRigidBody::upcast(obj);
				if (body) {
					btCollisionShape *shape = body->getCollisionShape();
					if (shape) {
						delete shape;
					}
					btMotionState *motion = body->getMotionState();
					if (motion) {
						delete motion;
					}
					UserObj *user = (UserObj*)body->getUserPointer();
					if (user) {
						delete user;
					}
				}
				dynamicsWorld->removeCollisionObject(obj);
				delete obj;
			}
		}
	}
}

// ステップ実行
void BulletWorld::step(double t) {
	if (dynamicsWorld) {
		// 第２引数は処理が間に合わなかった場合に何ステップ実行するか（1/30単位）
		dynamicsWorld->stepSimulation(t, 2);
		
		// 角剛体の処理
		if (handler) {
			btCollisionObjectArray array = dynamicsWorld->getCollisionObjectArray();
			for (int i=array.size()-1; i >= 0; i--) {
				if (handler) handler->stepBulletObject(this, array[i]);
			}
		}
	}
}

// 保存
bool BulletWorld::save(const char *filename) {
	bool result = false;
	const int maxSerializeBufferSize = 1024 * 1024 * 5;
	btDefaultSerializer* serializer = new btDefaultSerializer(maxSerializeBufferSize);
	if (serializer) {
		dynamicsWorld->serialize(serializer);
		
		FILE* file = fopen(filename, "wb");
		if (file) {
			fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file);
			fclose(file);
			result = true;
		}
		
		delete serializer;
	}
	return result;
}

// 読み込み
bool BulletWorld::load(const char *filename) {
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL) {
		// 保存データの確認を行い、存在しない場合には
		// データの読み込みは行わないようにする
		return false;
	}
	fclose(fp);
	
	btBulletWorldImporter* fileLoader = new btBulletWorldImporter(dynamicsWorld);
	if (fileLoader) {
		fileLoader->loadFile(filename);
		delete fileLoader;
		return true;
	}
	return false;
}

// 地面追加
btRigidBody* BulletWorld::addGround(float x, float y, float z, float restitution, float friction) {
	
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(x, y, z));
	btCollisionShape* shape = new btStaticPlaneShape(btVector3(0,1,0),0);
	if (shape) {
		return addRigidShape(shape, transform, 0, restitution, friction, false);
	}
	return NULL;
}

// ボックス追加
btRigidBody* BulletWorld::addRigidBoxShape(float x, float y, float z, float xx, float yy,
											float zz, float mass, float restitution, float friction, bool isKinematic) {
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(x, y, z));
	btCollisionShape* shape = new btBoxShape(btVector3(xx, yy, zz));
	if (shape) {
		return addRigidShape(shape, transform, mass, restitution, friction, isKinematic);
	}
	return NULL;
}

// 円柱追加
btRigidBody* BulletWorld::addRigidCylinderShape(float x, float y, float z, float xx, float yy,
												 float zz, float mass, float restitution, float friction, bool isKinematic) {
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(x, y, z));
	btCylinderShape *cylinderShape = new btCylinderShape(btVector3(xx, yy, zz));
	if (cylinderShape) {
		return addRigidShape(cylinderShape, transform, mass, restitution, friction, isKinematic);
	}
	return NULL;
}

// 球追加
btRigidBody* BulletWorld::addRigidSphereShape(float x, float y, float z, float r,
											   float mass, float restitution, float friction, bool isKinematic) {
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(x, y, z));
	btCollisionShape* shape = new btSphereShape(r);
	if (shape) {
		return addRigidShape(shape, transform, mass, restitution, friction, isKinematic);
	}
	return NULL;
}

// メッシュ追加
btRigidBody* BulletWorld::addMeshShape(float x, float y, float z,
										Figure* mesh, float mass, float restitution, float friction, bool isKinematic) {
	
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(x, y, z));

	// メッシュの再構成
	btTriangleMesh* triangle = new btTriangleMesh;
	short indexCount = mesh->vertexIndexes.size();
	for( unsigned int i = 0; i < indexCount; i +=3 ){
		btVector3 tri[3];
		for (int j=0; j<3; j++) {
			int idx = mesh->vertexIndexes.at(i+j) * 3;
			btScalar x = mesh->vertices.at(idx);
			btScalar y = mesh->vertices.at(idx+1);
			btScalar z = mesh->vertices.at(idx+2);
			tri[j] = btVector3(x, y, z);
		}
		triangle->addTriangle( tri[0], tri[1], tri[2] );
	}

    btConvexShape* convex = new btConvexTriangleMeshShape( triangle );

    btShapeHull *hull = new btShapeHull( convex );
    hull->buildHull( convex->getMargin() );

    btConvexHullShape* shape = new btConvexHullShape;
    for( int i = 0; i < hull->numVertices(); i++ ){
        shape->addPoint( hull->getVertexPointer()[i] );
    }
	
	delete triangle;
	delete convex;
	delete hull;

	return addRigidShape(shape, transform, mass, restitution, friction, isKinematic);
}


////////////////////////////////////////////////////////////////
//
// ここからはprivate関数
//
////////////////////////////////////////////////////////////////

btRigidBody* BulletWorld::addRigidShape(btCollisionShape *shape, btTransform transform, btScalar mass,
										 float restitution, float friction, bool isKinematic) {
	// 慣性の計算
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.0f) {
		shape->calculateLocalInertia(mass, localInertia);
	}
	
	// using motionstate is recommended, it provides interpolation capabilities,
	// and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	if (!myMotionState) {
		return NULL;
	}
	
	// 剛体情報を作成
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	// 剛体を作成
	btRigidBody *body = new btRigidBody(rbInfo);
	// 反発係数を設定
	body->setRestitution(restitution);
	// 摩擦係数を設定
	body->setFriction(friction);
	// 空気抵抗
	body->setDamping(linearDamping, angularDamping);
	
	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(body);
	
	// 静的オブジェクトの処理
	if (isKinematic) {
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}
	
	//
	UserObj *user = new UserObj();
	user->world = this;
	body->setUserPointer(user);
	
	return body;
}

