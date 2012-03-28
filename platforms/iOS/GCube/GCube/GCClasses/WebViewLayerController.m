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


NSMutableArray *webViews;

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


- (void)addWebView:(int)viewId: (double)x: (double)y: (double)width: (double)height: (NSString*)url {
	
	
	for(id ids in webViews) {
		if([ids tag] == viewId) {
			[ids setHidden:NO];
			NSLog(@"this view id had been added");
			return;
		}
	}
	
	UIWebView *view = [[UIWebView alloc] initWithFrame:CGRectMake(x, y, width, height)];
	
	//初期ページのロード
	if([url length] != 0) {
		NSURL *cURL = [NSURL URLWithString:url];
		NSURLRequest *req = [NSURLRequest requestWithURL:cURL];
		[view loadRequest:req];
	}
	
	[view setTag:viewId];
	[view setHidden:NO];
	[webViews addObject:view];
	
	
	[[self view]addSubview:view];
}

-(void)closeWebView:(int)viewId {
	for(id ids in webViews) {
		if([ids tag] == viewId) {
			[ids setHidden:YES];
			
		}
	}
}


@end
