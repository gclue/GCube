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


struct PhysicsParams {

	float density;
	float friction;
	float restitution;
	
	b2Vec2 size;
	b2Vec2 position;
	
	b2BodyType type;
	
	
	PhysicsParams() {
		density = 1.0;
		friction = 0.3;
		restitution = 0.3;
	
		type = b2_dynamicBody;
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
	
	b2Body* addBox(PhysicsParams param);
	
	void removeBody(b2Body *body);
	
	void step(double dt);
	
};

#endif
