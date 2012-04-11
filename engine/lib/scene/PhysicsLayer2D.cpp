//
//  PhysicsLayer2D.cpp
//  GCubeHello
//
//  Created by Hitoshi Haga on 12/04/05.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#include <iostream>
#include "PhysicsLayer2D.h"
#include "Camera.h"
#include "SimpleShader.h"

PhysicsLayer2D::PhysicsLayer2D(GCContext *context) : Layer(context) {
	ppm = 2;
	b2Manager = new Box2DManager(ppm);
	
	bodyCount = 0;
	canStepPhysics = true;
	removeFlag = false;
}


PhysicsLayer2D::~PhysicsLayer2D() {
	delete b2Manager;
	
	
	std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.begin();
	while(it != bodies.end() ){
		b2Body *body = ((*it).second)->body;
		PhysicsUserData *data = (PhysicsUserData*)body->GetUserData();
		std::vector<View*> views =  data->view;
		for(int i = 0 ; i < views.size() ; i++) {
			if(views[i]) {
				views[i]->release();
				views[i] = NULL;
			}
		}
		
		
		it++;
	}
}



void PhysicsLayer2D::removeAllBodies() {
	removeFlag = true;
	
	std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.begin();
	while(it != bodies.end() ){
		b2Body *body = ((*it).second)->body;
		PhysicsUserData *data = (PhysicsUserData*)body->GetUserData();
		std::vector<View*> views =  data->view;
		for(int i = 0 ; i < views.size() ; i++) {
			if(views[i]) {
				views[i]->release();
				views[i] = NULL;
			}
		}
		delete data;
		b2Manager->removeBody(body);
		
		bodies.erase(it);
		it++;
	}
	bodyCount = 0;
	
	removeFlag = false;
	
}


int PhysicsLayer2D::addBody(View *view, PhysicsParams param) {
	
	if(view) {
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
	body->SetUserData(userData);
	
	PhysicsLayerInfo *data = new PhysicsLayerInfo(body, setId);
	bodies[setId] = data;
	return setId;
}




void PhysicsLayer2D::removeBody(int id) {
	std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.begin();
	while(it != bodies.end() ){
		PhysicsLayerInfo* info = (*it).second;
		if(info) {
			if(info->id == id) {
				b2Body *body = ((*it).second)->body;
				PhysicsUserData *data = (PhysicsUserData*)body->GetUserData();
				std::vector<View*> views =  data->view;
				for(int i = 0 ; i < views.size() ; i++) {
					if(views[i]) {
						views[i]->release();
						views[i] = NULL;
					}
				}
				
				delete data;
				body->SetUserData(NULL);
				removeList.push_front(body);
				bodies.erase(it);
				bodyCount--;
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



//////////////////////////////////////////////////////////
// Layer の実装
//////////////////////////////////////////////////////////
/**
 * セットアップを行います.
 */
void PhysicsLayer2D::setup(){
	
}

/**
 * 画面のリサイズを行います.
 * @param aspect 画面のアスペクト比
 */
void PhysicsLayer2D::resize(float aspect){
	
}

/**
 * 画面の描画を行います.
 * @param dt 前回描画からの差分時間
 */
void PhysicsLayer2D::render(double dt){
	
	
	while(removeList.size() > 0) {
		b2Body* body = removeList.front();
		b2Manager->removeBody(body);
		removeList.pop_front();
	}
	
	if(removeFlag) {
		return;
	}
	
	if(isStepPhysics() ) {
		b2Manager->step(dt);
	}
	
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	
	
	std::map<unsigned long, PhysicsLayerInfo*>::iterator it = bodies.begin();
	while(it != bodies.end() ){
		if((*it).second) {
			b2Body *body = ((*it).second)->body;
			b2Vec2 pos = body->GetPosition();
			float angle = body->GetAngle();
			
			
			pos.x = pos.x / ppm;
			pos.y = pos.y / ppm;
			
			std::vector<View*> views = ((PhysicsUserData*) body->GetUserData() )->view;
			
			if(views.size() == 1) {
				View *view;
				view = views[0];
				if (view) {
					view->point.x = pos.x;
					view->point.y = pos.y;
					view->setRotate(angle /b2_pi * 180);
					if (context->shader) {
						context->shader->useProgram();
					}
					view->render(dt);
				}
			}else {
				if(views.size() >1) {
					int count = 0;
					View *view;
					b2Fixture *fix = body->GetFixtureList();
					while(fix) {
						//					b2Body *rBody = fix->GetBody();
						view = views[count];
						if (view) {
							view->point.x = pos.x;
							view->point.y = pos.y;
							view->setRotate(angle /b2_pi * 180);
							if (context->shader) {
								context->shader->useProgram();
							}
							view->render(dt);
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

/**
 * タッチイベント.
 * @param event タッチイベント
 * @return true: 次のレイヤーにイベントを渡さない、false: 次のレイヤーにイベントを渡す
 */
bool PhysicsLayer2D::onTouch(TouchEvent &event){
	return false;
}
