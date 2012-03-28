//
//  WebViewLayerController.h
//  TheTerraDefender
//
//  Created by Hitoshi Haga on 12/03/27.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface WebViewLayerController : UIViewController


-(void)addWebView:(int)viewId: (double)x: (double)y: (double)width: (double)height: (NSString*)url;
-(void)closeWebView:(int)viewId;
-(void)removeWebView:(int)viewId;

@end
