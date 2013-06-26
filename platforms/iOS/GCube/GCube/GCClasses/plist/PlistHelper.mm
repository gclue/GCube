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
 * PlistHelper.mm
 *
 *  Created on: 2013/06/26
 *      Author: GClue, Inc.
 */

#include "PlistHelper.h"
#include "GCString.h"
#include "GCVector.h"
#include <string>

using namespace std;

void convertToArray(NSArray *array, GCVector *vec);
void convertToMap(NSDictionary *dict, GCMap *map);

void convertToArray(NSArray *array, GCVector *vec) {
    
    for (id obj in array) {
        
        if ([obj isKindOfClass:[NSString class]]) {
            
            GCString *value = new GCString([(NSString *) obj UTF8String]);
            vec->addObject(value);
            value->release();
            
        } else if ([obj isKindOfClass:[NSNumber class]]) {
            
            NSString *sValue = [obj stringValue];
            GCString *value = new GCString([sValue UTF8String]);
            vec->addObject(value);
            value->release();
            
        } else if ([obj isKindOfClass:[NSDictionary class]]) {
            
            GCMap *childMap = new GCMap();
            vec->addObject(childMap);
            convertToMap((NSDictionary *) obj, childMap);
            childMap->release();
            
        } else if ([obj isKindOfClass:[NSArray class]]) {
            
            GCVector *childVec = new GCVector();
            vec->addObject(childVec);
            convertToArray(obj, vec);
            childVec->release();
            
        }
        
    }
}

void convertToMap(NSDictionary *dict, GCMap *map) {
    
    NSArray *keys = [dict allKeys];
    
    for (int i = 0; i < keys.count; i++) {
        NSString *keyStr = (NSString *) [keys objectAtIndex:i];
        id obj = [dict objectForKey:keyStr];
        string key([keyStr UTF8String]);
        if ([obj isKindOfClass:[NSDictionary class]]) {
            
            GCMap *childMap = new GCMap();
            map->insert(key, childMap);
            convertToMap((NSDictionary *) obj, childMap);
            childMap->release();
            
        } else if ([obj isKindOfClass:[NSString class]]) {
            
            GCString *value = new GCString([(NSString *) obj UTF8String]);
            map->insert(key, value);
            value->release();
            
        } else if ([obj isKindOfClass:[NSNumber class]]) {
            
            NSString *sValue = [obj stringValue];
            GCString *value = new GCString([sValue UTF8String]);
            map->insert([keyStr UTF8String], value);
            value->release();
            
        } else if ([obj isKindOfClass:[NSArray class]]) {
            
            GCVector *vec = new GCVector();
            map->insert([keyStr UTF8String], vec);
            convertToArray(obj, vec);
            vec->release();
            
        }
    }
}

GCMap* createMapFromFile(const char *plistName) {
    
    NSString *fname = [NSString stringWithCString:plistName encoding:NSUTF8StringEncoding];
	NSString *path;
	if ([[NSFileManager defaultManager] fileExistsAtPath:fname]){
		// ファイルが存在したら絶対パス
		path = fname;
	} else {
		// 相対パスの場合はリソースから
		path = [[NSBundle mainBundle] pathForResource:[fname lastPathComponent] ofType:nil];
	}
    
    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:path];
    GCMap *map = new GCMap();
    convertToMap(dict, map);
    
    return map;
}
