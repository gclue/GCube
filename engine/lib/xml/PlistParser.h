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
 * PlistParser.h
 *
 *  Created on: 2013/06/24
 *      Author: GClue, Inc.
 */
#ifndef __GCube__PlistParser__
#define __GCube__PlistParser__

#include "XMLParser.h"
#include "GCObject.h"
#include "GCVector.h"
#include "GCMap.h"
#include "GCString.h"
#include "defines.h"
#include <string>
#include <stack>

using namespace std;

typedef enum {
    PT_NONE = 0,
    PT_KEY,
    PT_DICT,
    PT_INT,
    PT_REAL,
    PT_STRING,
    PT_ARRAY,
} PlistTagType;

typedef struct {
    int width;
    int height;
} PPSize;

class PlistParser : public XMLParser, public GCObject {
private:

    GCMap *m_pCurrentMap;
    GCVector *m_pRootList;
    GCVector *m_pCurrentList;
    stack<PlistTagType> mTypeStack;
    stack<GCMap*> mMapStack;
    stack<GCVector*> mListStack;
    PlistTagType mType;
    string mCurrentKey;
    string mCurrentValue;
    GCMap *m_pRootMap;
    
    PlistParser();    
public:
    virtual ~PlistParser();
    
#pragma mark - XMLParser Delegate
    virtual void startElement(const XML_Char *name, const XML_Char *atts[]);
	virtual void endElement(const XML_Char *name);
	virtual void bodyElement(const XML_Char *s, int len);
    
    static GCMap* createMapFromFile(const char *plist);
    
#pragma mark - format methods
    static void formatSizeData(const char *data, PPSize &size);
    static void formatSizeData(GCString *data, PPSize &size);
    static void formatPointData(const char *data, Pointf &point);
    static void formatPointData(GCString *data, Pointf &point);
    static void formatRectData(const char *data, Rectf &rect);
    static void formatRectData(GCString *data, Rectf &rect);    
};

#endif /* defined(__LandingCat2__PlistParser__) */
