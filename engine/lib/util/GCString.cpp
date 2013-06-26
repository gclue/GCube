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
 * GCString.cpp
 *
 *  Created on: 2013/06/25
 *      Author: GClue, Inc.
 */


#include "GCString.h"

using namespace std;

GCString::GCString(string str) {
    this->mString = str;
}

GCString::GCString(const char *str) {
    this->mString.append(str);
}

GCString::~GCString() {}


const string& GCString::getString() {
    return mString;
}

const char* GCString::getCString() {
    return mString.c_str();
}

#pragma mark - parse methods

int GCString::intValue() {
    return atoi(mString.c_str());
}

float GCString::floatValue() {
    return (float) atof(mString.c_str());
}

double GCString::doubleValue() {
    return atof(mString.c_str());
}