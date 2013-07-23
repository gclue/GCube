//
//  PhysicsView.h
//  GCube
//
//  Created by 小林 伸郎 on 2013/07/13.
//
//

#ifndef __GCube__PhysicsView__
#define __GCube__PhysicsView__

#include "ViewGroup.h"
#include "Box2D.h"

class PhysicsView : public ViewGroup {
private:
	b2Vec2 linearVelocity;
	b2Vec2 force;
	float32 angularVelocity;
public:
	b2Body *body;
	
	/**
	 * コンストラクタ.
	 */
	PhysicsView();
	
	/**
	 * コンストラクタ.
	 * @param context View用のコンテキスト
	 * @deprecated コンテキストを使用しないように変更したので、コンストラクタで設定する必要がない
	 */
	PhysicsView(GCContext *context);
	
	/**
	 * デストラクタ.
	 */
	virtual ~PhysicsView();
	
	/**
	 * b2Bodyを設定します.
	 * @param[in] body b2Body
	 */
	void setBody(b2Body *body);
	
	/**
	 * b2Bodyを取得します.
	 * @return b2Body
	 */
	b2Body *getBody();
	
	/**
	 * 移動速度を設定します.
	 * @param[in] x
	 * @param[in] y
	 */
	void setLinearVelocity(float x, float y);
	
	/**
	 * 回転速度を設定します.
	 * @aram[in] w 回転速度
	 */
	void setAngularVelocity(float w);
	
	/**
	 * 力を設定します.
	 * @param[in] forceX
	 * @param[in] forceY
	 */
	void applyForce(float forceX, float forceY);
	
	//////////////////////////////////////////////////////////////
	// Viewからの継承
	//////////////////////////////////////////////////////////////
	
	/**
	 * Viewの表示位置を設定します.
	 * @param[in] pos 表示位置
	 */
	virtual void setPosition(Pointf& pos);
	
	/**
	 * Viewの表示位置を設定します.
	 * @param[in] x x座標
	 * @param[in] y y座標
	 */
	virtual void setPosition(float x, float y);
	
	/**
	 * 回転を設定します.
	 * @param rotate 回転
	 */
	virtual void setRotate(float rotate);

	/**
	 * 指定されたアニメーションを反映して描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 * @param[in] animation 反映するアニメーション
	 */
	virtual void draw(double dt, ViewContext *context);
};

#endif /* defined(__GCube__PhysicsView__) */
