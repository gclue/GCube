//
//  PhysicsLayer3D.h
//  GCube
//
//  Created by 小林 伸郎 on 2013/08/21.
//
//

#ifndef __GCube__PhysicsLayer3D__
#define __GCube__PhysicsLayer3D__

#include "Layer3D.h"
#include "BulletWorld.h"

/**
 * 剛体の種類.
 */
enum RigidBodyType {
	RigidBodyType_None,
	RigidBodyType_Ground,
	RigidBodyType_Box,
	RigidBodyType_Sphere,
	RigidBodyType_Cylinder,
	RigidBodyType_Mesh
};

/**
 * 剛体情報構造体.
 */
typedef struct RigidBodyOption {
	float x;
	float y;
	float z;
	float sizeX;
	float sizeY;
	float sizeZ;
	float radius;
	float mass; // 質量
	float restitution; // 反発係数
	float friction; // 摩擦係数
	bool isKinematic;
	RigidBodyType type;
	
	RigidBodyOption() {
		x = y = z = restitution = friction = isKinematic = 0;
		sizeX = sizeY = sizeZ = radius = mass = 1.0;
		type = RigidBodyType_None;
	}
} RigidBodyOption;



class PhysicsFigureSet : public FigureSet {
public:
	btRigidBody* body;
	PhysicsFigureSet();
	virtual ~PhysicsFigureSet();
};

/**
 * Bulletを管理するためのレイヤー.
 */
class PhysicsLayer3D : public Layer3D, IBulletWorldEventHandler {
private:
	BulletWorld *bullet;	//!< 物理演算ワールド
	float gravity;			//!< 重力

public:
	PhysicsLayer3D();
	virtual ~PhysicsLayer3D();
	
	/**
	 * Figureを追加します.
	 * 追加したFigureはこのオブジェクトと共に解放されます.
	 * @param set オブジェクト
	 * @param option オプション
	 */
	void addFigureSet(PhysicsFigureSet *set, RigidBodyOption &option);

	//////////////////////////////////////////////////////////
	// Layer の実装
	//////////////////////////////////////////////////////////
	/**
	 * セットアップを行います.
	 */
	virtual void setup();
	
	/**
	 * 画面の描画を行います.
	 * @param dt 前回描画からの差分時間
	 */
	virtual void render(double dt = 0.033);
	
	
	//////////////////////////////////////////////////////////
	// IBulletWorldEventHandler の実装
	//////////////////////////////////////////////////////////
	
	/**
	 * 各オブキェクトの処理.
	 */
	virtual void stepBulletObject(BulletWorld *world, btCollisionObject *obj);
	
	/**
	 * 各オブキェクトの衝突処理.
	 */
	virtual void contactBulletObject(BulletWorld *world, btRigidBody *obj0, btRigidBody *obj1);
	
	/**
	 * 各オブキェクトの保存処理.
	 */
	virtual bool saveBulletObject(BulletWorld *world, btRigidBody *body, UserObj *obj, int index, int max);
	
	/**
	 * 各オブキェクトの読み込み処理.
	 */
	virtual bool loadBulletObject(BulletWorld *world, btRigidBody *body, UserObj *obj, int index, int max);

};

#endif /* defined(__GCube__PhysicsLayer3D__) */
