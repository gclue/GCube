//
//  Box2DManager.cpp
//  GCubeHello
//
//  Created by Hitoshi Haga on 12/04/05.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#include <iostream>
#include "Box2DManager.h"

Box2DManager::Box2DManager(float ppm) {
	this->ppm = ppm;
	b2Vec2 gravity(0, -9.8);
	world = new b2World(gravity);
}

Box2DManager::~Box2DManager() {
	delete world;
}

void Box2DManager::removeBody(b2Body *body) {
	world->DestroyBody(body);
}



b2Body* Box2DManager::addBox(PhysicsParams param) {
	//ボディの定義
	b2BodyDef def;
	def.position.Set(param.position.x * ppm, param.position.y * ppm);	//配置する場所
	def.type = param.type;
	
	
	//シェイプの定義
	b2PolygonShape shape = createBoxShape(param.size.x * ppm, param.size.y * ppm);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = param.density;
	fixtureDef.friction = param.friction;
	fixtureDef.restitution = param.restitution;
	
	
	
	b2Body* body = world->CreateBody(&def);
	body->CreateFixture(&fixtureDef);
	body->ResetMassData();
	
	
	return body;
}



b2PolygonShape Box2DManager::createBoxShape(float width, float height) {
	b2PolygonShape shape;
	shape.SetAsBox(width/2.0, height/2.0);
	return shape;
}



void Box2DManager::step(double dt) {
	int velocityIterations = 6;
	int positionIterations = 2;
	world->Step(dt, velocityIterations, positionIterations);
	
	world->ClearForces();
}