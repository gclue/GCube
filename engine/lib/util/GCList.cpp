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
 * GCList.cpp
 *
 *  Created on: 2013/06/25
 *      Author: GClue, Inc.
 */


#include "GCList.h"

using namespace std;

GCList::GCList() {
    
}

GCList::~GCList() {
    clear();
}

#pragma mark - public methods

void GCList::addObject(GCObject *value) {
    mList.push_back(value);
    value->retain();
    
}

GCObject* GCList::getObject(int index) {
    
    list<GCObject*>::iterator it = mList.begin();

    int i = 0;
    GCObject *obj = NULL;
    while (it != mList.end()) {
        
        if (i == index) {
            obj = (*it);
            break;
        }
        
        i++;
        it++;
    }
    
    return obj;
}

void GCList::removeAt(int index) {
    list<GCObject*>::iterator it = mList.begin();
    
    int i = 0;
    while (it != mList.end()) {
        
        if (i == index) {
            
            GCObject *obj = (*it);
            obj->release();
            mList.erase(it);
            break;
        }
        
        i++;
        it++;
    }
}

int GCList::size() {
    return mList.size();
}

void GCList::clear() {
    list<GCObject*>::iterator it = mList.begin();
    
    while (it != mList.end()) {
        
        GCObject *obj = (*it);
        obj->release();
        it++;
    }

    mList.clear();
}