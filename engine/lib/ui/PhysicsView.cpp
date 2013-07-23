//
//  PhysicsView.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/07/13.
//
//

#include "PhysicsView.h"
#include "Box2DManager.h"

PhysicsView::PhysicsView() : ViewGroup() {
	body = NULL;
}

PhysicsView::PhysicsView(GCContext *context) : ViewGroup(context) {
	body = NULL;
}

PhysicsView::~PhysicsView() {
}

void PhysicsView::setBody(b2Body *body) {
	this->body = body;
	
	b2Vec2 point(0, 0);
	body->SetLinearVelocity(linearVelocity);
	body->SetAngularVelocity(angularVelocity);
	body->ApplyForce(force, point);
	body->SetTransform(body->GetPosition(), getRotate() / b2_pi * 180);
}

b2Body *PhysicsView::getBody() {
	return this->body;
}

void PhysicsView::setLinearVelocity(float x, float y) {
	linearVelocity.Set(x, y);
	if (body) {
		body->SetLinearVelocity(linearVelocity);
	}
}

void PhysicsView::setAngularVelocity(float w) {
	angularVelocity = w;
	if (body) {
		body->SetAngularVelocity(angularVelocity);
	}
}

void PhysicsView::applyForce(float forceX, float forceY) {
	force.Set(forceX, forceY);
	if (body) {
		b2Vec2 point(0, 0);
		body->ApplyForce(force, point);
	}
}


//////////////////////////////////////////////////////////////
// Viewからの継承
//////////////////////////////////////////////////////////////

void PhysicsView::setPosition(Pointf& point) {
	this->setPosition(point.x, point.y);
}

void PhysicsView::setPosition(float x, float y) {
	this->point.x = x;
	this->point.y = y;
	if (body) {
		float32 angle = body->GetAngle();
		b2Vec2 position(x, y);
		body->SetTransform(position, angle);
	}
}

void PhysicsView::setRotate(float rotate) {
	this->rotate = rotate;
	if (body) {
		b2Vec2 position = body->GetPosition();
		float32 angle = angle / b2_pi * 180;
		body->SetTransform(position, angle);
	}
}

void PhysicsView::draw(double dt, ViewContext *context) {
	if (body) {
		b2Vec2 pos = body->GetPosition();
		float angle = body->GetAngle();
		
		pos.x = pos.x / PPM;
		pos.y = pos.y / PPM;
		
		this->point.x = pos.x;
		this->point.y = pos.y;
		this->rotate = angle / 180 * b2_pi;
	}
	ViewGroup::draw(dt, context);
}


