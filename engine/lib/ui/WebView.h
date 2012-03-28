//
//  WebView.h
//  TheTerraDefender
//
//  Created by Hitoshi Haga on 12/03/27.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#ifndef TheTerraDefender_WebView_h
#define TheTerraDefender_WebView_h

#include "View.h"
#include <string>


static int wholeWebViewCount = 0;

class WebView : public View {
	
private:
	bool openedView;
	int webViewID;
	
	Pointf convertGlCoordToPixel(Pointf p);
	Pointf convertGLSizeToPixel(Pointf p);
	
	std::string initialURL;
	
public:
	
	WebView(GCContext *context, const char* url = "");
	
	virtual ~WebView();
	
	virtual void loadUrl(const char* url);
	
	
	virtual void setInitialURL(const char *url);
	
	//////////////
	// Viewからの継承
	//////////////
	
	virtual void render(double dt);
	
	virtual void draw(double dt, IAnimation *animation = NULL);
};


#endif
