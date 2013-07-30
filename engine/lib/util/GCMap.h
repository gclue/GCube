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
 * GCMap.h
 *
 *  Created on: 2013/06/25
 *      Author: GClue, Inc.
 */


#ifndef __GCube__GCMap__
#define __GCube__GCMap__

#include <map>
#include <string>
#include <vector>
#include "GCObject.h"

class GCMap : public GCObject {
private:
    std::map<std::string, GCObject*> mMap;
public:
    GCMap();
    virtual ~GCMap();
    
    /**
     マップにオブジェクトを追加する
     
     @param key キーとなる文字列
     @param value 追加する値
     */
    void insert(std::string key, GCObject *value);
    
    /**
     指定されたキーから値を取り出す。
     
     @param key キーとなる文字列
     @return 指定したキーに一致する値があればそのオブジェクトを返す。一致する物がない場合はNULLを返す。
     */
    GCObject* objectForKey(std::string key);
    
    /**
     指定されたキーの値を削除する
     
     @param key キーとなる文字列
     */
    void remove(std::string key);
    
    /**
     要素の数を返す
     */
    int size();
    
    /**
     全てのキーを取得する
     
     @param keys キーを格納するstringクラスの配列
     */
    void allKeys(std::string *keys);
    
	/**
	 * すべてのキーを取得します.
	 * @param[out] keys キーを格納するvectorクラス
	 */
	void allKeys(std::vector<std::string> &keys);
	
    /**
     マップをクリアする
     */
    void clear();
};

#endif /* defined(__GCube__GCMap__) */
