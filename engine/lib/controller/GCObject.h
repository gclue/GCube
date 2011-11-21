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
//  GCObject.h
//

#ifndef GCObject_h
#define GCObject_h

#include "Log.h"

/**
 * 基底オブジェクト.
 * 現在は参照カウンタのみ実装
 */
class GCObject {
private:
	int _refcount;
	
public:
	
	/**
	 * コンストラクタ.
	 */
	GCObject() {_refcount=1;}
	
	/**
	 * コピーコンストラクタ.
	 */
	GCObject(const GCObject& obj) {_refcount=1;}

	/**
	 * デストラクタ.
	 */
	virtual ~GCObject(){_refcount=0;};
	
	/**
	 * 参照カウンタを返します.
	 * @return 参照カウンタ.
	 */
	int retainCount() {return _refcount;};
	
	/**
	 * 参照カウンタを１つ増やします.
	 * @return 参照カウンタ.
	 */
	int retain() {return ++_refcount;};
	
	/**
	 * 参照カウンタを減らします.
	 * カウンタが０になったら削除されます.
	 */
	int release() {
		_refcount--;
		if (_refcount <= 0) {
			delete this;
		}
		return _refcount;
	};
};


#endif
