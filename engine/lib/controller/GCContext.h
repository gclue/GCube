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
 * GCContext.h
 *
 *  Created on: 2011/10/08
 *      Author: GClue, Inc.
 */
#ifndef AECONTEXT_H_
#define AECONTEXT_H_

class Camera;
class SimpleShader;
class TextureManager;

/**
 * GCubeで使用されるコンテキスト.
 */
class GCContext {
protected:
	int width;				//!< 画面の横幅
	int height;				//!< 画面の縦幅
	float aspect;			//!< 画面のアスペクト比

public:
	Camera *camera;			//!< カメラ (UIライブラリなどで使用する)
	SimpleShader *shader;	//!< シェーダ (UIライブラリなどで使用する)
	TextureManager *texMgr;	//!< テクスチャ管理クラス
	void *userObj;			//!< ユーザデータ

	/** コンストラクタ. */
	GCContext();

	/** デストラクタ. */
	virtual ~GCContext();

	/** セットアップ. */
	void setupContext();

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	float getAspect() {
		return aspect;
	}

};

#endif /* AECONTEXT_H_ */
