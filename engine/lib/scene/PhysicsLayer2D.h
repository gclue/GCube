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
class PhysicsView;

/**
 * Box2Dを管理するためのレイヤー.
 */
class PhysicsLayer2D : public Layer {
protected:
	Box2DManager *b2Mgr;
	
	DebugDraw *draw;
	
	ViewContext viewcontext;
	
	std::list<PhysicsView *> views;
	std::list<PhysicsView *> removeViewList;
	
	bool canStepPhysics;
	bool removeFlag;
	bool debugFlag;
	
	void init();
	void stepPhysics(float dt);
	
public:
	PhysicsLayer2D();
	PhysicsLayer2D(GCContext* context);
	virtual ~PhysicsLayer2D();
	
	/**
	 * Box2Dのインスタンスを取得します.
	 * @return Box2Dのインスタンス
	 */
	b2World* getWorld();
	
	/**
	 * Box2Dのデバックフラグを設定します.
	 * @param[in] flag デバックフラグ
	 */
	void setDebugFlag(bool flag);
	
	/**
	 * レイヤーにカメラを設定します.
	 * @param[in] camera カメラ
	 */
	void setCamera(Camera *camera);
	
	/**
	 * Box2Dを一時停止します.
	 */
	void pausePhysics();
	
	/**
	 * Box2Dを開始します.
	 */
	void restartPhysics();
	
	/**
	 * 現在Box2Dが動作しているかをチェックします.
	 * @return 動作している場合はtrue, それ以外はfalse
	 */
	bool isStepPhysics();
	
	/**
	 * ViewをLayerに追加します.
	 * 追加されたViewはretainします。
	 * @param[in] view 追加するView
	 * @param[in] param box2dに追加するパラメータ
	 */
	void addView(PhysicsView *view, PhysicsParams& param);
	
	/**
	 * viewAとviewBのJointを作成します.
	 * viewAとviewBは既に、LayerにaddViewされていることをが前提です。
	 * また、この関数では、viewAとviewBにはretainしない。
	 * @param[in] viewA
	 * @param[in] viewB
	 * @param[in] param
	 */
	void addJoint(PhysicsView *viewA, PhysicsView *viewB, PhysicsJointParams& param);
	
	/**
	 * 指定されたViewをLayerから削除します.
	 * @param[in] view 削除するView
	 */
	void removeView(PhysicsView *view);
	
	/**
	 * 指定されたIDのViewをLayerから削除します.
	 * @param[in] userId 削除するViewのID
	 */
	void removeView(int userId);
	
	/**
	 * LayerにあるViewをすべて削除します.
	 */
	void removeAllViews();
	
	/**
	 * 指定されたUserIDのViewを探します.
	 * @param[in] userId ユーザID
	 */
	View *findViewById(int userId);
	
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
	virtual bool onTouchEvent(TouchEvent &event);
};

#endif
