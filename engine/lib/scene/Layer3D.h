/*
 * The MIT License (MIT)
 * Copyright (c) 2011 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * Layer3D.h
 *
 *  Created on: 2011/09/15
 *      Author: GClue, Inc.
 */
#ifndef LAYER3D_H_
#define LAYER3D_H_

#include <map>
#include <vector>
#include "Layer.h"
#include "BulletWorld.h"

class Camera;
class Figure;
class Matrix3D;

struct RigidBodyOption {
	float x;
	float y;
	float z;
	float sizeX;
	float sizeY;
	float sizeZ;
	float radius;
	float mass; // 質量
	float restitution; // 反発係数
	float friction; // 摩擦係数
	bool isKinematic;
	
	RigidBodyOption() {
		x = y = z = restitution = friction = isKinematic = 0;
		sizeX = sizeY = sizeZ = radius = mass = 1.0;
	}
};

enum RigidBodyType {
	RigidBodyType_None,
	RigidBodyType_Ground,
	RigidBodyType_Box,
	RigidBodyType_Sphere,
	RigidBodyType_Cylinder,
	RigidBodyType_Mesh
};

class Layer3D : public Layer, IBulletWorldEventHandler {
private:
	struct FigureSet {
		Figure *fig;
		Texture *tex;
		Matrix3D *mtx;
		btRigidBody* body;
		
		Matrix3D *getMatrix() {
			if (mtx) {
				return mtx;
			} else {
				return fig->transForm;
			}
		}
	};
	std::map<int, FigureSet> figures;	//!< 追加したFigureを保持
	BulletWorld *bullet;

	
public:
	Camera *camera;	//!< カメラ
	
	/**
	 * コンストラクタ.
	 */
	Layer3D(GCContext *context);
	
	/**
	 * デストラクタ.
	 */
	virtual ~Layer3D();
	
	/**
	 * Figureを追加します.
	 * 追加したFigureはこのオブジェクトと共に解放されます.
	 * @param id 識別ID
	 * @param fig Figure
	 * @param tex テクスチャ
	 * @param mtx 座標変換行列（NULLの場合はFigureのtransformを使用します）
	 */
	virtual void addFigure(int id, Figure *fig, Texture *tex=NULL, Matrix3D *mtx=NULL, RigidBodyType type=RigidBodyType_None, RigidBodyOption option=RigidBodyOption());
	
	/**
	 * 指定したIDに対応するFigureを取得します.
	 * 指定したIDに対応するFigureが存在しない場合にはNULLを返却します.
	 * @param id ID
	 * @return Figure
	 */
	virtual Figure* findFigureByID(int id);
	
	/**
	 * 指定したIDに対応するTextureを取得します.
	 * 指定したIDに対応するTextureが存在しない場合にはNULLを返却します.
	 * @param id ID
	 * @return Texture
	 */
	virtual Texture* findTextureByID(int id);
	
	/**
	 * 指定したIDに対応するMatrix3Dを取得します.
	 * 指定したIDに対応するMatrix3Dが存在しない場合にはNULLを返却します.
	 * @param id ID
	 * @return Matrix3D
	 */
	virtual Matrix3D* findMatrixByID(int id);


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
	
	/**
	 * コンテキストが切り替わったことを通知します.
	 */
	virtual void onContextChanged();
	
	
	//////////////////////////////////////////////////////////
	// IBulletWorldEventHandler の実装
	//////////////////////////////////////////////////////////
	
	/**
	 * 各オブキェクトの処理.
	 */
	virtual void stepBulletObject(BulletWorld *world, btCollisionObject *obj);
};

#endif /* LAYER3D_H_ */
