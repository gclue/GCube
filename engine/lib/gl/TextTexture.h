//
//  TextTexture.h
//  GCube
//
//  Created by  on 12/04/06.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef GCube_TextTexture_h
#define GCube_TextTexture_h

#include "glcommon.h"
#include "Texture.h"
#include <string>


/**
 * 文字列情報を格納する構造体.
 */
struct _TextInfo {
	std::string text;
	float size;
	float r;
	float g;
	float b;
};
typedef struct _TextInfo TextInfo;

/**
 * 文字列用テクスチャ.
 */
class TextTexture : Texture {
private:
	std::vector<TextInfo> list;

public:
	TextTexture();

	/**
	 * デストラクタ.
	 */
	virtual ~TextTexture();

	virtual void reload();

	void addTextInfo(TextInfo info);

	std::vector<TextInfo> getTextList();
};


#endif
