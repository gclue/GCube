//
//  PhysicsLayer2D.h
//  GCubeHello
//
//  Created by Hitoshi Haga on 12/04/05.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#ifndef GCubeHello_PhysicsLayer2D_h
#define GCubeHello_PhysicsLayer2D_h

#include "Layer.h"
#include "Box2DManager.h"
#include <vector>
#include <map>
#include <list>


class DebugDraw;

class PhysicsUserData {
public:
	std::vector<View*> view;
	int id;
	int userId;
	PhysicsUserData() { id = -1; userId = -1; }
	virtual ~PhysicsUserData() {}
};

class PhysicsLayerInfo {
public:
	b2Body *body;
	int id;
	int groupID;
	
	PhysicsLayerInfo(b2Body *body, int id) {
		this->body = body;
		this->id = id;
		groupID = -1;
	}
	virtual ~PhysicsLayerInfo() {};
};


class PhysicsLayer2D : public Layer {
protected:
	Box2DManager *b2Manager;
	
	DebugDraw *draw;
	
	ViewContext viewcontext;
	
	std::map<unsigned long, PhysicsLayerInfo*> bodies;
	std::list<b2Body*> removeList;
	
	int addBodyList(b2Body *body);
	
	int bodyCount;
	float ppm;
	bool canStepPhysics;
	bool removeFlag;
	
	bool debugFlag;
	
	void stepPhysics(float dt);
	void onDraw(float dt);
	
public:
	PhysicsLayer2D(GCContext* context);
	virtual ~PhysicsLayer2D();
	
	b2World* getWorld();
	
	int addBody(View *view, PhysicsParams& param);
	int addBody(View *view, PhysicsParams& param, const char *filename);
	
	int addCircleBody(View *view, PhysicsParams& param);
	
	int addJoint(int bodyA, int bodyB, float x1, float y1, float x2, float y2);
	
	b2Body* getBody(int id);
	
	void setCamera(Camera *camera);
	
	void removeBody(int id);
	void removeBody(b2Body *body);
	void removeAllBodies();
	
	void pausePhysics();
	void restartPhysics();
	bool isStepPhysics();
	
	//////////////////////////////////////////////////////////
	// Layer の実装
	//////////////////////////////////////////////////////////
	/**
	 * セットアップを行います.
	 */
	virtual void setup();
	
	/**
	 * 画面のリサイズを行います.
	 * @param aspect 画面のアスペクト比
	 */
	virtual void resize(float aspect);
	
	/**
	 * 画面の描画を行います.
	 * @param dt 前回描画からの差分時間
	 */
	virtual void render(double dt = 0.033);
};

#endif
