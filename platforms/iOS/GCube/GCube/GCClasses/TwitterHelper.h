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

@class SA_OAuthTwitterEngine;

@interface TwitterHelper : UIViewController <SA_OAuthTwitterControllerDelegate> {
	SA_OAuthTwitterEngine *_engine;
}

@property (copy, nonatomic) void (^responseBlock)(int);
@property (copy, nonatomic) void (^tweetResponseBlock)(int);

-(void) tweet:(NSString*)text;
-(void) authenticate;
@end
