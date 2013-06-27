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
 * PlistTexture.cpp
 *
 *  Created on: 2013/06/26
 *      Author: GClue, Inc.
 */

#include "PlistTexture.h"
#include "PlistParser.h"
#include "GCMap.h"
#include "GCString.h"
#include "GCVector.h"
#include "Log.h"
#include "PlistHelper.h"
#include "Texture.h"

#pragma mark - PlistTexture constructor

PlistTexture::PlistTexture(const char *plist) {
	list.clear();
    
    if (plist) {
        loadPlist(plist);
    }
}

PlistTexture::~PlistTexture() {
	list.clear();
}

#pragma mark - private methods
void PlistTexture::loadPlist(const char *plistName) {
    
    string sPlistName(plistName);
    string::size_type index = sPlistName.rfind("/");
    
    GCMap *plist = createMapFromFile(plistName); // PlistParser::createMapFromFile(plistName);
    
    if (!plist) {
        return;
    }
    
    GCMap *metadata = (GCMap *) plist->objectForKey("metadata");
    
    GCString *texName = (GCString *) metadata->objectForKey("textureFileName");
    
    const char* texturePath;
    
    
    if (index != string::npos) {
        sPlistName = sPlistName.substr(0, index + 1);
        sPlistName.append(texName->getCString());
        texturePath = sPlistName.c_str();
    } else {
        texturePath = plistName;
    }

    
    load(texturePath);
    
    // テクスチャデータの読み込み
    int format = ((GCString *) metadata->objectForKey("format"))->intValue();
    GCMap *frames = (GCMap *) plist->objectForKey("frames");
    
    string *pngNames = new string[frames->size()];
    frames->allKeys(pngNames);
    
    for (int i = 0; i < frames->size(); i++) {
        GCMap *data = (GCMap *) frames->objectForKey(pngNames[i]);
        TexData tex;
        tex.name.append(pngNames[i].c_str());
        
        string textureRectKey;
        string rotateKey;
        
        if (format == 3) {
            textureRectKey = "textureRect";
            rotateKey = "textureRotated";
        } else if (format == 2) {
            textureRectKey = "frame";
            rotateKey = "rotated";
        }
        
        GCString *textureRect = (GCString *) data->objectForKey(textureRectKey);
        tex.rotate = ((GCString *) data->objectForKey(rotateKey))->intValue();
        
        PlistParser::formatRectData(textureRect->getCString(), tex.rect);
        
        if (format == 2 && tex.rotate == 1) {
            PPSize size;
            GCString *sourceSize = (GCString *) data->objectForKey("sourceSize");
            PlistParser::formatSizeData(sourceSize, size);
            tex.rect.right = tex.rect.left + size.height;
            tex.rect.bottom = tex.rect.top + size.width;
        }
        
        tex.padding.top = tex.padding.left = tex.padding.right = tex.padding.bottom = 0;
        list.push_back(tex);
    }
    
    delete[] pngNames;
    plist->release();
}


#pragma mark - public methods

void PlistTexture::addTexData(TexData tex) {
	list.push_back(tex);
}

TexData PlistTexture::getTexData(int ID, int sub) {
	return list.at(ID);
}

TexData PlistTexture::getTexData(const char *name) {
	std::vector<TexData>::iterator it = list.begin();
	while (!list.empty() && it != list.end()) {
		TexData dat = *it;
		if (strcmp(dat.name.c_str(), name) == 0) {
			return dat;
		}
		it++;
	}
	return list.at(0);
}