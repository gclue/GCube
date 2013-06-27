//
//  Box2DManager.cpp
//  GCubeHello
//
//  Created by Hitoshi Haga on 12/04/05.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#include <iostream>
#include "Box2DManager.h"
#include "picojson.h"
#include "log.h"
#include "ApplicationController.h"

Box2DManager::Box2DManager(float ppm) {
	this->ppm = ppm;
	
	b2Vec2 gravity(0, -9.8);
	world = new b2World(gravity);
}

Box2DManager::~Box2DManager() {
	delete world;
}

b2World* Box2DManager::getWorld()
{
	return world;
}

void Box2DManager::removeBody(b2Body *body) {
	world->DestroyBody(body);
}

b2Body* Box2DManager::addBox(PhysicsParams &param) {
	//ボディの定義
	b2BodyDef def;
	def.position.Set(param.position.x * ppm, param.position.y * ppm);	//配置する場所
	def.angle = param.angle;
	def.type = param.type;
	if (param.angle == -1) {
		def.fixedRotation = true;
	}
	
	//シェイプの定義
	b2PolygonShape shape = createBoxShape(param.size.x * ppm, param.size.y * ppm);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = param.density;
	fixtureDef.friction = param.friction;
	fixtureDef.restitution = param.restitution;
	fixtureDef.isSensor = param.isSensor;
	
	b2Vec2 pos(param.position.x * ppm, param.position.y * ppm);
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

b2Body* Box2DManager::addCircle(PhysicsParams &param)
{
	//ボディの定義
	b2BodyDef def;
	def.position.Set(param.position.x * ppm, param.position.y * ppm);	//配置する場所
	def.angle = param.angle;
	def.type = param.type;
	
	//シェイプの定義
	b2CircleShape shape;
	shape.m_radius = param.radius;
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = param.density;
	fixtureDef.friction = param.friction;
	fixtureDef.restitution = param.restitution;
	fixtureDef.isSensor = param.isSensor;
		
	b2Vec2 pos(param.position.x * ppm, param.position.y * ppm);
	b2Body* body = world->CreateBody(&def);
	body->CreateFixture(&fixtureDef);
	body->ResetMassData();
	
	return body;
}

b2Body* Box2DManager::addBody(PhysicsParams &param, const char *json)
{
	float dw = ApplicationController::getInstance()->getHeight() / 2.0;
	std::string err;
	picojson::value v;
	picojson::parse(v, json, json + strlen(json), &err);
	if (err.empty()) {
		b2Vec2 vertices[b2_maxPolygonVertices];
		b2BodyDef bodyDef;
		bodyDef.position.Set(param.position.x * ppm, param.position.y * ppm);
		bodyDef.angle = param.angle;
		bodyDef.type = param.type;
		
		b2Body* body = world->CreateBody(&bodyDef);
		
		int width = param.size.x / 2;
		int height = param.size.y / 2;
		
		picojson::object root = v.get<picojson::object>();
		picojson::object::iterator it;
		for (it = root.begin(); it != root.end(); it++) {
			picojson::array array = it->second.get<picojson::array>();
			picojson::array::iterator it2;
			for (it2 = array.begin(); it2 != array.end(); it2++) {
				picojson::object obj = it2->get<picojson::object>();
				
				picojson::array shapeArray = obj["shape"].get<picojson::array>();
				int count = shapeArray.size() / 2;
				if (count < 3 || count >= b2_maxPolygonVertices) {
					LOGE("ERROR.");
				}
				
				int i = 0;
				picojson::array::iterator it3;
				for (it3 = shapeArray.begin(); it3 != shapeArray.end();) {
					float x = (it3->get<double>() - width) / dw;
					it3++;
					float y = (it3->get<double>() - height) / dw;
					it3++;
					vertices[i].Set(x * ppm, y * ppm);
					i++;
				}
				
				b2PolygonShape shape;
				shape.Set(vertices, count);
				
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = param.density;
				fixtureDef.friction = param.friction;
				fixtureDef.restitution = param.restitution;
				fixtureDef.isSensor = param.isSensor;
				
				body->CreateFixture(&fixtureDef);
			}
		}
		return body;
	}
	return NULL;
}


void Box2DManager::step(double dt) {
	int velocityIterations = 6;
	int positionIterations = 2;
	world->Step(dt, velocityIterations, positionIterations);
	world->ClearForces();
}
