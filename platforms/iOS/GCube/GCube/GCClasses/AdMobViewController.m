//
//  AdMobViewController.m
//  GCube
//
//  Created by Hitoshi Haga on 12/04/23.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#import "AdMobViewController.h"


@implementation AdMobViewController

@synthesize unitID;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (id)customInit:(NSString*)unitid {
	self = [super init];
	if(self != nil) {
		unitID = [NSString stringWithFormat:unitid];
		[unitID retain];
	}
	return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

	//ビューの作成.
	bannerView_ = [[GADBannerView alloc] initWithAdSize:kGADAdSizeBanner];
	
	//ビューにユニットIDを設定
	bannerView_.adUnitID = unitID;
	
	//ビューコントローラーの設定.
	bannerView_.rootViewController = self;
	//追加.
	[self.view addSubview:bannerView_];
	
	//AdMobのリクエストを作成.
	//このリクエストでターゲッティングなどを行える.
	GADRequest *request = [GADRequest request];
	
	//TODO: リリース時はここをNOにするかコメントアウト.
	request.testing  = YES;

	
	
	//リクエストをロード.
	[bannerView_ loadRequest:request];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
	[bannerView_ release];
}

-(void)changePosition:(int)pos: (float)width2: (float)height2 {
	CGFloat width = self.view.frame.size.width;
	CGFloat height = self.view.frame.size.height;
	
	float w = width2 / [UIScreen mainScreen].scale;
	float h = height2 / [UIScreen mainScreen].scale;
	switch(pos) {
		case 1:  //下中央
			self.view.frame = CGRectMake(w/2.0 - width/2.0, h - height, width, height);
			
			break;
		case 0: //上中央
			self.view.frame = CGRectMake(w/2.0 -width/2.0, 0, width, height);
			break;
		case 4://左下
			self.view.frame = CGRectMake(0, h-height, width, height);
			break;
		case 3://左上
			self.view.frame = CGRectMake(0, 0, width, height);
			break;
		case 6://右下
			self.view.frame = CGRectMake(w - width, h-height, width, height);
			break;
		case 5://右上
			self.view.frame = CGRectMake(w - width, 0, width, height);
			break;
		case 2: //中央（使わないと思うけど）
			self.view.frame = CGRectMake(w/2.0-width/2.0, h/2.0 - height/2.0, width, height);
			break;
			
	}
	
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)dealloc {
	[super dealloc];
	[unitID release];
}


- (void)adViewDidReceiveAd:(GADBannerView *)bannerView {
	
}
- (void)adView:(GADBannerView *)bannerView
didFailToReceiveAdWithError:(GADRequestError *)error {
	
}

- (void)adViewWillPresentScreen:(GADBannerView *)bannerView{
	
}
- (void)adViewDidDismissScreen:(GADBannerView *)bannerView{
	
}
- (void)adViewWillLeaveApplication:(GADBannerView *)bannerView{
	
}
@end
