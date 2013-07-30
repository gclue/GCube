//
//  ParticleShader.h
//  GCube
//
//  Created by 小林 伸郎 on 2013/07/24.
//
//

#ifndef __GCube__ParticleShader__
#define __GCube__ParticleShader__


#include "Camera.h"
#include "Shader.h"
#include "Figure.h"

enum {
	PARTICLE_ATTR_VERT = NUM_ATTRIBUTES,
	PARTICLE_ATTR_TEXCOORD,
	PARTICLE_ATTR_COLOR,
};

/**
 * シェーダークラス.
 */
class ParticleShader : public Shader {
private:
	int texname;		//!< 使用しているtextureのID

public:
	/**
	 * コンストラクタ.
	 */
	ParticleShader();
	
	/**
	 * デストラクタ.
	 */
	virtual ~ParticleShader();
	
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
	 * カメラと変換行列からMVPをシェーダに設定します.
	 * <br>
	 * @param[in] camera カメラオブジェクト
	 * @param[in] matrix 変換マトリックス
	 */
	void setMVPMatrix(Camera *camera, Matrix3D *matrix);
	
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

#endif /* defined(__GCube__ParticleShader__) */
