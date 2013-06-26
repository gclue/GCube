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
 * GCVector.cpp
 *
 *  Created on: 2013/06/25
 *      Author: GClue, Inc.
 */


#include "GCVector.h"

using namespace std;

GCVector::GCVector() {
    
}

GCVector::~GCVector() {
    clear();
}

#pragma mark - public methods

void GCVector::addObject(GCObject *value) {
    mVector.push_back(value);
    value->retain();
    
}

GCObject* GCVector::getObject(int index) {
    return mVector.at(index);
}

void GCVector::removeAt(int index) {
    
    vector<GCObject*>::iterator it = mVector.begin();
    
    int i = 0;
    
    while (it != mVector.end()) {
        
        if (i == index) {
            GCObject *obj = (*it);
            obj->release();
            mVector.erase(it);
            break;
        }
        
        i++;
        it++;
    }
}

int GCVector::size() {
    return mVector.size();
}

void GCVector::clear() {
    for (int i = 0; i < mVector.size(); i++) {
        GCObject *obj = mVector.at(i);
        obj->release();
    }
    
    mVector.clear();
}