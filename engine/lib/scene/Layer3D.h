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

#include "dixsmartptr.h"

class Camera;
class Figure;
class Matrix3D;
class Light;
class Layer3D;
class FigureSet;
class Storage;

/**
 * 剛体の種類.
 */
enum RigidBodyType {
	RigidBodyType_None,
	RigidBodyType_Ground,
	RigidBodyType_Box,
	RigidBodyType_Sphere,
	RigidBodyType_Cylinder,
	RigidBodyType_Mesh
};

/**
 * 剛体情報構造体.
 */
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
	RigidBodyType type;
	
	RigidBodyOption() {
		x = y = z = restitution = friction = isKinematic = 0;
		sizeX = sizeY = sizeZ = radius = mass = 1.0;
		type = RigidBodyType_None;
	}
};

/**
 * Figure情報構造体.
 */
struct FigureInfo {
	int id;
	Figure *fig;
	Texture *tex;
	Matrix3D *mtx;
	GCObject *userObj;
	Vector3D force; // out
	Vector3D velocity; // out
	Vector3D rel_pos; // out
};

/**
 * イベントハンドラインターフェイス.
 */
class ILayer3DEventHandler {
public:
	/**
	 * デストラクタ.
	 */
	virtual ~ILayer3DEventHandler(){};
	
	/**
	 * 各3Dオブジェクトの処理.
	 * @return trueでワールドから削除されます.
	 */
	virtual bool handleFigure(Layer3D *layer, FigureInfo &info) = 0;
	
	/**
	 * 各オブキェクトの衝突処理.
	 */
	virtual void contactFigure(Layer3D *layer, FigureInfo &obj0, FigureInfo &obj1) {};
	
	/**
	 * 各オブキェクトの保存処理.
	 */
	virtual void save3DObject(Layer3D *layer, FigureInfo *info, int index, int max) {};
	
};

/**
 * 3Dを描画するためのレイヤー.
 */
class Layer3D : public Layer, IBulletWorldEventHandler {
private:
	std::map<unsigned long, FigureSet*> figures;	//!< FigureSetを保持
	std::map<int, Light*> lights;		//!< 追加したライトを保持
	BulletWorld *bullet;				//!< 物理演算ワールド
	ILayer3DEventHandler *handler;		//!< イベントハンドラ
	unsigned long objcount;						//!< 総オブジェクト数
	Storage *tmpStorage;

	/** 初期化します. */
	void initLayer3D();
	
	/**
	 * 外力をかける.
	 */
	void applyForce(FigureInfo &info, FigureSet *set);
	
public:
	Camera *camera;	//!< カメラ
	float gravity;
	
	/**
	 * コンストラクタ.
	 */
	Layer3D();
	
	/**
	 * コンストラクタ.
	 * @param context 描画するためのコンテキスト
	 * @deprecated コンテキストを使用しないように変更したので、コンストラクタで設定する必要がない
	 */
	Layer3D(GCContext *context);
	
	/**
	 * デストラクタ.
	 */
	virtual ~Layer3D();
	
	/**
	 * 内容のクリア.
	 */
	void clear();
	
	/**
	 * イベントリスナー設定.
	 * @param[in] listener リスナー
	 */
	void setEventHandler(ILayer3DEventHandler *handler) {this->handler=handler;};
	
	/**
	 * Figureを追加します.
	 * 追加したFigureはこのオブジェクトと共に解放されます.
	 * @param info オブジェクト情報
	 * @param mtx 座標変換行列（NULLの場合はFigureのtransformを使用します）
	 * @return 識別ID
	 */
	virtual int addFigure(FigureInfo &info, RigidBodyOption option=RigidBodyOption());
	
	/**
	 * オブジェクトの情報を書き換えます.
	 * @param id 識別ID
	 * @param info オブジェクト情報
	 * @return 成功true
	 */
	virtual bool replaceObjectByID(unsigned long id, FigureInfo &info);
	
	/**
	 * 指定したIDに対応するFigureを取得します.
	 * 指定したIDに対応するFigureが存在しない場合にはNULLを返却します.
	 * @param id ID
	 * @return Figure
	 */
	virtual Figure* findFigureByID(unsigned long id);
	
	/**
	 * 指定したIDに対応するTextureを取得します.
	 * 指定したIDに対応するTextureが存在しない場合にはNULLを返却します.
	 * @param id ID
	 * @return Texture
	 */
	virtual Texture* findTextureByID(unsigned long id);
	
	/**
	 * 指定したIDに対応するMatrix3Dを取得します.
	 * 指定したIDに対応するMatrix3Dが存在しない場合にはNULLを返却します.
	 * @param id ID
	 * @return Matrix3D
	 */
	virtual Matrix3D* findMatrixByID(unsigned long id);

	/**
	 * 指定したIDに対応するUserObjectを取得します.
	 * 指定したIDに対応するUserObjectが存在しない場合にはNULLを返却します.
	 * @param id ID
	 * @return Matrix3D
	 */
	virtual GCObject* findUserObjectByID(unsigned long id);

	/**
	 * ライトを追加します.
	 * 追加したライトはこのオブジェクトと共に解放されます.
	 * @param id 識別ID
	 * @param fig Light
	 */
	virtual void addLight(int id, Light *light);

	/**
	 * 現在の状態を指定されたファイルに保持します.
	 * @param[in] filename ファイル名
	 */
	virtual bool save(const char *filename);
	
	/**
	 * 指定されたファイルから前回の状態を復帰します.
	 * @param[in] filename ファイル名
	 */
	virtual bool load(const char *filename);
	
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
	
	/**
	 * コンテキストが切り替わったことを通知します.
	 */
	virtual void onContextChanged();
	
	
	//////////////////////////////////////////////////////////
	// IBulletWorldEventHandler の実装
	//////////////////////////////////////////////////////////
	
	/**
	 * 各オブジェクトの処理.
	 */
	virtual void stepBulletObject(BulletWorld *world, btCollisionObject *obj);
	
	/**
	 * 各オブジェクトの衝突処理.
	 */
	virtual void contactBulletObject(BulletWorld *world, btRigidBody *obj0, btRigidBody *obj1);
	
	/**
	 * 各オブジェクトの保存処理.
	 */
	virtual bool saveBulletObject(BulletWorld *world, btRigidBody *body, UserObj *obj, int index, int max);
	
	/**
	 * 各オブジェクトの読み込み処理.
	 */
	virtual bool loadBulletObject(BulletWorld *world, btRigidBody *body, UserObj *obj, int index, int max);
	
};

#endif /* LAYER3D_H_ */
