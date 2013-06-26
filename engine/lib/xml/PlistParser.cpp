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
 * PlistParser.cpp
 *
 *  Created on: 2013/06/24
 *      Author: GClue, Inc.
 */

#include "PlistParser.h"
#include <typeinfo>

PlistParser::PlistParser() {
    m_pCurrentList = NULL;
    m_pCurrentMap = NULL;
    m_pRootList = NULL;
    m_pRootMap = NULL;
    mType = PT_NONE;
}

PlistParser::~PlistParser() {
    this->m_pRootMap = NULL;
    this->m_pCurrentMap = NULL;
}

#pragma mark - XMLParser

void PlistParser::startElement(const XML_Char *name, const XML_Char **atts) {
    
    (void) atts;
    
    string nameStr(name);
    
    if (nameStr == "dict") {
        m_pCurrentMap = new GCMap();
        
        // TODO: arrayを返すメソッドとmapを返すメソッドをわける場合はここでrootArrayに保持
        if (m_pRootMap == NULL) {
            m_pRootMap = m_pCurrentMap;
            m_pRootMap->retain();
        }
        
        mType = PT_DICT;
        PlistTagType preType = PT_NONE;
        
        if (!mTypeStack.empty()) {
            preType = mTypeStack.top();
        }
        
        if (preType == PT_ARRAY) {
            m_pCurrentList->addObject(m_pCurrentMap);
        } else if (preType == PT_DICT) {
            GCMap *preMap = mMapStack.top();
            preMap->insert(mCurrentKey, m_pCurrentMap);
        }
        
        m_pCurrentMap->release();
        
        mTypeStack.push(mType);
        mMapStack.push(m_pCurrentMap);
        
    } else if (nameStr == "array") {
        mType = PT_ARRAY;
        m_pCurrentList = new GCVector();
        
        // TODO: arrayを返すメソッドを作る場合はここでrootArrayに保持
        
        PlistTagType preType = PT_NONE;
        
        if (!mTypeStack.empty()) {
            preType = mTypeStack.top();
        }
        
        if (preType == PT_ARRAY) {
            GCVector *preList = mListStack.top();
            preList->addObject(m_pCurrentList);
        } else if (preType == PT_DICT) {
            m_pCurrentMap->insert(mCurrentKey, m_pCurrentList);
        }
        
        m_pCurrentList->release();
        
        mTypeStack.push(mType);
        mListStack.push(m_pCurrentList);
        
    } else if (nameStr == "key") {
        mType = PT_KEY;
    } else if (nameStr == "string") {
        mType = PT_STRING;
    } else if (nameStr == "real") {
        mType = PT_REAL;
    } else if (nameStr == "integer") {
        mType = PT_INT;
    } else {
        mType = PT_NONE;
    }
    
}

void PlistParser::bodyElement(const XML_Char *s, int len) {
    
    string str;
    str.append(s, len);
    
    if (mType == PT_NONE) {
        return;
    }
    
    switch (mType) {
        case PT_KEY:
            mCurrentKey = str;
            break;
        case PT_STRING:
        case PT_INT:
        case PT_REAL:
            mCurrentValue.append(str);
            break;
        default:
            break;
    }
}


void PlistParser::endElement(const XML_Char *name) {
    
    PlistTagType currentType;
    
    if (mTypeStack.empty()) {
        currentType = PT_DICT;
    } else {
        currentType = mTypeStack.top();
    }
    
    string nameStr(name);
    
    if (nameStr == "dict") {
        mTypeStack.pop();
        mMapStack.pop();
        
        if (!mMapStack.empty()) {
            m_pCurrentMap = mMapStack.top();
        }
    } else if (nameStr == "array") {
        mTypeStack.pop();
        mListStack.pop();
        if (!mListStack.empty()) {
            m_pCurrentList = mListStack.top();
        }
    } else if (nameStr == "true" || nameStr == "false") {
        
        GCString *value;
        if (nameStr == "true") {
            value = new GCString("1");
        } else {
            value = new GCString("0");
        }
        
        if (currentType == PT_DICT) {
            m_pCurrentMap->insert(mCurrentKey, value);
        } else if (currentType == PT_ARRAY) {
            m_pCurrentList->addObject(value);
        }
        
        value->release();
        
    } else if (nameStr == "string" || nameStr == "integer" || nameStr == "real") {
        
        GCString *value = new GCString(mCurrentValue);
        
        if (currentType == PT_DICT) {
            m_pCurrentMap->insert(mCurrentKey, value);
        } else if (currentType == PT_ARRAY) {
            m_pCurrentList->addObject(value);
        }
        
        value->release();
        mCurrentValue.clear();
    }
    
    mType = PT_NONE;
}

#pragma mark - static methods

GCVector* split(const char *str, const char *split) {

    GCVector *vec = new GCVector();
    string ss(str);
    string::size_type index;
    
    while ((index = ss.find(split)) != string::npos) {

        GCString *sss = new GCString(ss.substr(0, index));
        ss = ss.substr(index + 1, ss.size() - 1);
        vec->addObject(sss);
    }
    
    GCString *sss = new GCString(ss);
    vec->addObject(sss);
    
    return vec;
}

GCVector* parseSingleParenthesis(const char *data) {
    
    string tmpStr(data);
    
    string::size_type leftParPos = tmpStr.find("{");
    string::size_type rightParPos = tmpStr.find("}");
    
    if (leftParPos == string::npos || rightParPos == string::npos ||
        rightParPos < leftParPos)
    {
        LOGD("invalid format : %s", data);
        return NULL;
        
    }
    
    tmpStr = tmpStr.substr(leftParPos + 1, rightParPos - 1);
    
    leftParPos = tmpStr.find("{");
    rightParPos = tmpStr.find("}");
    string::size_type commaPos = tmpStr.find(",");
    
    if (leftParPos != string::npos || rightParPos != string::npos || commaPos == string::npos) {
        LOGD("invalid format : %s", data);
        return NULL;
    }
    
    GCVector *vec = split(tmpStr.c_str(), ",");
    if (vec->size() != 2) {
        LOGD("invalid format : %s", data);
        vec->release();
        return NULL;
    }
    
    return vec;
}

GCMap* PlistParser::createMapFromFile(const char *plist) {
    PlistParser *parser = new PlistParser();
    parser->parseFromAsset(plist);
    GCMap *map = parser->m_pRootMap;
    parser->release();
    return map;    
}

void PlistParser::formatSizeData(const char *data, PPSize &size) {
    
    size.width = 0;
    size.height = 0;
    
    GCVector *vec = parseSingleParenthesis(data);
    if (!vec) {
        return;
    }
    
    size.width = ((GCString *) vec->getObject(0))->intValue();
    size.height = ((GCString *) vec->getObject(1))->intValue();
    
    vec->release();
}

void PlistParser::formatSizeData(GCString *gString, PPSize &size) {
    formatSizeData(gString->getCString(), size);
}

void PlistParser::formatPointData(const char *data, Pointf &point) {
    
    point.x = 0;
    point.y = 0;
    
    GCVector *vec = parseSingleParenthesis(data);
    if (!vec) {
        return;
    }
    
    point.x = ((GCString *) vec->getObject(0))->floatValue();
    point.y = ((GCString *) vec->getObject(1))->floatValue();
    
    vec->release();
}

void PlistParser::formatPointData(GCString *gString, Pointf &point) {
    
    formatPointData(gString->getCString(), point);
}

void PlistParser::formatRectData(const char *data, Rectf &rect) {
    
    rect.top = 0;
    rect.bottom = 0;
    rect.right = 0;
    rect.left = 0;
    
    string str(data);
    
    string::size_type leftParPos = str.find("{");
    string::size_type rightParPos = str.rfind("}");
    
    if (leftParPos == string::npos || rightParPos == string::npos ||
        rightParPos < leftParPos)
    {
        LOGD("invalid format : %s", data);
        return;
    }
    
    str = str.substr(leftParPos + 1, rightParPos - 1);
    
    GCVector *vec = split(str.c_str(), ",");
    
    if (vec->size() != 4) {
        LOGD("invalid format : %s", data);
        return;
    }
    
    float rectParam[4];
    
    for (int i = 0; i < vec->size(); i++) {
        GCString *contents = (GCString *) vec->getObject(i);
        
        leftParPos = contents->getString().find("{");
        rightParPos = contents->getString().rfind("}");
        
        if (leftParPos == string::npos && rightParPos == string::npos) {
            LOGD("invalid format : %s", data);
            return;
        }
        
        // 正しくないデータは保証しない
        string c = contents->getString();
        string value;
        
        if (leftParPos != string::npos) {
            value = c.substr(leftParPos + 1, c.size() - 1);
        } else if (rightParPos != string::npos) {
            value = c.substr(0, rightParPos);
        }
        
        rectParam[i] = atof(value.c_str());
    }
    
    float x = rectParam[0];
    float y = rectParam[1];
    float width = rectParam[2];
    float height = rectParam[3];
    
    rect.top = y;
    rect.left = x;
    rect.right = x + width;
    rect.bottom = y + height;
    
    vec->release();
}

void PlistParser::formatRectData(GCString *gString, Rectf &rect) {
    formatRectData(gString->getCString(), rect);
}
































