//
//  LayoutInflater.h
//  GCube
//
//  Created by  on 12/04/18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef GCube_LayoutInflater_h
#define GCube_LayoutInflater_h

#include "GCContext.h"
#include "View.h"
#include "ViewGroup.h"
#include <map>
#include <string>
#include <vector>
#include "XMLParser.h"

class ViewFactoryInterface {
public:
	virtual ~ViewFactoryInterface() {}
	virtual View* onCreateView(const char* name, GCContext *context, std::map<std::string, std::string> attrs) = 0;
};

class ViewFactoryInterfaceMerger : public ViewFactoryInterface {
private:
    std::vector<ViewFactoryInterface *> factories;
public:
    virtual ~ViewFactoryInterfaceMerger();
    virtual void addFactory(ViewFactoryInterface *factory);
    virtual View* onCreateView(const char* name, GCContext *context, std::map<std::string, std::string> attrs);
};

class ViewTreeObj {
public:
    ViewTreeObj *parent;
    View *current;
    std::string name;
};

class LayoutInflater : public GCObject , public XMLParser{
private:
	GCContext *context;
    ViewFactoryInterfaceMerger *factory;
    ViewTreeObj *curView;

public:
	LayoutInflater(GCContext *context);

	virtual ~LayoutInflater();

	virtual View* inflate(const char *xmlFileName);

	virtual void addFactory(ViewFactoryInterface *factory);


	virtual void startElement(const XML_Char *name, const XML_Char *atts[]);
	virtual void endElement(const XML_Char *name);
	virtual void bodyElement(const XML_Char *s, int len);
};

#endif
