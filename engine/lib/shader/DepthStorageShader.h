//
//  DepthStorageShader.h
//  GCube
//
//  Created by 小林 伸郎 on 2013/08/09.
//
//

#ifndef __GCube__DepthStorageShader__
#define __GCube__DepthStorageShader__

#include "Shader.h"

/**
 * 影描画用のシェーダークラス.
 */
class DepthStorageShader : public Shader {
public:
	/**
	 * コンストラクタ.
	 */
	DepthStorageShader();
	
	/**
	 * デストラクタ.
	 */
	virtual ~DepthStorageShader();
	
	/**
	 * シェーダの使用を開始します.
	 */
	void useProgram();
	
	/**
	 * カメラと変換行列からMVPをシェーダに設定します.
	 * <br>
	 * @param[in] camera カメラオブジェクト
	 * @param[in] matrix 変換マトリックス
	 */
	void setMVPMatrix(Camera *camera, Matrix3D *matrix);
	
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

#endif /* defined(__GCube__DepthStorageShader__) */
