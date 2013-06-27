//
//  PhysicsLayer2D.cpp
//  GCubeHello
//
//  Created by Hitoshi Haga on 12/04/05.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <list>

#include "PhysicsLayer2D.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "AssetManager.h"


class DebugDraw : public b2Draw {
private:
	Shader *shader;
	Camera *camera;
	
public:
	DebugDraw() {
		shader = NULL;
		camera = NULL;
	}
	~DebugDraw() {
		delete shader;
	}
	
	void setCamera(Camera *camera);
	
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	virtual void DrawTransform(const b2Transform& xf);
};

void DebugDraw::setCamera(Camera *camera)
{
	this->camera = camera;
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	
}
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	
}
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	
}
void DebugDraw::DrawTransform(const b2Transform& xf)
{
}



PhysicsLayer2D::PhysicsLayer2D(GCContext *context) : Layer(context) {
	ppm = 2;
	b2Manager = new Box2DManager(ppm);
	
	viewcontext.shader = context->shader;
	viewcontext.camera = context->camera;

	bodyCount = 0;
	canStepPhysics = true;
	removeFlag = false;
	debugFlag = false;
	
	// デバック用
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_centerOfMassBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	
	draw = new DebugDraw();
	draw->setCamera(context->camera);
	draw->SetFlags(flags);
	getWorld()->SetDebugDraw(draw);
}

PhysicsLayer2D::~PhysicsLayer2D() {
	LOGD("~PhysicsLayer2D ");
	removeAllBodies();
	delete b2Manager;
	delete draw;
	LOGD("~PhysicsLayer2D end");
}

void PhysicsLayer2D::setCamera(Camera *camera)
{
	viewcontext.camera = camera;
}

b2World* PhysicsLayer2D::getWorld()
{
	return b2Manager->getWorld();
}

void PhysicsLayer2D::removeAllBodies() {
	removeFlag = true;
	
	std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.begin();
	while (it != bodies.end()) {
		b2Body *body = ((*it).second)->body;
		PhysicsUserData *data = (PhysicsUserData *) body->GetUserData();
		std::vector<View*> views =  data->view;
		for (int i = 0 ; i < views.size() ; i++) {
			if (views[i]) {
				views[i]->release();
				views[i] = NULL;
			}
		}

		b2Manager->removeBody(body);
		
		delete data;
		delete ((*it).second);
		
		bodies.erase(it++);
	}
	bodyCount = 0;
	
	removeFlag = false;
}

int PhysicsLayer2D::addBody(View *view, PhysicsParams &param)
{
	if (view) {
		view->retain();
	}
	
	float width = view->size.x * 2.0;
	float height = view->size.y * 2.0;
	
	param.position.x = view->point.x;
	param.position.y = view->point.y;
	param.size.x = width;
	param.size.y = height;
	
	b2Body* body = b2Manager->addBox(param);
	
	int setId = addBodyList(body);
	
	PhysicsUserData* userData = new PhysicsUserData();
	userData->view.push_back(view);
	userData->id = setId;
	userData->userId = param.id;
	body->SetUserData(userData);
	
	PhysicsLayerInfo *data = new PhysicsLayerInfo(body, setId);
	bodies[setId] = data;
	return setId;
}

int PhysicsLayer2D::addBody(View *view, PhysicsParams& param, const char *filename)
{
	if (view) {
		view->retain();
	}
	
	AssetManager mgr = AssetManager::getInstance();
	std::vector<char>* fdata = mgr.open(filename);
	if (!fdata) return -1;
	fdata->push_back('\0');
	const char *json = (const char *) &(*fdata)[0];
	
	b2Body* body = b2Manager->addBody(param, json);
	delete fdata;
	if (body) {
		int setId = addBodyList(body);
		
		PhysicsUserData* userData = new PhysicsUserData();
		userData->view.push_back(view);
		userData->id = setId;
		userData->userId = param.id;
		body->SetUserData(userData);
		
		PhysicsLayerInfo *data = new PhysicsLayerInfo(body, setId);
		bodies[setId] = data;
		return setId;
	}
	return -1;
}

int PhysicsLayer2D::addCircleBody(View *view, PhysicsParams& param)
{
	if (view) {
		view->retain();
	}
	float width = view->size.x * 2.0;
	float height = view->size.y * 2.0;
	
	param.position.x = view->point.x;
	param.position.y = view->point.y;
	param.size.x = width;
	param.size.y = height;
	param.radius = width;
	
	b2Body* body = b2Manager->addCircle(param);
	
	int setId = addBodyList(body);
	
	PhysicsUserData* userData = new PhysicsUserData();
	userData->view.push_back(view);
	userData->id = setId;
	userData->userId = param.id;
	body->SetUserData(userData);
	
	PhysicsLayerInfo *data = new PhysicsLayerInfo(body, setId);
	bodies[setId] = data;
	return setId;

}


int PhysicsLayer2D::addJoint(int bodyA, int bodyB, float x1, float y1, float x2, float y2)
{
	b2Body *bA = getBody(bodyA);
	b2Body *bB = getBody(bodyB);
	
	/*
	b2Vec2 b1(x1, y1);
	b2RevoluteJointDef RD;
	RD.Initialize(bA, bB, bB->GetWorldCenter() - b1);
	RD.collideConnected = false;
	RD.enableLimit = false;
	RD.enableMotor = false;
	
	getWorld()->CreateJoint(&RD);
	 */
	
	b2Vec2 b1(x1, y1);
	b2Vec2 b2(x2, y2);
	b2DistanceJointDef DJ; 
	DJ.Initialize(bA, bB, bA->GetWorldCenter()+b1, bB->GetWorldCenter()-b2);
	DJ.collideConnected = false;
	DJ.frequencyHz = 2.5;
	DJ.dampingRatio = 1;
	DJ.length = 0.00;
	
	getWorld()->CreateJoint(&DJ);

	return 0;
}

b2Body* PhysicsLayer2D::getBody(int id)
{
	std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.begin();
	while (it != bodies.end()){
		PhysicsLayerInfo* info = (*it).second;
		if (info) {
			if (info->id == id) {
				return ((*it).second)->body;
			}
		}
		it++;
	}
	return NULL;
}



void PhysicsLayer2D::removeBody(b2Body *body) {
	std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.begin();
	while (it != bodies.end()) {
		PhysicsLayerInfo* info = (*it).second;
		if (info) {
			if (info->body == body) {
				b2Body *b = ((*it).second)->body;
				PhysicsUserData *data = (PhysicsUserData *) b->GetUserData();
				std::vector<View*> views =  data->view;
				for (int i = 0; i < views.size(); i++) {
					if(views[i]) {
						views[i]->release();
						views[i] = NULL;
					}
				}
				
				b->SetUserData(NULL);
				removeList.push_front(b);
				bodies.erase(it);
				bodyCount--;
				delete data;
				delete info;
				break;
			}
		}
		it++;
	}

}

void PhysicsLayer2D::removeBody(int id) {
	std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.begin();
	while (it != bodies.end()) {
		PhysicsLayerInfo* info = (*it).second;
		if (info) {
			if (info->id == id) {
				b2Body *body = ((*it).second)->body;
				PhysicsUserData *data = (PhysicsUserData *) body->GetUserData();
				std::vector<View*> views =  data->view;
				for (int i = 0; i < views.size(); i++) {
					if(views[i]) {
						views[i]->release();
						views[i] = NULL;
					}
				}
				
				body->SetUserData(NULL);
				removeList.push_front(body);
				bodies.erase(it);
				bodyCount--;
				delete data;
				delete info;
				break;
			}
		}
		it++;
	}
}

int PhysicsLayer2D::addBodyList(b2Body *body) {
	int setID;
	while (true) {
		std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.find(++bodyCount);
		if (it==bodies.end()) {
			break;
		}
	}
	setID = bodyCount;
	return setID;
}


void PhysicsLayer2D::pausePhysics() {
	canStepPhysics = false;
}

void PhysicsLayer2D::restartPhysics() {
	canStepPhysics = true;
}

bool PhysicsLayer2D::isStepPhysics() {
	return canStepPhysics;
}

void PhysicsLayer2D::stepPhysics(float dt)
{
	while (removeList.size() > 0) {
		b2Body* body = removeList.front();
		b2Manager->removeBody(body);
		removeList.pop_front();
	}
	
	if (isStepPhysics()) {
		b2Manager->step(dt);
	}
}

void PhysicsLayer2D::onDraw(float dt)
{
	context->shader->useProgram();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.begin();
	while (it != bodies.end()){
		if ((*it).second) {
			b2Body *body = ((*it).second)->body;
			b2Vec2 pos = body->GetPosition();
			float angle = body->GetAngle();
			
			pos.x = pos.x / ppm;
			pos.y = pos.y / ppm;
			
			std::vector<View*> views = ((PhysicsUserData *) body->GetUserData())->view;
			
			if (views.size() == 1) {
				View *view;
				view = views[0];
				if (view) {
					view->point.x = pos.x;
					view->point.y = pos.y;
					view->setRotate(angle / b2_pi * 180);
					if (context->shader) {
						context->shader->useProgram();
					}
					view->render(dt, &viewcontext);
				}
			} else {
				if (views.size() > 1) {
					int count = 0;
					View *view;
					b2Fixture *fix = body->GetFixtureList();
					while (fix) {
						view = views[count];
						if (view) {
							view->point.x = pos.x;
							view->point.y = pos.y;
							view->setRotate(angle / b2_pi * 180);
							if (context->shader) {
								context->shader->useProgram();
							}
							view->render(dt, &viewcontext);
						}
						fix = fix->GetNext();
						count++;
					}
				}
			}
		}
		it++;
	}
}

//////////////////////////////////////////////////////////
// Layer の実装
//////////////////////////////////////////////////////////
/**
 * セットアップを行います.
 */
void PhysicsLayer2D::setup() {
	
}

/**
 * 画面のリサイズを行います.
 * @param aspect 画面のアスペクト比
 */
void PhysicsLayer2D::resize(float aspect) {
	
}

/**
 * 画面の描画を行います.
 * @param dt 前回描画からの差分時間
 */
void PhysicsLayer2D::render(double dt) {
	if (!visible) {
		return;
	}
	
	if (removeFlag) {
		return;
	}
	
	stepPhysics(dt);
		
	onDraw(dt);
	
	if (debugFlag) {
		getWorld()->DrawDebugData();
	}
}
