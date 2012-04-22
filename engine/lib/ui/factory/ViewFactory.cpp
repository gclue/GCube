//
//  ViewFactory.cpp
//  GCube
//
//  Created by  on 12/04/18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ViewFactory.h"
#include "ViewGroup.h"
#include "ImageView.h"
#include "Button.h"
#include "TextureManager.h"
#include "SharedTexture.h"
#include "MathExpression.h"

#include <list>

ViewFactory::ViewFactory(){
    textureSercher = NULL;

}

ViewFactory::~ViewFactory() {
    if(textureSercher != NULL) delete textureSercher;
}

View* ViewFactory::onCreateView(const char* name, GCContext *context, std::map<std::string, std::string> attrs){
    LOGD("ViewFactory::onCreateView %s",name);
    View *view = NULL;
    std::map<std::string, std::string>::iterator it;
    
    if(strcmp("ViewGroup", name)==0){
        LOGD("create ViewGroup");
        ViewGroup *viewGroup = new ViewGroup(context);
        
        viewConfig(context, viewGroup, attrs);
        
        view = viewGroup;
    }
    else if(strcmp("ImageView", name) == 0){
        LOGD("create ImageView");
        ImageView *imageView = new ImageView(context);
        
        viewConfig(context, imageView, attrs);
        imageViewConfig(context, imageView, attrs);
        
        view = imageView;
    }
    else if(strcmp("Button", name) == 0){
        LOGD("create ImageView");
        Button *button = new Button(context);
        
        viewConfig(context, button, attrs);
        buttonConfig(context, button, attrs);
        
        view = button;
    }
    
	return view;
}

int atob(const char *str){
    if(strcmp("true", str)==0) return 1;
    else if(strcmp("false", str)==0) return -1;
    return 0;
}

float parseExpression(std::string expression, GCContext *context){
    int index = 0;
    char aspect[16];
    snprintf(aspect, 16, "%f", context->getAspect());
    LOGD("before: %s", expression.c_str());
    while(true){
        index = expression.find("#aspect");
        
        if(index == std::string::npos) break;
        
        expression.replace(index, 7, aspect);
    }
    LOGD("after: %s", expression.c_str());
    
    float ret;
    MathExpression *math = new MathExpression(expression);
    ret = math->eval();
    delete math;
    return ret;
}

std::list<std::string> split(std::string str, std::string delim){
    std::list<std::string> result;
    int cutAt;
    while( (cutAt = str.find_first_of(delim)) != str.npos ){
        if(cutAt > 0){
            result.push_back(str.substr(0, cutAt));
        }
        str = str.substr(cutAt + 1);
    }
    if(str.length() > 0){
        result.push_back(str);
    }
    return result;
}

void parseTextureName(std::string textureStr, std::string *textureName, std::string *imageName){
    std::list<std::string> strList;
    std::list<std::string>::iterator lit;
    strList = split(textureStr, "/");
    
    lit = strList.begin();
    lit++;
    
    if(lit != strList.end()){
        strList = split(*lit, ":");
        lit = strList.begin();
        
        *textureName = *lit;
        lit++;
        *imageName = *lit;
        
    }
}

void ViewFactory::viewConfig(GCContext *context, View* view, std::map<std::string, std::string> attrs){
    std::map<std::string, std::string>::iterator it;
    
    /*
     animation = NULL;
     parent = NULL;
     texture = NULL;
     visible = true;
     clickable = true;
     absolute = false;
     rotate = 0.0;
     alpha = 1.0;
     scale.x = 1.0;
     scale.y = 1.0;
     point.x = 0.0;
     point.y = 0.0;
     userObj = NULL;
     userID = -1;
     */
    
    it = attrs.find("id");
    if(it != attrs.end()){
        LOGD("id: %s",(it->second).c_str());
        view->setUserID(atoi(it->second.c_str()));
    }

    it = attrs.find("position_x");
    if(it != attrs.end()){
        float posx = parseExpression(it->second, context);
        
        it = attrs.find("position_y");
        if(it != attrs.end()){
            float posy = parseExpression(it->second, context);
            
            LOGD("position: (%f, %f)",posx, posy);
            view->setPosition(posx, posy);
        }
    }
    
    it = attrs.find("width");
    if(it != attrs.end()){
        float width = parseExpression(it->second, context);
        
        it = attrs.find("height");
        if(it != attrs.end()){
            float height = parseExpression(it->second, context);
            
            LOGD("size: (%f, %f)",width, height);
            view->setSize(width, height);
        }
    }
    
    it = attrs.find("scale_width");
    if(it != attrs.end()){
        float width = parseExpression(it->second, context);
        
        it = attrs.find("scale_height");
        if(it != attrs.end()){
            float height = parseExpression(it->second, context);
            
            LOGD("scale: (%f, %f)",width, height);
            view->setScale(width, height);
        }
    }


    
    it = attrs.find("visible");
    if(it != attrs.end()){
        LOGD("visible: %s",(it->second).c_str());
        int visible = atob(it->second.c_str());
        
        if(visible != 0){
            view->setVisible(visible==1);
        }
    }
    
    it = attrs.find("clickable");
    if(it != attrs.end()){
        LOGD("clickable: %s",(it->second).c_str());
        int clickable = atob(it->second.c_str());
        
        if(clickable != 0){
            view->setClickable(clickable==1);
        }
    }

    it = attrs.find("rotate");
    if(it != attrs.end()){
        float rotate = parseExpression(it->second, context);
        
        LOGD("rotate: %f",rotate);
        view->setRotate(rotate);
    }
    
    it = attrs.find("alpha");
    if(it != attrs.end()){
        float alpha = parseExpression(it->second, context);
        
        LOGD("alpha: %f",alpha);
        view->setAlpha(alpha);
    }

}

void ViewFactory::imageViewConfig(GCContext *context, ImageView* view, std::map<std::string, std::string> attrs){
    std::map<std::string, std::string>::iterator it;
    
    it = attrs.find("figure");
    if(it != attrs.end()){
        LOGD("figure: %s",(it->second).c_str());
        
        std::string textureName;
        std::string imageName;
        
        parseTextureName(it->second, &textureName, &imageName);
        
        if(textureSercher == NULL) textureSercher = new TextureSercher();
        SharedTexture *texture = textureSercher->getTexture(context->texMgr, textureName);
        LOGD("list: %s %s",textureName.c_str(), imageName.c_str());
        
        if(texture != NULL){
            
            float figureWidth,figureHeight;
            bool sizeFlag = false;
            
            it = attrs.find("figure_width");
            if(it != attrs.end()){
                figureWidth = parseExpression(it->second, context);
                
                it = attrs.find("figure_height");
                if(it != attrs.end()){
                    figureHeight = parseExpression(it->second, context);
                    
                    LOGD("figureSize: (%f, %f)",figureWidth, figureHeight);
                    sizeFlag = true;
                }
            }

            
            if(sizeFlag){
                view->setFigure(texture->makePlateWithSize(imageName.c_str(), figureWidth, figureHeight));
            }
            else {
                view->setFigure(texture->makePlate(imageName.c_str()));
            }
            view->setTexture(&texture->getTexture());
        }
       
    }

}

void ViewFactory::buttonConfig(GCContext *context, Button* view, std::map<std::string, std::string> attrs){
    std::map<std::string, std::string>::iterator it;
    
    it = attrs.find("figure");
    if(it != attrs.end()){
        LOGD("figure: %s",(it->second).c_str());
        
        std::string textureName;
        std::string imageName;
        
        parseTextureName(it->second, &textureName, &imageName);
        
        if(textureSercher == NULL) textureSercher = new TextureSercher();
        SharedTexture *texture = textureSercher->getTexture(context->texMgr, textureName);
        LOGD("list: %s %s",textureName.c_str(), imageName.c_str());
        
        if(texture != NULL){
            
            float figureWidth,figureHeight;
            bool sizeFlag = false;
            
            it = attrs.find("figure_width");
            if(it != attrs.end()){
                figureWidth = parseExpression(it->second, context);
                
                it = attrs.find("figure_height");
                if(it != attrs.end()){
                    figureHeight = parseExpression(it->second, context);
                    
                    LOGD("figureSize: (%f, %f)",figureWidth, figureHeight);
                    sizeFlag = true;
                }
            }
            
            
            if(sizeFlag){
                view->setFigure(texture->makePlateWithSize(imageName.c_str(), figureWidth, figureHeight));
            }
            else {
                view->setFigure(texture->makePlate(imageName.c_str()));
            }
            view->setTexture(&texture->getTexture());
        }
        
    }
    
    it = attrs.find("select_figure");
    if(it != attrs.end()){
        LOGD("select_figure: %s",(it->second).c_str());
        
        std::string textureName;
        std::string imageName;
        
        parseTextureName(it->second, &textureName, &imageName);
        
        if(textureSercher == NULL) textureSercher = new TextureSercher();
        SharedTexture *texture = textureSercher->getTexture(context->texMgr, textureName);
        LOGD("list: %s %s",textureName.c_str(), imageName.c_str());
        
        if(texture != NULL){
            
            float figureWidth,figureHeight;
            bool sizeFlag = false;
            
            it = attrs.find("select_figure_width");
            if(it != attrs.end()){
                figureWidth = parseExpression(it->second, context);
                
                it = attrs.find("select_figure_height");
                if(it != attrs.end()){
                    figureHeight = parseExpression(it->second, context);
                    
                    LOGD("figureSize: (%f, %f)",figureWidth, figureHeight);
                    sizeFlag = true;
                }
            }
            
            
            if(sizeFlag){
                view->setSelectFigure(texture->makePlateWithSize(imageName.c_str(), figureWidth, figureHeight));
            }
            else {
                view->setSelectFigure(texture->makePlate(imageName.c_str()));
            }
            view->setTexture(&texture->getTexture());
        }
        
    }

}



/*************
 TextureSercher
 *************/
TextureSercher::TextureSercher(){
    int result = XMLParser::parseFromAsset("texture/texture.xml");
    LOGD("TextureSercher:XMLParser %d", result);
    //TODO: エラー処理
}

TextureSercher::~TextureSercher(){
    
}

SharedTexture* TextureSercher::getTexture(TextureManager *mgr, std::string name){
    SharedTextureObj *tmp;
    std::vector<SharedTextureObj*>::iterator it;
    for(it=textureList.begin();it != textureList.end();it++){
        tmp = *it;
        if(tmp->name.compare(name)==0){
            return mgr->getSharedTexture(tmp->textureName.c_str(), tmp->packerName.c_str());
        }
    }
    return NULL;
}

void TextureSercher::startElement(const XML_Char *name, const XML_Char *atts[]){
    LOGD("startElement: %s", name);
    
    if(strcmp("SharedTexture", name) == 0){
        SharedTextureObj *obj = new SharedTextureObj();
        for(int i=0;atts[i];i+=2){
            LOGD("atts %d: [%s] %s",i, atts[i], atts[i+1]);
            
            if(strcmp("name", atts[i])==0){
                obj->name = atts[i+1];
            }
            else if(strcmp("texture_name", atts[i])==0){
                obj->textureName = atts[i+1];
            }
            else if(strcmp("packer_name", atts[i])==0){
                obj->packerName = atts[i+1];
            }
        }
        
        textureList.push_back(obj);
    }

}
void TextureSercher::endElement(const XML_Char *name){
    LOGD("endElement: %s", name);
}
void TextureSercher::bodyElement(const XML_Char *s, int len){
}
