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
 * TextureManager.h
 *
 *  Created on: 2011/10/08
 *      Author: GClue, Inc.
 */
#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <vector>
#include <string>

class Texture;
class SharedTexture;
class TextureObj;

/**
 * テクスチャを管理するクラス.
 */
class TextureManager {
private:
	std::vector<TextureObj*> cache;	//!< テクスチャをキャッシュするvector

	bool addTexture(const char *name, Texture *tex);
	bool addTexture(const char *name, SharedTexture *tex);
	bool removeTexture(const char *name);
	TextureObj* searchTexture(const char *name);

public:
	/** コンテキスト. */
	TextureManager();
	/** デストラクタ. */
	virtual ~TextureManager();
    
    void addExtraTexture(const char *name, SharedTexture *tex);

	Texture *getTexture(const char *name);
	SharedTexture *getSharedTexture(const char *png, const char *txt);

	Texture* loadTexture(const char *name);
	SharedTexture* loadSharedTexture(const char *png, const char *txt);
	void deleteTexture(const char *name);

	void reload();
};

#endif /* TEXTUREMANAGER_H_ */
