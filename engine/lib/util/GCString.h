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
 * GCString.h
 *
 *  Created on: 2013/06/25
 *      Author: GClue, Inc.
 */

#ifndef __GCube__GCString__
#define __GCube__GCString__

#include "GCObject.h"
#include <string>

/**
 std::stringを保持するクラス
 */
class GCString : public GCObject {
private:
    std::string mString;
public:
    
    GCString(std::string str);
    GCString(const char *str);
    
    virtual ~GCString();
    
    /**
     文字列をintに変換して取得する
     
     @return 文字列のint値
     */
    int intValue();
    
    /**
     文字列をfloatに変換して取得する

     @return 文字列のfloat値     
     */
    float floatValue();
    
    /**
     文字列をdoubleに変換して取得する
     
     @return 文字列のfloat値
     */
    double doubleValue();
    
    const std::string& getString();
    const char* getCString();
};

#endif /* defined(__LandingCat2__GCString__) */
