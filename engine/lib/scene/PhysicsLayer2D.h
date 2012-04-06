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


class PhysicsUserData {
public:
	PhysicsUserData(){
		id = -1;
	};
	
	~PhysicsUserData() {
		
	};
	
	std::vector<View*> view;
	int id;
};

class PhysicsLayerInfo {
	
public:
	PhysicsLayerInfo(b2Body *body, int id){
		this->body = body;
		this->id =id;
		groupID = -1;
	}
	~PhysicsLayerInfo(){};
	
	
	b2Body *body;
	int id;
	int groupID;
	
};


class PhysicsLayer2D : public Layer {
	
	
private:
	Box2DManager *b2Manager;
	
	std::map<unsigned long, PhysicsLayerInfo*> bodies;
	std::list<b2Body*> removeList;
	
	int addBodyList(b2Body *body);
	
	int bodyCount;
	float ppm;
	
public:
	PhysicsLayer2D(GCContext* context);
	virtual ~PhysicsLayer2D();
	
	
	virtual int addBody(View *view, PhysicsParams param);
	
	virtual void removeBody(int id);
	
	
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
	
	/**
	 * タッチイベント.
	 * @param event タッチイベント
	 * @return true: 次のレイヤーにイベントを渡さない、false: 次のレイヤーにイベントを渡す
	 */
	virtual bool onTouch(TouchEvent &event);
	
};

#endif
