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
#include "PackerTexture.h"
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
class TextTexture : public PackerTexture {
private:
	std::vector<TextInfo> textlist;

public:
	TextTexture();

	/**
	 * デストラクタ.
	 */
	virtual ~TextTexture();

	virtual void reload();

	void addTextInfo(TextInfo& info);

	std::vector<TextInfo> getTextList();
};


#endif
