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

//
//  BulletWorld.h
//  GCube
//

#ifndef GCube_BulletWorld_h
#define GCube_BulletWorld_h

#include "Figure.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletFile.h"
#include "btBulletWorldImporter.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

class BulletWorld;

/**
 * Bulletの当たり判定のコールバック関数を定義します.
 */
extern ContactProcessedCallback	gContactProcessedCallback;

/**
 * UserObject構造体.
 * RigitBodyのUserObjectを使用する場合は
 * このオブジェクトがセットされているので、こちらを使用してください。
 */
struct UserObj {
	unsigned long id;
	void *user;
	BulletWorld *world;
	UserObj() {
		user = NULL;
		world = NULL;
		id = 0;
	}
};

/**
 * イベントハンドラインターフェイス.
 */
class IBulletWorldEventHandler {
public:
	/**
	 * デストラクタ.
	 */
	virtual ~IBulletWorldEventHandler(){};
	
	/**
	 * 各オブキェクトの処理.
	 */
	virtual void stepBulletObject(BulletWorld *world, btCollisionObject *obj) {};
	
	/**
	 * 各オブキェクトの衝突処理.
	 */
	virtual void contactBulletObject(BulletWorld *world, btRigidBody *obj0, btRigidBody *obj1) {};
	
	/**
	 * 各オブキェクトの保存処理.
	 */
	virtual bool saveBulletObject(BulletWorld *world, btRigidBody *body, UserObj *obj, int index, int max) {return true;};
	
	/**
	 * 各オブキェクトの読み込み処理.
	 */
	virtual bool loadBulletObject(BulletWorld *world, btRigidBody *body, UserObj *obj, int index, int max) {return true;};
};

/**
 * Bulletを操作するための便利クラス.
 */
class BulletWorld {
private:
	
	/**
	 * 指定されたShapeをBulletに追加します.
	 * <br><br>
	 * @param[in] shape 追加するShape
	 * @param[in] transform マトリクス
	 * @param[in] mass 質量
	 * @param[in] restitution 反発係数
	 * @param[in] friction 摩擦係数
	 * @param[in] isKinematic 運動
	 */
	btRigidBody* addRigidShape(btCollisionShape *shape, btTransform transform, btScalar mass,
							   float restitution, float friction, bool isKinematic);
	
public:
	IBulletWorldEventHandler *handler; //!< イベントハンドラ
	btDiscreteDynamicsWorld *dynamicsWorld;				//!< Bulletの本体
	btDefaultCollisionConfiguration *collisionConfig;	//!< Bulletの当たり判定の設定
	btCollisionDispatcher *collisionDispatcher;			//!< Bulletの当たり判定のディスパッチャー
	btBroadphaseInterface *broadphase;					//!< Bulletのブロードフェーズオブジェクト
	btSequentialImpulseConstraintSolver *solver;		//!< Bulletのソルバオブジェクト
	float linearDamping; //!= 追加するBodyのリニア減衰
	float angularDamping; //!= 追加するBodyの角速度減衰
	
	/**
	 * コンストラクタ.
	 */
	BulletWorld(float g=-9.8);
	
	/**
	 * デストラクタ.
	 */
	~BulletWorld();
	
	/**
	 * イベントリスナー設定.
	 * @param[in] listener リスナー
	 */
	void setEventHandler(IBulletWorldEventHandler *handler) {this->handler=handler;};
	
	/**
	 * 初期化します.
	 */
	void clear();
	
	/**
	 * ステップ実行します.
	 */
	void step(double t);
	
	/**
	 * 現在の状態を指定されたファイルに保持します.
	 * @param[in] filename ファイル名
	 */
	bool save(const char *filename);
	
	/**
	 * 指定されたファイルから前回の状態を復帰します.
	 * @param[in] filename ファイル名
	 */
	bool load(const char *filename);
	
	/**
	 * 地面を追加します.
	 */
	btRigidBody* addGround(float x, float y, float z, float restitution, float friction);
	
	/**
	 * Bulletにボックスオブジェクトを追加します.
	 * <br><br>
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @param[in] z z座標
	 * @param[in] xx x軸への幅
	 * @param[in] yy y軸への幅
	 * @param[in] zz z軸への幅
	 * @param[in] mass 質量
	 * @param[in] restitution 反発係数
	 * @param[in] friction 摩擦係数
	 * @param[in] isKinematic 運動
	 */
	btRigidBody* addRigidBoxShape(float x, float y, float z, float xx, float yy,
								  float zz, float mass, float restitution, float friction, bool isKinematic);
	
	/**
	 * Bulletに円柱オブジェクトを追加します.
	 * <br><br>
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @param[in] z z座標
	 * @param[in] xx x軸への幅
	 * @param[in] yy y軸への幅
	 * @param[in] zz z軸への幅
	 * @param[in] mass 質量
	 * @param[in] restitution 反発係数
	 * @param[in] friction 摩擦係数
	 * @param[in] isKinematic 運動
	 */
	btRigidBody* addRigidCylinderShape(float x, float y, float z, float xx, float yy,
									   float zz, float mass, float restitution, float friction, bool isKinematic);
	
	/**
	 * Bulletに球オブジェクトを追加します.
	 * <br><br>
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @param[in] z z座標
	 * @param[in] r 球半径
	 * @param[in] mass 質量
	 * @param[in] restitution 反発係数
	 * @param[in] friction 摩擦係数
	 * @param[in] isKinematic 運動
	 */
	btRigidBody* addRigidSphereShape(float x, float y, float z, float r,
									 float mass, float restitution, float friction, bool isKinematic);
	
	/**
	 * Bulletにメッシュオブジェクトを追加します.
	 * <br><br>
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @param[in] z z座標
	 * @param[in] mesh Figure
	 * @param[in] mass 質量
	 * @param[in] restitution 反発係数
	 * @param[in] friction 摩擦係数
	 * @param[in] isKinematic 運動
	 */
	btRigidBody* addMeshShape(float x, float y, float z,
							  Figure* mesh, float mass, float restitution, float friction, bool isKinematic);
};

#endif
