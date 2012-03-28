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


/**
 * WebViewクラス.
 */
class WebView : public View {

private:
	bool openedView; //!< ビューが開かれているかどうか.
	
	/**
	 * GL座標からピクセル座標へ変換します.
	 * @return p 座標（サイズ）.
	 */
	Pointf convertGlCoordToPixel(Pointf p);
	
	std::string initialURL;	//!< 初期URL.
	
public:
	/**
	 * コンストラクタ
	 * @param[in] context コンテキスト.
	 * @param[in] url URL 初期値は空白です.
	 */
	WebView(GCContext *context, const char* url = "");
	
	/**
	 * デストラクタ.
	 */
	virtual ~WebView();
	
	/**
	 * URLを読み込みます.
	 * @param[in] url URL.
	 */
	virtual void loadUrl(const char* url);
	
	/**
	 * 初期表示のURLを設定します.
	 * @param[in] url URL.
	 */
	virtual void setInitialURL(const char *url);
	
	//////////////
	// Viewからの継承
	//////////////
	
	virtual void render(double dt);
	
	virtual void draw(double dt, IAnimation *animation = NULL);
};


#endif
