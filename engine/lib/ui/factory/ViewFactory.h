//
//  ViewFactory.h
//  GCube
//
//  Created by  on 12/04/18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef GCube_ViewFactory_h
#define GCube_ViewFactory_h

#include "LayoutInflater.h"

class ImageView;
class SharedTexture;
class Button;

class SharedTextureObj {
public:
    std::string name;
    std::string textureName;
    std::string packerName;
};

class TextureSercher : public XMLParser{
private:
    std::vector<SharedTextureObj*> textureList;
public:
    TextureSercher();
    virtual ~TextureSercher();
    SharedTexture* getTexture(TextureManager *mgr, std::string name);
    
    virtual void startElement(const XML_Char *name, const XML_Char *atts[]);
	virtual void endElement(const XML_Char *name);
	virtual void bodyElement(const XML_Char *s, int len);
};

class ViewFactory : public ViewFactoryInterface{
private:
    TextureSercher *textureSercher;
    void viewConfig(GCContext *context, View* view, std::map<std::string, std::string> attrs);
    void imageViewConfig(GCContext *context, ImageView* view, std::map<std::string, std::string> attrs);
    void buttonConfig(GCContext *context, Button* view, std::map<std::string, std::string> attrs);

public:
	ViewFactory();
	virtual ~ViewFactory();

	virtual View* onCreateView(const char* name, GCContext *context, std::map<std::string, std::string> attrs);
};

#endif
