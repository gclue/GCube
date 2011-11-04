/*
 * The MIT License (MIT)
 * Copyright (c) 2011 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//
//  AEViewController.m
//  AizuEngine
//
//  Created by Takashi Tsuchiya on 11/10/14.
//  Copyright (c) 2011年 GClue, inc. All rights reserved.
//

#pragma mark - APIGlue

#include "ApplicationController.h"
#include "APIGlue.h"
#include "Figure.h"
#include "Texture.h"

// コントローラーインスタンス
static ApplicationController *controller = NULL;

/**
 * Figureを読み込みます.
 * @param[in] name フィギュア名前
 */
Figure* GCLoadFigure(const char *fname) {
	return NULL;
}

/**
 * リソースファイルを読み込みます.
 * @param[in] fileName ファイル名
 * @return charのvectorオブジェクト
 */
std::vector<char>* GCLoadAsset(const char *fileName) {
	LOGD("AELoadAsset: %s", fileName);
    NSString *fname = [NSString stringWithCString:fileName encoding:NSUTF8StringEncoding];
    NSString *path = [[NSBundle mainBundle] pathForResource:[fname lastPathComponent] ofType:nil];
    const char *source;
    NSString *fnet = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
    source = [fnet UTF8String];
    
    std::vector<char> *buff = new std::vector<char>();
    int max = [fnet lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
    for (int i = 0; i < max; ++i) {
        buff->push_back(source[i]);
    }
    return buff;
}

/**
 * SE,BGM再生
 * mode: 0:BGM再生
 *       1:BGM停止
 *       2:SE登録
 *       3:SE再生
 * @param[in] fname ファイル名
 * @param[in] mode モード
 */
void GCSoundEvent(const char *fileName, int mode) {
}


/**
 * テクスチャを読み込みます.
 * @param[in] fileName ファイル名
 * @return テクスチャオブジェクト
 */
bool GCLoadTexture(Texture *texture, const char *fileName) {
	LOGD("AELoadTexture: %s", fileName);
    
    // 画像読み込み
    NSString *fname = [NSString stringWithCString:fileName encoding:NSUTF8StringEncoding];
	CGImageRef img = [[UIImage imageNamed:[fname lastPathComponent]] CGImage];
    
    CGContextRef	texContext;
    GLubyte			*texData;
    
    // サイズ取得
    size_t width = CGImageGetWidth(img);
    size_t height = CGImageGetHeight(img);
    
    // メモリ確保
    texData = (GLubyte *) malloc(width * height * 4);
    // texDataに対応したBitmapContext作成
    texContext = CGBitmapContextCreate(texData, width, height, 8, width * 4, CGImageGetColorSpace(img), kCGImageAlphaPremultipliedLast);
    // Context(texData)に描画
    CGRect rect = CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height);
    CGContextClearRect(texContext, rect);
    CGContextDrawImage(texContext, rect, img);
    // Contextはもう要らないので解放
    CGContextRelease(texContext);

	// テクスチャ作成
	texture->setImageData(texData, width, height);
    
    // 解放
    free(texData);
    
	return true;
}

/**
 * イベントを配送します.
 * @param[in] type イベントタイプ
 * @param[in] param1 イベントパラメータ
 * @param[in] param2 イベントパラメータ
 * @param[in] param3 イベントパラメータ
 * @param[in] param4 イベントパラメータ
 * @param[in] param5 イベントパラメータ
 */
void GCSendGameEvent(int type, int param1, int param2, int param3, int param4, const char *param5) {
    LOGD( "**AESendGameEvent**:%d, %d, %d, %d, %d, %s", type, param1, param2, param3, param4, param5);
}


#pragma mark - GCViewController

#import "GCViewController.h"
#import <QuartzCore/QuartzCore.h>
#import "EAGLView.h"

@interface GCViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end


@implementation GCViewController

@synthesize animating;
@synthesize context;
@synthesize displayLink;

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    NSLog(@"GCViewController:viewDidLoad");
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    } else if (![EAGLContext setCurrentContext:self.context]) {
        NSLog(@"Failed to set ES context current");
    }
    
    EAGLView *view = (EAGLView *)self.view;
    view.context = self.context;
    [view setFramebuffer];
    
    animating = FALSE;
    animationFrameInterval = 2;
    self.displayLink = nil;
    
	if (!controller) {
		controller = ApplicationController::getInstance();
		CGRect screenSize = [[UIScreen mainScreen] bounds];
        controller->resize(screenSize.size.width, screenSize.size.height);
		controller->resetup();
		GCInitApplicationController(controller);
	} else {
		controller->resetup();
	}
    
    lastTime = CFAbsoluteTimeGetCurrent();
}

- (void)viewDidUnload
{
    NSLog(@"GCViewController:viewDidUnload");
    
	[super viewDidUnload];
	
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}


#pragma mark - Events

// タッチイベント
- (void)toucheEvent:(NSSet *)touches withEvent:(UIEvent *)event withType:(int)type {
	UITouch *touch = [touches anyObject];
	CGPoint location = [touch locationInView:self.view];
	if (controller) {
		controller->onTouch(type, location.x, location.y, touch.timestamp);
	}
}

// タッチ開始イベント
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[self toucheEvent:touches withEvent:event withType:0];
}

// タッチ移動イベント
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	[self toucheEvent:touches withEvent:event withType:2];
}

// タッチ終了イベント
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	[self toucheEvent:touches withEvent:event withType:1];
}

// タッチキャンセルイベント
//- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
//}


#pragma mark - DisplayLink

- (void)drawFrame
{
    CFTimeInterval interval = CFAbsoluteTimeGetCurrent() - lastTime;
    [(EAGLView *)self.view setFramebuffer];
    
	if (controller) {
		controller->step(interval);
	} else {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    [(EAGLView *)self.view presentFramebuffer];
    lastTime = CFAbsoluteTimeGetCurrent();
}


#pragma mark - Properties

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}


#pragma mark - Public method

- (void)startAnimation
{
    NSLog(@"startAnimation");
    if (!animating) {
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)suspend
{
	if (controller) {
		controller->onPause();
	}
}

- (void)resume
{
	if (controller) {
		controller->onResume();
	}
}


@end
