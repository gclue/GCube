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

struct PhysicsParams {
	float density;
	float friction;
	float restitution;
	float angle;
	float radius;
	
	bool isSensor;
	
	int id;
	
	b2Vec2 size;
	b2Vec2 position;
	
	b2BodyType type;
	
	PhysicsParams() {
		density = 1.0;
		friction = 0.3;
		restitution = 0.3;
		angle = 0;
		radius = 0;
		
		type = b2_dynamicBody;
		
		id = -1;
		
		isSensor = false;
	}
};

class Box2DManager {
private:
	b2World *world;
	float ppm;
	
	b2PolygonShape createBoxShape(float width, float height);
	
public:
	Box2DManager(float ppm);
	virtual ~Box2DManager();
	
	b2World* getWorld();
	
	b2Body* addBox(PhysicsParams &param);
	b2Body* addCircle(PhysicsParams &param);
	b2Body* addBody(PhysicsParams &param, const char *json);
	
	void removeBody(b2Body *body);
	
	void step(double dt);
};

#endif
