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

#ifndef ANDROID_DEFINES_H
#define ANDROID_DEFINES_H

#include <stdio.h>
#include <stdlib.h>

#define DELETE(a) if (a) {delete a;a=NULL;}
#define FREE(a) if (a) {free(a);a=NULL;}
#define RELEASE(a) if (a) {a->release(); a=NULL;}

#define SWAP(x, y, type) { type temp = (x); x = y; y = temp; }

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define DEGREES_TO_RADIANS(angle) ((angle) / 180.0 * M_PI)
#define RADIANS_TO_DEGREES(radians) ((radians) * (180.0 / M_PI))

#ifdef __cplusplus
extern "C" {
#endif
	
struct Ratio {
	double x;
	double y;
};

struct Position {
	double x;
	double y;
};
	
//struct Point {
//	int x;
//	int y;
//};
//typedef struct Point Point;
	
struct Pointf {
	float x;
	float y;
};
typedef struct Pointf Pointf;

struct Point3f {
	float x;
	float y;
	float z;
};
typedef struct Point3f Point3f;
	
typedef struct Rectf {
	float left;
	float top;
	float right;
	float bottom;
} Rectf;
	
typedef struct TextureParam {
	int max;
	int stx;
	int sty;
	int ttx;
	int tty;
} TextureParam;
	
typedef struct Colorf {
	float r;
	float g;
	float b;
	float a;
} Colorf;
	
/**
 * タッチイベントのタイプを定義します.
 */
enum TouchEventAction {
	touchDown = 0,		//!< タッチダウン
	touchUp = 1,		//!< タッチアップ
	touchMove = 2,		//!< タッチムーブ
	touchCancel = 3,	//!< タッチキャンセル
};
	
/**
 * タッチイベント.
 */
typedef struct _TouchEvent {
	float x;			//!< x座標
	float y;			//!< y座標
	int type;			//!< イベントタイプ
	long time;			//!< イベントが発生した時間
} TouchEvent;
	
/**
 * キーイベント.
 */
typedef struct _KeyEvent {
	int type;
	int keycode;
	long time;
} KeyEvent;
	
/**
 * センサーイベント.
 */
typedef struct _SensorEvent {
	Point3f acceleration;
	Point3f accelerationIncludingGravity;
	Point3f rotationRate;
} SensorEvent;
	
	
inline Rectf makeRect(int x, int y, int w, int h) {
	Rectf rect;
	rect.top = y;
	rect.bottom = rect.top + h;
	rect.left = x;
	rect.right = rect.left + w;
	return rect;
}
	
// 乱数
inline int GetRandom(int min,int max) {
	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}
	
/**
 * -1.0 〜 1.0の間の乱数を作成します.
 * @return 乱数
 */
inline float GetRandom1_1() {
	return ((2.0f*(rand()/(float)RAND_MAX))-1.0f);
}
	
inline float clampf(float value, float min_inclusive, float max_inclusive) {
	if (min_inclusive > max_inclusive) {
		SWAP(min_inclusive, max_inclusive, float);
	}
	return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}
	
#ifdef __cplusplus
};
#endif

#endif // ANDROID_DEFINES_H
