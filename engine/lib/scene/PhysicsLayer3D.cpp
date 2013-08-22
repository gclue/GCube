//
//  PhysicsLayer3D.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/08/21.
//
//

#include "PhysicsLayer3D.h"

PhysicsFigureSet::PhysicsFigureSet() : FigureSet() {
	body = NULL;
}

PhysicsFigureSet::~PhysicsFigureSet() {
	
}


///////////////////////////////////////////////////////////////////////////


PhysicsLayer3D::PhysicsLayer3D() : Layer3D() {
	gravity = -9.8;
	bullet = NULL;
}

PhysicsLayer3D::~PhysicsLayer3D() {
	DELETE(bullet);
}

void PhysicsLayer3D::addFigureSet(PhysicsFigureSet *set, RigidBodyOption &option)
{
	if (option.type != RigidBodyType_None) {
		if (!bullet) {
			bullet = new BulletWorld(gravity);
			bullet->setEventHandler(this);
		}
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
				set->body = bullet->addMeshShape(option.x, option.y, option.z, set->getFigure(), option.mass, option.restitution, option.friction, option.isKinematic);
				break;
		}
		if (set->body) {
			set->body->setUserPointer(set);
		}
	}
	
	Layer3D::addFigureSet(set);
}

//////////////////////////////////////////////////////////
// Layer の実装
//////////////////////////////////////////////////////////

void PhysicsLayer3D::setup() {
	
}

void PhysicsLayer3D::render(double dt) {
	lockflag = true;

	if (bullet) {
		// 位置変更があった場合はBulletに反映（TODO:スケーリングの考慮も必要）
		float mat[16];
		int size = figures.size();
		for (int i = 0; i < size; i++) {
			FigureSet *set = figures.at(i);
//			if (set->getMatrix()->dirtyflag && set->body) {
//				// 移動
//				btTransform transform;
//				set->getMatrix()->getElements(mat);
//				transform.setFromOpenGLMatrix(mat);
//				// kinematic
//				if (set->body->isStaticOrKinematicObject()) {
//					set->body->setCollisionFlags(set->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
//					set->body->setActivationState(DISABLE_DEACTIVATION);
//					if (set->body->getMotionState()) {
//						set->body->getMotionState()->setWorldTransform(transform);
//					} else {
//						set->body->setWorldTransform(transform);
//					}
//				} else {
//					set->body->setWorldTransform(transform);
//				}
//				set->body->activate(true);
//			}
		}
		// 物理演算
		bullet->step(dt);
	}
	Layer3D::render(dt);
	lockflag = false;
}

//////////////////////////////////////////////////////////
// IBulletWorldEventHandler の実装
//////////////////////////////////////////////////////////

/**
 * 各オブキェクトの処理.
 */
void PhysicsLayer3D::stepBulletObject(BulletWorld *world, btCollisionObject *obj) {
}

/**
 * 各オブキェクトの衝突処理.
 */
void PhysicsLayer3D::contactBulletObject(BulletWorld *world, btRigidBody *obj0, btRigidBody *obj1) {
}

/**
 * 各オブキェクトの保存処理.
 */
bool PhysicsLayer3D::saveBulletObject(BulletWorld *world, btRigidBody *body, UserObj *obj, int index, int max) {
	return true;
}

/**
 * 各オブキェクトの読み込み処理.
 */
bool PhysicsLayer3D::loadBulletObject(BulletWorld *world, btRigidBody *body, UserObj *obj, int index, int max) {
	return true;
}

