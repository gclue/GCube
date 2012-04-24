//
//  AdMobViewController.h
//  GCube
//
//  Created by Hitoshi Haga on 12/04/23.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GADBannerView.h"

//AdMobのビューコントロラークラス.
@interface AdMobViewController : UIViewController<GADBannerViewDelegate>{
	//バナーのビュー
	GADBannerView *bannerView_;
}

//ユニットID
@property (nonatomic, assign) NSString *unitID;

//初期化関数
//unitid 引数にユニットIDを渡します.
-(id)customInit:(NSString*)unitid;
-(void)changePosition:(int)pos: (float)width: (float)height;


@end
