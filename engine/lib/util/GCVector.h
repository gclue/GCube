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

/*
 * GCVector.h
 *
 *  Created on: 2013/06/25
 *      Author: GClue, Inc.
 */

#ifndef __GCube__GCVector__
#define __GCube__GCVector__

#include <vector>
#include "GCObject.h"

class GCVector : public GCObject {
private:
    std::vector<GCObject*> mVector;
public:
    GCVector();
    virtual ~GCVector();
    
    /**
     オブジェクトをリストに追加する。
     
     @param value 追加するオブジェクト
     */
    void addObject(GCObject *value);
    
    /**
     指定された位置のオブジェクトを取得する。
     
     @param index 取得するオブジェクトのインデックス
     @return 指定されたオブジェクト。
     */
    GCObject* getObject(int index);
    
    /**
     要素の数を返す
     */
    int size();
    
    /**
     指定したインデックスのオブジェクトを削除する
     
     @param index 削除するオブジェクトのインデックス
     */
    void removeAt(int index);
    
    /**
     リストをクリアする。
     */
    void clear();

};

#endif /* defined(__GCube__GCVector__) */
