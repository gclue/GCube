//
//  TextTexture.cpp
//  GCube
//
//  Created by  on 12/04/06.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "TextTexture.h"
#include "APIGlue.h"
#include "Log.h"

TextTexture::TextTexture() : PackerTexture() {
}

TextTexture::~TextTexture() {
}

std::vector<TextInfo> TextTexture::getTextList(){
	return textlist;
}

void TextTexture::addTextInfo(TextInfo& info) {
	textlist.push_back(info);
}

void TextTexture::reload() {
	LOGD("TextTexture::reload()");
	GCReloadTextTexture(this);
}
