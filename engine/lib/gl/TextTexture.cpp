//
//  TextTexture.cpp
//  GCube
//
//  Created by  on 12/04/06.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "TextTexture.h"
#include "APIGlue.h"

TextTexture::TextTexture() : Texture(){

}

TextTexture::~TextTexture() {


}

std::vector<TextInfo> TextTexture::getTextList(){
	return list;
}

void TextTexture::addTextInfo(TextInfo info) {
	list.push_back(info);
}

void TextTexture::reload() {
	//LOG("TextTexture::reload()");

	GCReloadTextTexture(this);
}
