//
//  TwitterHelper.m
//  GCube
//
//  Created by Hitoshi Haga on 12/04/18.
//  Copyright (c) 2012年 GClue, Inc. All rights reserved.
//

#import "TwitterHelper.h"

#import "SA_OAuthTwitterEngine.h"


#define kOAuthConsumerKey				@"REPLACE_ME"		//REPLACE ME
#define kOAuthConsumerSecret			@"REPLACE_ME"		//REPLACE ME

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
		self.responseBlock(TWITTER_HELPER_EVENT_AUTH_SUCCESS);
	}
	
	
}

- (void) OAuthTwitterControllerFailed: (SA_OAuthTwitterController *) controller {
	NSLog(@"Authentication Failed!");
	if (self.responseBlock) {
		self.responseBlock(TWITTER_HELPER_EVENT_AUTH_FAILED);
	}
	
}

- (void) OAuthTwitterControllerCanceled: (SA_OAuthTwitterController *) controller {
	NSLog(@"Authentication Canceled.");
	if (self.responseBlock) {
		self.responseBlock(TWITTER_HELPER_EVENT_AUTH_FAILED);
	}
	
}

//=============================================================================================================================
#pragma mark TwitterEngineDelegate
- (void) requestSucceeded: (NSString *) requestIdentifier {
	NSLog(@"Request %@ succeeded", requestIdentifier);
	self.tweetResponseBlock(TWITTER_HELPER_EVENT_POST_SUCCESS);
}

- (void) requestFailed: (NSString *) requestIdentifier withError: (NSError *) error {
	NSLog(@"Request %@ failed with error: %@", requestIdentifier, error);
	self.tweetResponseBlock(TWITTER_HELPER_EVENT_POST_FAILED);
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
			self.responseBlock(TWITTER_HELPER_EVENT_ALREADY_AUTHORIZED);
			
		}
		
		//		[_engine sendUpdate: [NSString stringWithFormat: @"Already Updated. %@", [NSDate date]]];
		
	}
	
	[controller release];
	
}


- (void) tweet:(NSString *)text {
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
		responseBlock(TWITTER_HELPER_EVENT_ALREADY_AUTHORIZED);
//		[controller release];
	}
	
	

}


@end

