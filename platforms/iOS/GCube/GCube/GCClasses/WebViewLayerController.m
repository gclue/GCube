//
//  WebViewLayerController.m
//  TheTerraDefender
//
//  Created by Hitoshi Haga on 12/03/27.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#import "WebViewLayerController.h"


@interface WebViewLayerController ()

@end

NSMutableArray *webViews; //画面上に出ているWebViewを管理する配列

@implementation WebViewLayerController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
	
	
	webViews = [[NSMutableArray alloc]init];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
	[webViews release];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}



#pragma mark public methods

//ウェブビューを追加します.
- (void)addWebView:(int)viewId: (double)x: (double)y: (double)width: (double)height: (NSString*)url {
	
	//ViewIDから既に追加されているかを探します。
	for(id ids in webViews) {
		if([ids tag] == viewId) {
			//あったらWebViewを表示
			[ids setHidden:NO];
			NSLog(@"this view id had been added");
			return;
		}
	}
	
	//ウェブビューの作成.
	UIWebView *view = [[UIWebView alloc] initWithFrame:CGRectMake(x, y, width, height)];
	
	//初期ページのロード
	if([url length] != 0) {
		NSURL *cURL = [NSURL URLWithString:url];
		NSURLRequest *req = [NSURLRequest requestWithURL:cURL];
		[view loadRequest:req];
	}
	
	//タグの追加
	[view setTag:viewId];
	//表示
	[view setHidden:NO];
	//管理配列に追加.
	[webViews addObject:view];
	
	
	[[self view]addSubview:view];
}

//ビューを閉じます.removeはしません
-(void)closeWebView:(int)viewId {
	for(id ids in webViews) {
		if([ids tag] == viewId) {
			//あったら非表示に.
			[ids setHidden:YES];
			
		}
	}
}

//ウェブビューをremoveします.
-(void)removeWebView:(int)viewId {
	for(id ids in webViews) {
		if([ids tag] == viewId) {
			[ids removeFromSuperview];
			[webViews removeObject:ids];
			return;
		}
	}
	NSLog(@"not found %d", viewId);
}


@end
