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

#import "TwitterHelper.h"
#import <GameKit/GameKit.h>
#import "AdMobViewController.h"

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

TwitterHelper *helper = NULL;

//ツイッターイベント送信を受け取ります。
void GCSendTwitterEvent(int type, const char *text) {
	LOGD("GCSendTwitterEvent %d, %s",type, text);
	switch(type) {
		case TwitterEvent_Post: {
			
			NSString *str = [NSString stringWithUTF8String:text];
			
			NSLog(str);
			//テスト投稿用のテキスト.時間を自動追加で二重投稿回避.
			//NSString *addTimeText = [str stringByAppendingString:[NSString stringWithFormat: @"Already Updated. %@", [NSDate date]]];
			
			
			//ツイッタービューの作成.
			if(!helper) {
				helper = [[TwitterHelper alloc] init];
			}
			
			
			//認証コールバック.
			helper.responseBlock = ^(int type) {
				NSLog(@"auth res %d",type);
				if(type == TWITTER_HELPER_EVENT_ALREADY_AUTHORIZED) {

					//認証済み
					[helper tweet:str];
				}
				if(type == TWITTER_HELPER_EVENT_AUTH_FAILED) {
					//認証失敗 or　キャンセル
					ApplicationController *ctr = CGControllerInstance();
					ctr->onTwitterEvent(TwitterEvent_Post, 2);
					
				}
				if(type == TWITTER_HELPER_EVENT_AUTH_SUCCESS) {
					//認証成功→ツイート
					[helper tweet:str];
				}
			};
			
			//ツイートコールバック.
			helper.tweetResponseBlock = ^(int type) {
				NSLog(@"tweet res %d",type);
				if(type == TWITTER_HELPER_EVENT_POST_SUCCESS) {
					//ツイート成功
					ApplicationController *ctr = CGControllerInstance();
					ctr->onTwitterEvent(TwitterEvent_Post, 0);
			
				}
				if(type == TWITTER_HELPER_EVENT_POST_FAILED) {
					//ツイート失敗
					ApplicationController *ctr = CGControllerInstance();
					ctr->onTwitterEvent(TwitterEvent_Post, 1);
				}
				[helper removeFromParentViewController];
			};
			
			
			//twitter登録画面の表示
			GCAppDelegate *delegate = (GCAppDelegate*)[[UIApplication sharedApplication] delegate];
			[delegate.viewController.view addSubview:helper.view];
			
			
			[helper authenticate];
			break;
		}
		case TwitterEvent_Authenticate: {
			if(!helper) {
				helper = [[TwitterHelper alloc] init];
			}
			
						
			helper.responseBlock = ^(int type) {
				NSLog(@"%d",type);
				ApplicationController *ctr = CGControllerInstance();
				switch(type) {
						
					case TWITTER_HELPER_EVENT_AUTH_SUCCESS:	//認証成功
						ctr->onTwitterEvent(TwitterEvent_Authenticate, 0);
						
						break;
					case TWITTER_HELPER_EVENT_AUTH_FAILED: //認証失敗
						ctr->onTwitterEvent(TwitterEvent_Authenticate, 1);
						break;
					case TWITTER_HELPER_EVENT_ALREADY_AUTHORIZED: //認証済み
						ctr->onTwitterEvent(TwitterEvent_Authenticate, 0);
						break;
				}
				[helper removeFromParentViewController];
				
			};
			
			
			
			
			//twitter登録画面の表示
			GCAppDelegate *delegate = (GCAppDelegate*)[[UIApplication sharedApplication] delegate];
			[delegate.viewController.view addSubview:helper.view];

			
			[helper authenticate];
			
			break;
		}
	}
	
}

//デリゲートイベントを受け取るためのクラス.
@interface GCubeLeaderBoardDelegate: NSObject <GKLeaderboardViewControllerDelegate>

@end

@implementation GCubeLeaderBoardDelegate

//リーダーボードのDoneが押された時の処理を行います.
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController {
	NSLog(@"did finished");
	
	//ビューコントローラを閉じる
	[viewController dismissModalViewControllerAnimated:YES];
	ApplicationController *ctr =CGControllerInstance();
	ctr->onGameCenterEvent(GameCenterEvent_Close_LeaderBoard);
	//いらないのでリリース
	[viewController release];
	//自身も必要ないのでリリース.
	[self release];
}


@end


//ゲームセンターイベントを処理します.
void GCSendGameCenterEvent(int type, long long lScore, int iScore, double dScore) {
	LOGD("GCSendGameCenterEvent: %d, %ld, %d, %f",type, lScore, iScore, dScore);
	
	ApplicationController *ctr =CGControllerInstance();
	
	switch(type) {
		case GameCenterEvent_Login:
			//GameCenterのログインを行います
			//既にログイン済みの場合は、「ようこそ〜」が、していない場合はOS指定のダイアログが表示されます。
			[[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error){
				//エラー処理.
				if(error != nil) {
					NSLog(@"GameCenterEvent_Login error = %@",error);
					ctr->onGameCenterEvent(GameCenterEvent_Login, 1);
				}else {
					ctr->onGameCenterEvent(GameCenterEvent_Login, 0);
				}
			}];
			break;
			
			//リーダーボードを開きます
		case GameCenterEvent_Open_LeaderBoard: {
						
			//ビューコントロラーを作成
			GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
			//ログインをしていればnil以外が入ります.
			if (leaderboardController != nil)
			{
				ctr->onGameCenterEvent(GameCenterEvent_Open_LeaderBoard, 0);
				//デフォルトで表示するリーダーボードカテゴリを指定します。
				//何も入れなければ、リーダーボードのトップページにいきます
				//				leaderboardController.category = @"gcubereaderboard2";
				
				//表示されるリーダーボードのタイムスコープを選択します。today, this week ,alltimeから選びます。
				//				leaderboardController.timeScope = GKLeaderboardTimeScopeToday;
				
				//デリゲートを設定します。リスナーです。
				//自前のデリゲートを継承したクラスを使います。必要な処理があればそちらに書いてください.
				leaderboardController.leaderboardDelegate = [[GCubeLeaderBoardDelegate alloc] init];
				
				//GCubeのデリゲートを取得.
				GCAppDelegate *delegate = (GCAppDelegate*)[[UIApplication sharedApplication] delegate];
				//デリゲートのビューコントローラーにリーダーボードのビューコントロラーを追加.
				[delegate.viewController presentModalViewController:leaderboardController animated:YES];
				
				
				
			}else {
				//TODO: エラー処理.
				ctr->onGameCenterEvent(GameCenterEvent_Open_LeaderBoard, 1);
			}
			
			break;
		}
			
			//整数のスコアを送信
		case GameCenterEvent_Send_Score_Integer: {
			//スコア送信インスタンスを作成
			//送信するカテゴリを引数に作成します。カテゴリ名は必須です。
			GKScore *scoreReporter = [[GKScore alloc] initWithCategory:@"gcubereaderboard2"];
			
			//NSIntegerにintを格納します。こっちじゃないとダメだという噂.
			NSInteger scoreR;
			scoreR=iScore;
			scoreReporter.value = scoreR;
			
			//スコアの送信
			[scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
				if (error != nil)
				{
					// 報告エラーの処理
					NSLog(@"SendScore_Integer error = %@",error);
					ctr->onGameCenterEvent(GameCenterEvent_Send_Score_Integer, 1);
				}else {
					//TODO: 成功処理.
					ctr->onGameCenterEvent(GameCenterEvent_Send_Score_Integer, 0);
				}
			}];
			
			break;
		}
			//Doubleとlong型で欲しい場合の処理。
			//必要に応じて実装してね！
		case GameCenterEvent_Send_Score_Double:
			break;
		case GameCenterEvent_Send_Score_Long:
			break;
	}
}



static AdMobViewController *admobController = NULL;

void GCSendAdMobEvent(int type, int pos, const char* unitID) {
	LOGD("*****GCSendAdMobEvent %d %d %s ",type,pos,unitID);
	

	
	switch(type) {
		case AdMobEvent_Create: { //AdMobを作成
			if(!admobController) {
				admobController = [[AdMobViewController alloc] customInit:[NSString stringWithUTF8String:unitID]];
				admobController.view.frame = CGRectMake(0, 0, 320, 48);
				ApplicationController *ctr =CGControllerInstance();
				float w = ctr->getWidth();
				float h = ctr->getHeight();
				[admobController changePosition:pos:w:h];
				
				//GCubeのデリゲートを取得.
				GCAppDelegate *delegate = (GCAppDelegate*)[[UIApplication sharedApplication] delegate];
				//ビューを追加.
				[delegate.viewController.view addSubview:admobController.view];
				
			}else {
				ApplicationController *ctr =CGControllerInstance();
				float w = ctr->getWidth();
				float h = ctr->getHeight();
				[admobController changePosition:pos:w:h];
			}
		
			break;
		}
		case AdMobEvent_Visible: { //AdMobを表示
			if(admobController) {
				//GCubeのデリゲートを取得.
				GCAppDelegate *delegate = (GCAppDelegate*)[[UIApplication sharedApplication] delegate];
				//ビューを追加.
				[delegate.viewController.view addSubview:admobController.view];
			}
			break;
		}
		case AdMobEvent_Invisible: {//AdMobを非表示
			if(admobController) {
				//ビューを削除.
				[admobController.view removeFromSuperview];
			}
			break;
		}

	}
	
}


void GCSoundEvent(const char *fileName, int mode){
	//空実装
}

bool GCReloadTextTexture(TextTexture *texture) {
	//空実装
	return false;
}