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
 * GCMap.cpp
 *
 *  Created on: 2013/06/25
 *      Author: GClue, Inc.
 */

#include "GCMap.h"

using namespace std;

GCMap::GCMap() {
}

GCMap::~GCMap() {
    clear();
}

#pragma mark - public methods

void GCMap::insert(std::string key, GCObject *value) {
    mMap.insert(pair<string, GCObject*>(key, value));
    value->retain();
}

GCObject* GCMap::objectForKey(std::string key) {
    
    map<string, GCObject*>::iterator it = mMap.find(key);
    
    if (it != mMap.end()) {
        return (*it).second;
    }
    
    return NULL;
}

void GCMap::remove(std::string key) {
    mMap.erase(key);
}

int GCMap::size() {
    return mMap.size();
}

void GCMap::allKeys(std::string *keys) {
    
    map<string, GCObject*>::iterator it = mMap.begin();
    
    int i = 0;
    while (it != mMap.end()) {
        keys[i] = (*it).first;
        i++;
        it++;
    }
}

void GCMap::allKeys(std::vector<std::string> &keys)
{
    map<string, GCObject*>::iterator it = mMap.begin();
    while (it != mMap.end()) {
		keys.push_back((*it).first);
        it++;
    }
}

void GCMap::clear() {
    
    map<string, GCObject*>::iterator it = mMap.begin();
    
    while (it != mMap.end()) {
        GCObject *obj = (*it).second;
        obj->release();
        it++;
    }
    
    mMap.clear();
}