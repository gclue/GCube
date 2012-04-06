//
//  GCEventReceiver.m
//  GCubeHello
//
//  Created by Hitoshi Haga on 12/04/05.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//


#include "APIGlue.h"
#include "Log.h"
#include "ApplicationController.h"


#import "GCAppDelegate.h"
#import "GCViewController.h"

/**
 * イベントを配送します.
 * @param[in] type イベントタイプ
 * @param[in] param1 イベントパラメータ
 * @param[in] param2 イベントパラメータ
 * @param[in] param3 イベントパラメータ
 * @param[in] param4 イベントパラメータ
 * @param[in] param5 イベントパラメータ
 */
void GCSendGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5) {
    LOGD( "**GCSendGameEvent**:%d, %d, %ld, %f, %d, %s", type, param1, param2, param3, param4, param5);
	
	LOGD( "**GCSendGameEvent**:END");
}


/**
 * 非同期にHTTP通信を行います.
 * @param url 通信先のURL
 * @param headers ヘッダー
 * @param body ボディ
 * @param callback HTTP通信を行った結果を返すリスナー
 * @return 0以下の場合には通信失敗
 */
int GCHttpRequestAsync(std::string url, std::map<std::string, std::string> headers, std::string body, IHttpRquestListener *callback){
	
}

/**
 * HTTP通信を行います.
 * @param url 通信先のURL
 * @param headers ヘッダー
 * @param body ボディ
 * @return HTTP通信の結果
 */
HttpResponse* GCHttpRequest(std::string url, std::map<std::string, std::string> headers, std::string body){
	
}


void GCSoundEvent(const char *fileName, int mode){
	
}