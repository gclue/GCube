//
//  WebView.cpp
//  TheTerraDefender
//
//  Created by Hitoshi Haga on 12/03/27.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#include "WebView.h"
#include "Animation.h"
#include "APIGlue.h"
#include <string>

WebView::WebView(const char* url) : View(context) {
	openedView = false;
	initialURL = url;
	LOGD("viewID = %d",wholeWebViewCount);
	webViewID = wholeWebViewCount;
	wholeWebViewCount++;
}

WebView::WebView(GCContext *context, const char* url) : View(context) {
	openedView = false;
	initialURL = url;
	LOGD("viewID = %d",wholeWebViewCount);
	webViewID = wholeWebViewCount;
	wholeWebViewCount++;
}


WebView::~WebView() {
	//TODO: Native側のビューを消去する
	//deleteじゃなくてinvisibleにするだけでよい？
	GCSendWebViewEvent(WebViewEvent_RemoveView, webViewID, 0, 0, 0, 0, "");
	
}

void WebView::loadUrl(const char *url) {
	if(visible && openedView) {
		//TODO: 表示しているビューにURLを開かせる
	}
}


Pointf WebView::convertGlCoordToPixel(Pointf p) {
	Pointf r;
	float w = (float)context->getWidth();
	float h = (float)context->getHeight();
	r.x = w/2.0 + (w/2.0) * p.x;
	r.y = h/2.0 - (w/2.0) * p.y;
	
	
	return r;
	
};

Pointf WebView::convertGLSizeToPixel(Pointf p) {
	Pointf r;
	float w = (float)context->getWidth();
	float h = (float)context->getHeight();
	
	r.x = w * p.x;
	r.y = h * p.y;
	
	return r;
};




void WebView::setInitialURL(const char *url) {
	initialURL = url;
}

void WebView::render(double dt) {
	
	
	if (visible) {
		if(!openedView) {
			//TODO: Nativeのビューを表示
			Pointf webViewPos = convertGlCoordToPixel(point);
			Pointf webViewSize = convertGLSizeToPixel(size);
			LOGD("webViewPos = %f, %f",webViewPos.x, webViewPos.y);
			LOGD("webViewSize = %f, %f",webViewSize.x, webViewSize.y);
			
			GCSendWebViewEvent(WebViewEvent_AddView, webViewID, webViewPos.x - webViewSize.x / 2.0, webViewPos.y - webViewSize.y / 2.0, webViewSize.x, webViewSize.y, initialURL.c_str());
			
			
			openedView = true;
			return;
		}
		if (animation && !animation->isFinish()) {
			animation->step(dt);
		}
		
		//TODO:アニメーション処理
		
		
		if (animation) {
			if (animation->isFinish()) {
				// アニメーション終了
			}
		}
	}else {
		if(openedView) {
			//			GCSendWebViewEvent(WebViewEvent_CloseView, webViewID, 0, 0, 0, 0, "");
			GCSendWebViewEvent(WebViewEvent_RemoveView, webViewID, 0, 0, 0, 0, "");
			openedView = false;
			return;
		}
	}
}


void WebView::draw(double dt, IAnimation *animation) {
	//何もしない
}

