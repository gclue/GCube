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
 * TextureManager.cpp
 *
 *  Created on: 2011/10/08
 *      Author: GClue, Inc.
 */
#include "TextureManager.h"
#include "Texture.h"
#include "SharedTexture.h"
#include "PackerTexture.h"

//////////////////////////////////////////////////////////////////////////////////////////
/// TextureObj
//////////////////////////////////////////////////////////////////////////////////////////

class TextureObj {
public:
	std::string name;
	Texture *texture;
	PackerTexture *packTex;

	TextureObj();
	virtual ~TextureObj();
};

TextureObj::TextureObj() {
	texture = NULL;
	packTex = NULL;
}
TextureObj::~TextureObj() {
	DELETE(texture);
	DELETE(packTex);
}

//////////////////////////////////////////////////////////////////////////////////////////
/// TextureManager
//////////////////////////////////////////////////////////////////////////////////////////

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
	for (int i = 0; i < cache.size(); i++) {
		TextureObj *obj = cache.at(i);
		DELETE(obj);
	}
	cache.clear();
}

Texture *TextureManager::getTexture(const char *name) {
	return loadTexture(name);
}

SharedTexture *TextureManager::getSharedTexture(const char *png, const char *txt) {
	return loadSharedTexture(png, txt);
}

Texture* TextureManager::loadTexture(const char *name) {
	TextureObj *obj = searchTexture(name);
	if (obj) {
		return obj->texture;
	}

	Texture *tex = new Texture(name);
	if (tex) {
		if (addTexture(name, tex)) {
			return tex;
		} else {
			DELETE(tex);
		}
	}
	return NULL;
}

SharedTexture* TextureManager::loadSharedTexture(const char *png, const char *txt) {
	TextureObj *obj = searchTexture(png);
	if (obj) {
		return obj->packTex;
	}

	PackerTexture *tex = new PackerTexture(png, txt);
	if (tex) {
		if (addTexture(png, (SharedTexture *) tex)) {
			return tex;
		} else {
			DELETE(tex);
		}
	}
	return NULL;

}
void TextureManager::deleteTexture(const char *name) {
	removeTexture(name);
}

void TextureManager::reload() {
	for (int i = 0; i < cache.size(); i++) {
		TextureObj *obj = cache.at(i);
		obj->packTex->reload();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
/// privateな関数
//////////////////////////////////////////////////////////////////////////////////////////

bool TextureManager::addTexture(const char *name, Texture *tex) {
	TextureObj *obj = new TextureObj();
	if (!obj) {
		return false;
	}
	obj->name.append(name);
	obj->texture = tex;
	cache.push_back(obj);
	return true;
}

bool TextureManager::addTexture(const char *name, SharedTexture *tex) {
	TextureObj *obj = new TextureObj();
	if (!obj) {
		return false;
	}
	obj->name.append(name);
	obj->packTex = (PackerTexture *) tex;
	cache.push_back(obj);
	return true;
}

bool TextureManager::removeTexture(const char *name) {
	std::vector<TextureObj *>::iterator it = cache.begin();
	while (!cache.empty() && it != cache.end()) {
		TextureObj *a = *it;
		if (a->name.compare(name) == 0) {
			it = cache.erase(it);
			delete a;
		} else {
			it++;
		}
	}
	return true;
}

TextureObj* TextureManager::searchTexture(const char *name) {
	for (int i = 0; i < cache.size(); i++) {
		TextureObj *obj = cache.at(i);
		if (obj->name.compare(name) == 0) {
			return obj;
		}
	}
	return NULL;
}
