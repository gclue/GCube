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
 * XMLParser.cpp
 *
 *  Created on: 2011/09/17
 *      Author: GClue, Inc.
 */
#include "XMLParser.h"
#include "AssetManager.h"
#include "Log.h"

void XMLParser_startElement(void *userData, const XML_Char *name, const XML_Char *atts[]);
void XMLParser_startElement(void *userData, const XML_Char *name, const XML_Char *atts[]) {
	XMLParser *loader = (XMLParser *) userData;
	loader->startElement(name, atts);
}

void XMLParser_endElement(void *userData, const XML_Char *name);
void XMLParser_endElement(void *userData, const XML_Char *name) {
	XMLParser *loader = (XMLParser *) userData;
	loader->endElement(name);
}

void XMLParser_bodyElement(void *userData, const XML_Char *s, int len);
void XMLParser_bodyElement(void *userData, const XML_Char *s, int len) {
	XMLParser *loader = (XMLParser *) userData;
	loader->bodyElement(s, len);
}

//////////////////////////////////////////////////////////////////////////////////////
// XMLParserの実装
//////////////////////////////////////////////////////////////////////////////////////

XMLParser::XMLParser() {
}

XMLParser::~XMLParser() {
}

//////////////////////////////////////////////////////////////////////////////////////
// publicな関数
//////////////////////////////////////////////////////////////////////////////////////

int XMLParser::parseFromAsset(const char *fileName) {
	AssetManager mgr = AssetManager::getInstance();
	std::vector<char> *fdata = mgr.open(fileName);
	int result = -1;
	if (fdata != NULL) {
		fdata->push_back('\0');
        const char *data = (const char *) &(*fdata)[0];
		result = parse(data, fdata->size());
		delete fdata;
	}
	return result;
}

int XMLParser::parse(const char *data, int len) {
	XML_Parser parser;
	if ((parser = XML_ParserCreate(NULL)) == NULL) {
		return XML_ERROR_NO_MEMORY;
	}
	XML_SetUserData(parser, (void *) this);
	XML_SetElementHandler(parser, XMLParser_startElement, XMLParser_endElement);
	XML_SetCharacterDataHandler(parser, XMLParser_bodyElement);

	if ((XML_Parse(parser, data, len, true)) == XML_STATUS_ERROR) {
		return XML_GetErrorCode(parser);
	}
	XML_ParserFree(parser);

	return XML_ERROR_NONE;
}
