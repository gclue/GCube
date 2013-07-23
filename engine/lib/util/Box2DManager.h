//
//  Box2DManager.h
//  GCubeHello
//
//  Created by Hitoshi Haga on 12/04/05.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#ifndef GCubeHello_Box2DManager_h
#define GCubeHello_Box2DManager_h

#include "Box2D.h"
#include "defines.h"
#include <string>

/**
 * Box2Dの距離を合わせるための定数。
 */
#define PPM 10.0

/**
 * ジョイントのタイプを定義.
 */
enum {
	JOINT_DISTANCE = 0,
	JOINT_REVOLUTE,
	JOINT_PRISMATIC,
	JOINT_PULLEY,
	JOINT_GEAR,
};

/**
 * SHAPEのタイプを定義.
 */
enum {
	SHAPE_TYPE_BOX = 0,
	SHAPE_TYPE_CIRCLE,
	SHAPE_TYPE_SHAPE,
	SHAPE_TYPE_NONE,
};

/**
 * ジョイントのパラメータ.
 */
struct PhysicsJointParams {
	int jointType;
	float x1;
	float y1;
	float x2;
	float y2;
	
	PhysicsJointParams() {
		jointType = JOINT_DISTANCE;
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
	}
};

/**
 * Physicsのパラメータ.
 */
struct PhysicsParams {
	int shapeType;
	float density;
	float friction;
	float restitution;
	float angle;
	float radius;
	
	bool isSensor;
	
	std::string filename;
	
	int id;
	
	b2BodyType type;
	b2Vec2 size;
	b2Vec2 position;
	
	PhysicsParams() {
		shapeType = SHAPE_TYPE_BOX;
		density = 1.0;
		friction = 0.3;
		restitution = 0.3;
		angle = 0;
		radius = 0;
		
		isSensor = false;
		
		id = -1;
		
		type = b2_dynamicBody;
		position.Set(0, 0);
		size.Set(1, 1);
	}
};

/**
 * Box2Dを管理するクラス.
 */
class Box2DManager {
private:
	b2World *world;	//!< box2dのインスタンス.
	
	b2PolygonShape createBoxShape(float width, float height);
	
public:
	Box2DManager();
	virtual ~Box2DManager();
	
	/**
	 * box2dのインスタンスを取得します.
	 * @return box2dのインスタンス
	 */
	b2World* getWorld();
	
	/**
	 * box2dにboxを追加します.
	 * boxの追加に失敗した場合にはNULLを返却します。
	 * @param[in] param パラメータ
	 * @return b2Body
	 */
	b2Body* addBox(PhysicsParams &param);
	
	/**
	 * box2dにcircleを追加します.
	 * circleの追加に失敗した場合にはNULLを返却します。
	 * @param[in] param パラメータ
	 * @return b2Body
	 */
	b2Body* addCircle(PhysicsParams &param);
	
	/**
	 * box2dにshapeを追加します.
	 * shapeの追加に失敗した場合にはNULLを返却します。
	 * @param[in] param パラメータ
	 * @param[in] json PhysicsEditorで作成したデータ
	 * @return b2Body
	 */
	b2Body* addBody(PhysicsParams &param, const char *json);
	
	/**
	 * 指定されたBodyをbox2dから削除します.
	 * @param[in] body 削除するbody
	 */
	void removeBody(b2Body *body);
	
	/**
	 * box2dをステップ実行します.
	 * @parma[in] dt 前回からの経過時間
	 */
	void step(double dt);
};

#endif
