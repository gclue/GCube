//
//  TwitterHelper.h
//  GCube
//
//  Created by Hitoshi Haga on 12/04/18.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "SA_OAuthTwitterEngine.h"
#import "GCAppDelegate.h"
#import "SA_OAuthTwitterController.h"

#define TWITTER_HELPER_EVENT_POST_SUCCESS 0
#define TWITTER_HELPER_EVENT_POST_FAILED 1
#define TWITTER_HELPER_EVENT_AUTH_SUCCESS 0
#define TWITTER_HELPER_EVENT_AUTH_FAILED 1
#define TWITTER_HELPER_EVENT_ALREADY_AUTHORIZED 2

@class SA_OAuthTwitterEngine;


@interface TwitterHelper : UIViewController <SA_OAuthTwitterControllerDelegate> {
	SA_OAuthTwitterEngine *_engine;
}

@property (copy, nonatomic) void (^responseBlock)(int);
@property (copy, nonatomic) void (^tweetResponseBlock)(int);

-(void) tweet:(NSString*)text;
-(void) authenticate;
@end
