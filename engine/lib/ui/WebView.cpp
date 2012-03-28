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


WebView::WebView(GCContext *context, const char* url) : View(context) {
	//開かれていない状態.
	openedView = false;
	initialURL = url;
}


WebView::~WebView() {
	//TODO: Native側のビューを消去する
	//deleteじゃなくてinvisibleにするだけでよい？
	//TODO: ここらへんのライフサイクルを考えます.
	GCSendWebViewEvent(WebViewEvent_RemoveView, userID, 0, 0, 0, 0, "");
	
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
	
	//TODO: サイズはあっているけど、この計算方法だとiPhone,Androidで大きさ位置があわない。画面的にはあってる。
	
	return r;
	
};


void WebView::setInitialURL(const char *url) {
	initialURL = url;
}

void WebView::render(double dt) {
	
	if (visible) {
		if(!openedView) { //visibleでかつまだビューが開かれていない場合

			//TODO: Nativeのビューを表示
			Pointf webViewPos = convertGlCoordToPixel(point);
			Pointf webViewSize = convertGlCoordToPixel(size);
		
			//ビューの追加。既に追加されている場合は非表示から表示に切り替えるだけ.
			GCSendWebViewEvent(WebViewEvent_AddView, userID, webViewPos.x - webViewSize.x / 2.0, webViewPos.y - webViewSize.y / 2.0, webViewSize.x, webViewSize.y, initialURL.c_str());
			
			
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
		//非表示でビューが開かれている場合
		if(openedView) {
			//TODO: Nativeのビューをクローズ
			GCSendWebViewEvent(WebViewEvent_CloseView, userID, 0, 0, 0, 0, "");
			openedView = false;
			return;
		}
	}
}


void WebView::draw(double dt, IAnimation *animation) {
	//何もしない
}

