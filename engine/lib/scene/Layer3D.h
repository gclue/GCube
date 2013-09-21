//
//  Layer3D.h
//  GCube
//
//  Created by 小林 伸郎 on 2013/08/21.
//
//

#ifndef __GCube__Layer3D__
#define __GCube__Layer3D__


#include "Layer.h"
#include "Light.h"
#include "Camera.h"
#include <vector>

class DepthStorageShader;
class SimpleShader;
class ShadowShader;
class BoneShader;

enum GC3DRenderMode {
	RenderTypeNone = 0,
	RenderTypeShadow,
};

enum GC3DShaderType {
	DepthStorageShaderType = 1,
	ShadowShaderType,
	SimpleShaderType,
	BoneShaderType,
};

typedef struct GC3DContext {
	DepthStorageShader *depthShader;
	SimpleShader *simpleShader;
	ShadowShader *shadowShader;
	BoneShader *boneShader;
	Camera *camera;
	Camera *lightcamera;
	Light *light;
	int type;
	bool shadowFlag;
} GC3DContext;

typedef struct GCFrameBuffer {
	GLuint fb;
	GLuint rb;
	GLuint t;
} GCFrameBuffer;


/**
 * Layer3DでFigureを表示するためのクラス.
 */
class FigureSet : public GCObject {
protected:
	Figure *figure;
	Texture *texture;
	Texture *textureMlt;
	
	bool useEdge;
	bool shadowFlag;
	bool removeFlag;
	int userId;
	float edgeSize;
	
	Point3f pos;
	
	Matrix3D matrix;
	
	Colorf edgeColor;
	IAnimation *animation;
	
	void testMatrix(Matrix3D *m);
	
public:
	FigureSet();
	virtual ~FigureSet();
	
	Figure *getFigure() {
		return figure;
	}
	
	Texture *getTexture() {
		return texture;
	}
	
	void startAnimation(IAnimation *a);
	
	/**
	 * 削除フラグの状態を取得します.
	 * @return 削除された場合はtrue、それ以外の場合はfalse
	 */
	bool isRemoveFlag();
	
	/**
	 * 削除フラグを設定します.
	 * @param[in] flag 削除フラグ
	 */
	void setRemoveFlag(bool flag);
	
	/**
	 * エッジの描画設定.
	 */
	void setUseEdge(bool flag);
	
	/**
	 * エッジの描画設定を取得します.
	 */
	bool isUseEdge();
	
	/**
	 * エッジのカラーを設定します.
	 * @param[in] r 赤色成分
	 * @param[in] g 緑色成分
	 * @param[in] b 青色成分
	 * @param[in] a アルファ値成分
	 */
	void setEdgeColor(float r, float g, float b, float a);
	
	void setEdgeSize(float size);
	
	void setShadowFlag(bool flag);
	
	/**
	 * 指定されたユーザIDチェックします.
	 * @param[in] userId ユーザID
	 * @return
	 */
	FigureSet *findFigureSetByID(int userId);
	
	/**
	 * ユーザIDを設定します.
	 * @param[in] userId ユーザID
	 */
	void setUserID(int userId);
	
	/**
	 * ユーザIDを取得します.
	 * @return ユーザID
	 */
	int getUserID();
	
	/**
	 * テクスチャを設定します.
	 * @param[in] texture テクスチャ
	 */
	void setTexture(Texture *texture);
	
	/**
	 * マルチテクスチャを設定します.
	 * @param[in] texture テクスチャ
	 */
	void setTextureMlt(Texture *texture);
	
	/**
	 * Figureデータを設定します.
	 * @param[in] fig Figureデータ
	 */
	void setFigure(Figure *fig);
	
	void makeIdentity();
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void rotate(float angle, float x, float y, float z);
	
	/**
	 * 描画を行います.
	 * @param[in] dt
	 * @param[in] context
	 */
	void render(float dt, GC3DContext &context);
};

/**
 * 3D表示用のレイヤークラス.
 */
class Layer3D : public Layer {
protected:
	std::vector<FigureSet*> figures;
	bool lockflag;
	bool depthBufferFlag;
	bool useShadowFlag;
	
	int renderMode;
	
	Camera camera;
	Camera lightcamera;
	Light light;
	
	GC3DContext gc3dcontext;
	
	DepthStorageShader *depthShader;
	ShadowShader *shadowShader;
	SimpleShader *simpleShader;
	BoneShader *boneShader;
	
	FigureSet *lightfigure;
	
	int fbWidth;
	int fbHeight;
	GCFrameBuffer fb;
	
	/**
	 * @private
	 * シャドウマッピング用のフレームバッファを作成します.
	 * @param[in] width 横幅
	 * @param[in] height 縦幅
	 */
	void createFrameBuffer(int width, int height);
	
	/**
	 * FigureSetの削除を行います.
	 * removeFigureSetでは、削除フラグをONにするだけで、実際にはこの関数で削除する。
	 */
	void removeInternal();
	
	void drawScene(float dt);
	void drawSceneWithShadow(float dt);
	
public:
	Layer3D();
	virtual ~Layer3D();
	
	/**
	 * カメラを取得します.
	 * @return カメラ
	 */
	Camera& getCamera() {
		return camera;
	}
	
	/**
	 * シャドウマッピング用のカメラを取得します.
	 * @return カメラ
	 */
	Camera& getLightCamera() {
		return lightcamera;
	}
	
	void setShadowFlag(bool flag);
	
	/**
	 * ライトの位置を設定します.
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @param[in] z z座標
	 */
	void setLightPos(float x, float y, float z);
	
	/**
	 * 表示するFigureSetを追加します.
	 * @param[in] set 追加するFigureSet
	 */
	void addFigureSet(FigureSet *set);
	
	/**
	 * 指定されたユーザIDのFigureSetを削除します.
	 * 指定されたユーザIDがレイヤーに存在しない場合には何もしません。
	 * @param[in] userId ユーザID
	 */
	void removeFigureSet(int userId);
	
	/**
	 * 指定されたFigureSetを削除します.
	 * 指定されたFigureSetがレイヤーに存在しない場合には何もしません。
	 * @param[in] set 削除するFigureSet
	 */
	void removeFigureSet(FigureSet *set);
	
	/**
	 * すべてのFigureSetを削除します.
	 */
	void removeAllFigureSet();
	
	/**
	 * 指定されたユーザIDのFigureSetを取得します.
	 * @param[in] userId ユーザID
	 * @return 指定されたユーザIDのFigureSet
	 */
	FigureSet *findFigureSetByID(int userId);
	
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

#endif /* defined(__GCube__Layer3D__) */
