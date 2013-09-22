//
//  DepthShadowShader.h
//  GCube
//
//  Created by 小林 伸郎 on 2013/08/09.
//
//

#ifndef __GCube__DepthShadowShader__
#define __GCube__DepthShadowShader__


#include "Shader.h"
#include "Light.h"

/**
 * シェーダークラス.
 */
class DepthShadowShader : public Shader {
public:
	/**
	 * コンストラクタ.
	 */
	DepthShadowShader();
	
	/**
	 * デストラクタ.
	 */
	virtual ~DepthShadowShader();
	
	/**
	 * シェーダの使用を開始します.
	 */
	void useProgram();
	
	/**
	 * テクスチャをバインドします.
	 * <br>
	 * @param[in] texname テクスチャ名
	 */
	void bindTexture(int texname);
	
	/**
	 * テクスチャをバインドします.
	 * <br>
	 * @param[in] texname テクスチャ名
	 */
	void bindShadowTexture(int texname);
	
	/**
	 * カメラと変換行列からMVPをシェーダに設定します.
	 * <br>
	 * @param[in] camera カメラオブジェクト
	 * @param[in] matrix 変換マトリックス
	 */
	void setMVPMatrix(Camera *camera, Matrix3D *matrix);
	
	/**
	 * モデルのマトリックスをシェーダに設定します.
	 * @param[in] matrix モデルのマトリクス
	 */
	void setModelMatrix(Matrix3D *matrix);
	
	/**
	 * 影用のテクスチャマトリクスを設定します.
	 * @param[in] camera ライトのカメラ
	 */
	void setTextureMatrix(Camera *camera);
	
	/**
	 * ライトから見たモデルのMVPをシェーダに設定します.
	 * @param[in] carema ライトからのカメラ
	 * @param[in] matrix モデルのマトリクス
	 */
	void setLightMatrix(Camera *camera, Matrix3D *matrix);
	
	/**
	 * モデルの逆行列を設定します.
	 * @param[in] matrix マトリクス
	 */
	void setInverseMatrix(Matrix3D *matrix);
	
	/**
	 * ライトの位置をシェーダに設定します.
	 * @param[in] light ライト
	 */
	void setLightPosition(Light *light);
	
	/**
	 * スキニング用のボーンの変換行列をシェーダに設定します.
	 * <br>
	 * @param[in] matrix 変換行列の配列
	 * @param[in] len 配列の長さ
	 */
	void setSkinningMatrix(Matrix3D **matrix, int len);
	
	/**
	 * スキニング用のボーンの変換行列をシェーダに設定します.
	 * <br>
	 * @param fig
	 */
	void setSkinningMatrix(Figure *fig);
	
	/**
	 * 影のON/OFF
	 */
	void setUseShadow(bool flag);
	
	/**
	 * トゥーンシェーダのON/OFFを設定します.
	 */
	void setUseEdge(bool use);
	
	/**
	 * エッジの色を指定します.
	 */
	void setEdgeColor(float r, float g, float b, float a);
	
	/**
	 * エッジのサイズを設定します.
	 */
	void setEdgeSize(float size);
	
	/**
	 * アルファ値を設定します.
	 */
	void setAlpha(float alpha);

	/**
	 * シェーダのAttributeへのバインド処理を行います.
	 * <br><br>
	 * ES2Rendererから継承した関数.
	 * ES2Renderer内部から呼び出されるので、実装しないとエラーになります.
	 * <br>
	 * @param[in] program シェーダプログラム
	 * @param[in] name シェーダファイルへの名前
	 * @param[in] user ユーザ識別ID
	 */
	void bindAttribute(GLuint program, const char *name, int user);
	
	/**
	 * シェーダのUniformを取得します.
	 * <br><br>
	 * ES2Rendererから継承した関数.
	 * ES2Renderer内部から呼び出されるので、実装しないとエラーになります.
	 * <br>
	 * @param[in] program シェーダプログラム
	 * @param[in] name シェーダファイルへの名前
	 * @param[in] user ユーザ識別ID
	 */
	void getUniform(GLuint program, const char *name, int user);
	
};

#endif /* defined(__GCube__DepthShadowShader__) */
