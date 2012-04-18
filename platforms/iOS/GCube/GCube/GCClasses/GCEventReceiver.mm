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
#include "HttpClient.h"

#import "GCAppDelegate.h"
#import "GCViewController.h"

/**
 * 非同期HTTP Client用レスポンスクラス
 */
@interface AsyncResponse:NSObject{
    IHttpRquestListener* listener;
    NSMutableData* asyncData;
    int statusCode;
}
- (id)initWithListener:(IHttpRquestListener*)lis;
@end


@implementation AsyncResponse
- (id)initWithListener:(IHttpRquestListener*)lis
{
    self = [super init];
    if (self) {
        listener = lis;
        statusCode = 0;
        asyncData = nil;
    }
    return self;
}
- (void)dealloc
{
    if(asyncData != nil) [asyncData release];
    [super dealloc];
}


- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response{
    //NSLog(@"didReceiveResponse");
    NSHTTPURLResponse *res = [response isKindOfClass:[NSHTTPURLResponse class]]?(NSHTTPURLResponse*)response:nil;
    
    if(res!=nil){
        statusCode = [res statusCode];
    }
    
    asyncData = [[NSMutableData alloc]initWithCapacity:64];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data{
    NSLog(@"didReceiveData");
    [asyncData appendData:data];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error{
    NSLog(@"didFailWithError");
    
    
    HttpResponse *httpResponse = new HttpResponse();
    httpResponse->setStatusCode(statusCode);
    
    NSString *errorMes = [error localizedDescription];
    
    httpResponse->setData([errorMes cStringUsingEncoding:NSUTF8StringEncoding]);
    listener->onResponse(httpResponse);
    
    [self release];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection{
    NSLog(@"connectionDidFinishLoading");
    
    HttpResponse *httpResponse = new HttpResponse();
    httpResponse->setStatusCode(statusCode);
    
    if (asyncData != nil) {
        NSUInteger length = [asyncData length];
        char *dataChar;
        dataChar = (char*)malloc(sizeof(char) * length);
        [asyncData getBytes:dataChar length:length];
        NSLog(@"data length: %d",length);
        httpResponse->setData(dataChar, length);
        free(dataChar);
        
        listener->onResponse(httpResponse);
    } else {
        // error handling
        listener->onResponse(NULL);
    }
    
    [self release];
}

@end



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
    NSString *urlString = [NSString stringWithCString:url.c_str() encoding:NSUTF8StringEncoding];
    
    
    // Create the request.
    NSURLRequest *request=[NSURLRequest requestWithURL:[NSURL URLWithString:urlString]
                                           cachePolicy:NSURLRequestUseProtocolCachePolicy
                                       timeoutInterval:60.0];
    
    AsyncResponse *asyncResponse = [[AsyncResponse alloc]initWithListener:callback];
    
    NSURLConnection *conn = [NSURLConnection connectionWithRequest:request delegate:asyncResponse];
    
    if(conn!=nil){
        return 1;
    }
    else {
        return 0;
    }
}

/**
 * HTTP通信を行います.
 * @param url 通信先のURL
 * @param headers ヘッダー
 * @param body ボディ
 * @return HTTP通信の結果
 */
HttpResponse* GCHttpRequest(std::string url, std::map<std::string, std::string> headers, std::string body){
    NSString *urlString = [NSString stringWithCString:url.c_str() encoding:NSUTF8StringEncoding];
    
    
    // Create the request.
    NSURLRequest *request=[NSURLRequest requestWithURL:[NSURL URLWithString:urlString]
                                           cachePolicy:NSURLRequestUseProtocolCachePolicy
                                       timeoutInterval:60.0];
    
    NSHTTPURLResponse *response = nil;
    NSError *error = nil;
    NSData *data = [NSURLConnection sendSynchronousRequest : request returningResponse : &response error : &error];//リクエストの送信（１）
    
    //NSLog(@"response %lld  %@",[response expectedContentLength], [response suggestedFilename]);
    //NSLog(@"response %@  %@",[response MIMEType], [response textEncodingName]);
    //NSLog(@"response %@ %d",[response URL], [response statusCode]);
    
    HttpResponse *httpResponse = new HttpResponse();
    
    httpResponse->setStatusCode([response statusCode]);
    
    
    // error
    NSString *error_str = [error localizedDescription];
    if (0<[error_str length]) {
        /*UIAlertView *alert = [
         [UIAlertView alloc]
         initWithTitle : @"RequestError"
         message : error_str
         delegate : nil
         cancelButtonTitle : @"OK"
         otherButtonTitles : nil
         ];
         [alert show];
         [alert release];
         */
        httpResponse->setData([error_str cStringUsingEncoding:NSUTF8StringEncoding]);
        return httpResponse;
    }
    
    
    /*
     // responseを受け取ったあとの処理
     NSFileManager *fm = [NSFileManager defaultManager];
     [fm createFileAtPath:@"" contents:[NSData data] attributes:nil];
     NSFileHandle *file = [[NSFileHandle fileHandleForWritingAtPath:filePath] 
     retain];
     [file writeData:data];                                   //ファイルへのデータの書き込み（３）
     */
    if (data != nil) {
        // response
        /*int encodeArray[] = {
         NSUTF8StringEncoding,			// UTF-8
         NSShiftJISStringEncoding,		// Shift_JIS
         NSJapaneseEUCStringEncoding,	// EUC-JP
         NSISO2022JPStringEncoding,		// JIS
         NSUnicodeStringEncoding,		// Unicode
         NSASCIIStringEncoding			// ASCII
         };
         NSString *dataString = nil;
         int max = sizeof(encodeArray) / sizeof(encodeArray[0]);
         for (int i=0; i<max; i++) {
         dataString = [
         [NSString alloc]
         initWithData : data
         encoding : encodeArray[i]
         ];
         if (dataString != nil) {
         break;
         }
         }*/
        NSUInteger length = [data length];
        char *dataChar;
        dataChar = (char*)malloc(sizeof(char) * length);
        [data getBytes:dataChar length:length];
        NSLog(@"data length: %d",length);
        //NSLog(@"str %@",dataString);
        //httpResponse->setData([dataString cStringUsingEncoding:NSUTF8StringEncoding]);
        httpResponse->setData(dataChar, length);
        free(dataChar);
        
    } else {
        // error handling
    }
    
    return httpResponse;
}


void GCSoundEvent(const char *fileName, int mode){
	
}