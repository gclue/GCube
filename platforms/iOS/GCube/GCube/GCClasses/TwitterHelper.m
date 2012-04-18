//
//  TwitterHelper.m
//  GCube
//
//  Created by Hitoshi Haga on 12/04/18.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#import "TwitterHelper.h"

#import "SA_OAuthTwitterEngine.h"


#define kOAuthConsumerKey				@"ByRxuc6bUEaNjKhPdnFg"		//REPLACE ME
#define kOAuthConsumerSecret			@"yvtlEEsNyIVmzm6HePK7aFeZhtfWBE5DJb4AuiuHE"		//REPLACE ME

@implementation TwitterHelper

@synthesize responseBlock;
@synthesize tweetResponseBlock;




//=============================================================================================================================
#pragma mark SA_OAuthTwitterEngineDelegate
- (void) storeCachedTwitterOAuthData: (NSString *) data forUsername: (NSString *) username {
	NSUserDefaults			*defaults = [NSUserDefaults standardUserDefaults];
	
	[defaults setObject: data forKey: @"authData"];
	[defaults synchronize];
}

- (NSString *) cachedTwitterOAuthDataForUsername: (NSString *) username {
	return [[NSUserDefaults standardUserDefaults] objectForKey: @"authData"];
}

//=============================================================================================================================
#pragma mark SA_OAuthTwitterControllerDelegate
- (void) OAuthTwitterController: (SA_OAuthTwitterController *) controller authenticatedWithUsername: (NSString *) username {
	NSLog(@"Authenicated for %@", username);
	if (self.responseBlock) {
		self.responseBlock(0);
	}
	
	
}

- (void) OAuthTwitterControllerFailed: (SA_OAuthTwitterController *) controller {
	NSLog(@"Authentication Failed!");
	if (self.responseBlock) {
		self.responseBlock(1);
	}
	
}

- (void) OAuthTwitterControllerCanceled: (SA_OAuthTwitterController *) controller {
	NSLog(@"Authentication Canceled.");
	if (self.responseBlock) {
		self.responseBlock(2);
	}
	
}

//=============================================================================================================================
#pragma mark TwitterEngineDelegate
- (void) requestSucceeded: (NSString *) requestIdentifier {
	NSLog(@"Request %@ succeeded", requestIdentifier);
	self.tweetResponseBlock(0);
}

- (void) requestFailed: (NSString *) requestIdentifier withError: (NSError *) error {
	NSLog(@"Request %@ failed with error: %@", requestIdentifier, error);
	self.tweetResponseBlock(1);
}



//=============================================================================================================================
#pragma mark ViewController Stuff
- (void)dealloc {
	[_engine release];
    [super dealloc];
}


- (void) viewDidAppear: (BOOL)animated {
	if (_engine) return;
	_engine = [[SA_OAuthTwitterEngine alloc] initOAuthWithDelegate: self];
	_engine.consumerKey = kOAuthConsumerKey;
	_engine.consumerSecret = kOAuthConsumerSecret;
	
	UIViewController *controller = [SA_OAuthTwitterController controllerToEnterCredentialsWithTwitterEngine: _engine delegate: self];
	
	if (controller) 
		[self presentModalViewController: controller animated: YES];
	else {
		if (self.responseBlock) {
			self.responseBlock(3);
			
		}
		
		//		[_engine sendUpdate: [NSString stringWithFormat: @"Already Updated. %@", [NSDate date]]];
		
	}
	
	[controller release];
	
}


- (BOOL) tweet:(NSString *)text {
	[_engine sendUpdate: text];
}

- (void) authenticate {
	if (_engine) {
		NSLog(@"_engine is exist");
	}else {
		NSLog(@"new engine");
		_engine = [[SA_OAuthTwitterEngine alloc] initOAuthWithDelegate: self];
		_engine.consumerKey = kOAuthConsumerKey;
		_engine.consumerSecret = kOAuthConsumerSecret;
	}
	
	UIViewController *controller = [SA_OAuthTwitterController controllerToEnterCredentialsWithTwitterEngine: _engine delegate: self];
	
	if(controller) {
		[self presentModalViewController: controller animated: YES];
	}else {
		responseBlock(3);
		[controller release];
	}
	
	

}


@end

