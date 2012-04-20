//
//  LayoutInflater.cpp
//  GCube
//
//  Created by  on 12/04/18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "LayoutInflater.h"
#include "ViewGroup.h"

LayoutInflater::LayoutInflater(GCContext *context){
    this->context = context;
    factory = new ViewFactoryInterfaceMerger();

}

LayoutInflater::~LayoutInflater() {
	LOGD("~LayoutInflater");
    
    delete factory;
}

View* LayoutInflater::inflate(const char *xmlFileName){
	LOGD("filename: %s",xmlFileName);

    curView = NULL;
	int result = XMLParser::parseFromAsset(xmlFileName);

	LOGD("parse finish: %d", result);

    if(curView != NULL){
        View *view = curView->current;
        delete curView;
        curView = NULL;
        
        return view;
    }
    
	return NULL;
}

void LayoutInflater::addFactory(ViewFactoryInterface *newfactory){
	factory->addFactory(newfactory);
}


void LayoutInflater::startElement(const XML_Char *name, const XML_Char *atts[]){
	LOGD("startElement: %s",name);


    std::map<std::string, std::string> attrsMap;
	for(int i=0;atts[i];i+=2){
		LOGD("atts %d: [%s] %s",i, atts[i], atts[i+1]);
        
        attrsMap[atts[i]] = atts[i+1];
	}
    
    View *view = factory->onCreateView(name, context, attrsMap);

    
    if(curView != NULL){
        ViewGroup *viewGroup = dynamic_cast<ViewGroup *>(curView->current);
        
        if(viewGroup != NULL){
            viewGroup->addView(view);
            
            ViewTreeObj *tree = new ViewTreeObj();
            tree->parent = curView;
            tree->current = view;
            tree->name = name;
            
            curView = tree;
            
        }
        view->release();
    }
    else {
        curView = new ViewTreeObj();
        curView->parent = NULL;
        curView->current = view;
        curView->name = name;
    }
}

void LayoutInflater::endElement(const XML_Char *name){
	LOGD("endElement: %s",name);
    if(strcmp(curView->name.c_str(), name)==0){
        LOGD("remove stack: %s",name);
        
        if(curView->parent != NULL){
            LOGD("remove: %s",name);
            
            ViewTreeObj *tmp = curView->parent;
            delete curView;
            
            curView = tmp;
        }
    }

}
void LayoutInflater::bodyElement(const XML_Char *s, int len){
	//LOGD("bodyElement: %d %s",len, s);

}

/**********************************************
 ViewFactoryInterfaceMerger
 **********************************************/
ViewFactoryInterfaceMerger::~ViewFactoryInterfaceMerger(){
    int size = factories.size();
    for (int i = 0; i < size; i++) {
        delete factories.at(i);
    }
    factories.clear();
}

void ViewFactoryInterfaceMerger::addFactory(ViewFactoryInterface *factory){
    factories.push_back(factory);
}

View* ViewFactoryInterfaceMerger::onCreateView(const char* name, GCContext *context, std::map<std::string, std::string> attrs){
    
    View *view = NULL;
    int size = factories.size();
    for (int i = 0; i < size; i++) {
        view = factories.at(i)->onCreateView(name, context, attrs);
        
        if(view != NULL) return view;
    }

    return NULL;
}