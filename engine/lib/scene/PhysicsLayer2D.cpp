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
#include "ApplicationController.h"
#include "PhysicsView.h"

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

PhysicsLayer2D::PhysicsLayer2D() : Layer() {
	init();
}

PhysicsLayer2D::PhysicsLayer2D(GCContext *context) : Layer(context) {
	init();
}

PhysicsLayer2D::~PhysicsLayer2D() {
	LOGD("~PhysicsLayer2D ");
	removeAllViews();
	removeViewList.clear();
	DELETE(b2Mgr);
	DELETE(draw);
	LOGD("~PhysicsLayer2D end");
}

void PhysicsLayer2D::init()
{
	b2Mgr = new Box2DManager();
	
	ApplicationController *ctl = ApplicationController::getInstance();
	viewcontext.shader = ctl->shader;
	viewcontext.camera = ctl->camera;
	
	canStepPhysics = true;
	removeFlag = false;
	debugFlag = false;
	
	// デバック用
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
//	flags += b2Draw::e_centerOfMassBit;
//	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	
	draw = new DebugDraw();
	draw->setCamera(ctl->camera);
	draw->SetFlags(flags);
	getWorld()->SetDebugDraw(draw);
}

void PhysicsLayer2D::setDebugFlag(bool flag)
{
	debugFlag = flag;
}

void PhysicsLayer2D::setCamera(Camera *camera)
{
	viewcontext.camera = camera;
	draw->setCamera(camera);
}

b2World* PhysicsLayer2D::getWorld()
{
	return b2Mgr->getWorld();
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

void PhysicsLayer2D::addView(PhysicsView *view, PhysicsParams& param)
{
	if (view) {
		b2Body* body = NULL;
		
		switch (param.shapeType) {
			default:
			case SHAPE_TYPE_BOX: {
				float width = view->size.x * 2.0;
				float height = view->size.y * 2.0;
				
				param.position.x = view->point.x;
				param.position.y = view->point.y;
				param.size.x = width;
				param.size.y = height;
				
				body = b2Mgr->addBox(param);
			}	break;
			case SHAPE_TYPE_CIRCLE: {
				float width = view->size.x * 2.0;
				float height = view->size.y * 2.0;
				
				param.position.x = view->point.x;
				param.position.y = view->point.y;
				param.size.x = width;
				param.size.y = height;
				param.radius = width;
				
				body = b2Mgr->addCircle(param);
			}	break;
			case SHAPE_TYPE_SHAPE: {
				AssetManager mgr = AssetManager::getInstance();
				std::vector<char>* fdata = mgr.open(param.filename.c_str());
				if (!fdata) return ;
				fdata->push_back('\0');
				const char *json = (const char *) &(*fdata)[0];
				body = b2Mgr->addBody(param, json);
				delete fdata;
			}	break;
			case SHAPE_TYPE_NONE:
				break;
		}
		
		body->SetUserData(view);
		view->setBody(body);
		view->retain();
		
		views.push_back(view);
	}
}

void PhysicsLayer2D::addJoint(PhysicsView *viewA, PhysicsView *viewB, PhysicsJointParams& param)
{
	// TODO 未実装
	b2Body *bA = viewA->getBody();
	b2Body *bB = viewB->getBody();
	
	switch (param.jointType) {
		case JOINT_DISTANCE:
			break;
	}
	
	float x1, y1;
	float x2, y2;
	
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
}

void PhysicsLayer2D::removeView(PhysicsView *view)
{
	// 削除するのはstepPhyicsの中で行うので、ここでは削除を登録するだけ
	removeViewList.push_back(view);
}

void PhysicsLayer2D::removeView(int userId)
{
	PhysicsView *view = (PhysicsView *) findViewById(userId);
	if (view) {
		removeView(view);
	}
}

void PhysicsLayer2D::removeAllViews()
{
	removeFlag = true;
	
	std::list<PhysicsView*>::iterator it = views.begin();
	while (it != views.end()) {
		PhysicsView *v = (*it);
		b2Mgr->removeBody(v->getBody());
		v->setBody(NULL);
		v->setParent(NULL);
		RELEASE(v);
		views.erase(it++);
	}
	
	removeFlag = false;
}

View *PhysicsLayer2D::findViewById(int userId)
{
	std::list<PhysicsView*>::iterator it = views.begin();
	while (it != views.end()) {
		PhysicsView *v = (*it);
		View *view = v->findViewByID(userId);
		if (view) {
			return view;
		}
		it++;
	}
	return NULL;
}

void PhysicsLayer2D::stepPhysics(float dt)
{
	while (!removeViewList.empty()) {
		PhysicsView* view = removeViewList.front();
		
		std::list<PhysicsView *>::iterator it = views.begin();
		while (!views.empty() && it != views.end()) {
			PhysicsView *a = *it;
			if (a == view) {
				it = views.erase(it);
				b2Mgr->removeBody(a->getBody());
				a->setParent(NULL);
				a->setBody(NULL);
				RELEASE(a);
			} else {
				it++;
			}
		}
		
		removeViewList.pop_front();
	}
	
	if (isStepPhysics()) {
		b2Mgr->step(dt);
	}
	
	if (visible) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		if (viewcontext.shader) {
			viewcontext.shader->useProgram();
		}
		
		std::list<PhysicsView*>::iterator it = views.begin();
		while (it != views.end()){
			PhysicsView *v = (*it);
			v->render(dt, &viewcontext);
			it++;
		}
	}
}


//////////////////////////////////////////////////////////
// Layer の実装
//////////////////////////////////////////////////////////

void PhysicsLayer2D::setup() {
	
}

void PhysicsLayer2D::resize(float aspect) {
	
}

void PhysicsLayer2D::render(double dt) {
	if (!visible || removeFlag) {
		return;
	}
	
	stepPhysics(dt);
	
	if (debugFlag) {
		getWorld()->DrawDebugData();
	}
}

bool PhysicsLayer2D::onTouchEvent(TouchEvent &event) {
	if (touchable && visible) {
		std::list<PhysicsView*>::iterator it = views.begin();
		while (it != views.end()){
			PhysicsView *v = (*it);
			v->onTouch(event);
			it++;
		}
	}
	return false;
}
