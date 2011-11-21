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
 * GCContext.cpp
 *
 *  Created on: 2011/10/08
 *      Author: GClue, Inc.
 */
#include "GCContext.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "BoneShader.h"
#include "TextureManager.h"
#include "defines.h"
#include "GCObject.h"

GCContext::GCContext() {
	// カメラの初期化
	camera = new Camera();
	camera->loadOrthographic(true);
	camera->transForm.translate(0, 0, 100);

	// 初期化
	shader = new SimpleShader();
	shader3d = new BoneShader();
	texMgr = new TextureManager();
	userObj = NULL;
}

GCContext::~GCContext() {
	DELETE(shader3d);
	DELETE(shader);
	DELETE(camera);
	DELETE(texMgr);
	if(userObj) userObj->release();
}

void GCContext::setupContext() {
	// シンプルなシェーダの初期化
	delete shader;
	shader = new SimpleShader();
	// 3D用シェーダの初期化
	delete shader3d;
	shader3d = new BoneShader();
	// テクスチャマネージャーの初期化
	texMgr->reload();
}

