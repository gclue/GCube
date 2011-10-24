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
 * ColladaLoader.cpp
 *
 *  Created on: 2011/08/25
 *      Author: GClue, Inc.
 */
#include "ColladaLoader.h"
#include "Collada.h"
#include "Figure.h"

#include "Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * タグIDを定義します.
 */
enum {
	TAG_NONE = 0,
	TAG_COLLADA,
	TAG_ASSET,
	TAG_CONTRIBUTOR,
	TAG_AUTHOR,
	TAG_AUTHORING_TOOL,
	TAG_COMMENTS,
	TAG_COPYRIGHT,
	TAG_SOURCE_DATA,
	TAG_CREATED,
	TAG_MODIFIED,
	TAG_UINT,
	TAG_UP_AXIS,
	TAG_LIBRARY_EFFECTS,
	TAG_EFFECT,
	TAG_PROFILE_COMMON,
	TAG_NEWPARAM,
	TAG_SURFACE,
	TAG_SAMPLER2D,
	TAG_SOURCE,
	TAG_LIBRARY_IMAGES,
	TAG_IMAGE,
	TAG_INIT_FROM,
	TAG_LIBRARY_MATERIALS,
	TAG_MATERIAL,
	TAG_INSTANCE_EFFECT,
	TAG_LIBRARY_GEOMETRIES,
	TAG_GEOMETRY,
	TAG_MESH,
	TAG_FLOAT_ARRAY,
	TAG_TECHNIQUE_COMMON,
	TAG_ACCESSOR,
	TAG_PARAM,
	TAG_VERTICES,
	TAG_TRIANGLES,
	TAG_INPUT,
	TAG_P,
};

/**
 * 属性IDを定義します.
 */
enum {
	ATTR_ID = 0,
	ATTR_TYPE,
	ATTR_NAME,
	ATTR_COUNT,
	ATTR_MATERIAL,
	ATTR_SOURCE,
	ATTR_SEMANTIC,
	ATTR_OFFSET,
	ATTR_SID,
	ATTR_URL,
};

#define TXT_TAG_COLLADA "COLLADA"
#define TXT_TAG_ASSET "asset"
#define TXT_TAG_CONTRIBUTOR "contributor"
#define TXT_TAG_AUTHOR "author"
#define TXT_TAG_AUTHORING_TOOL "authoring_tool"
#define TXT_TAG_COMMENTS "comments"
#define TXT_TAG_COPYRIGHT "copyright"
#define TXT_TAG_SOURCE_DATA "source_data"
#define TXT_TAG_CREATED "created"
#define TXT_TAG_MODIFIED "modified"
#define TXT_TAG_UINT "unit"
#define TXT_TAG_UP_AXIS "up_axis"
#define TXT_TAG_LIBRARY_EFFECTS "library_effects"
#define TXT_TAG_EFFECT "effect"
#define TXT_TAG_PROFILE_COMMON "profile_COMMON"
#define TXT_TAG_NEWPARAM "newparam"
#define TXT_TAG_SURFACE "surface"
#define TXT_TAG_SAMPLER2D "sampler2D"
#define TXT_TAG_SOURCE "source"
#define TXT_TAG_LIBRARY_IMAGES "library_images"
#define TXT_TAG_IMAGE "image"
#define TXT_TAG_INIT_FROM "init_from"
#define TXT_TAG_LIBRARY_MATERIALS "library_materials"
#define TXT_TAG_MATERIAL "material"
#define TXT_TAG_INSTANCE_EFFECT "instance_effect"
#define TXT_TAG_LIBRARY_GEOMETRIES "library_geometries"
#define TXT_TAG_GEOMETRY "geometry"
#define TXT_TAG_MESH "mesh"
#define TXT_TAG_FLOAT_ARRAY "float_array"
#define TXT_TAG_TECHNIQUE_COMMON "technique_common"
#define TXT_TAG_ACCESSOR "accessor"
#define TXT_TAG_PARAM "param"
#define TXT_TAG_VERTICES "vertices"
#define TXT_TAG_TRIANGLES "triangles"
#define TXT_TAG_INPUT "input"
#define TXT_TAG_P "p"

#define TXT_ATTR_ID "id"
#define TXT_ATTR_TYPE "type"
#define TXT_ATTR_NAME "name"
#define TXT_ATTR_COUNT "count"
#define TXT_ATTR_MATERIAL "material"
#define TXT_ATTR_SOURCE "source"
#define TXT_ATTR_SEMANTIC "semantic"
#define TXT_ATTR_OFFSET "offset"
#define TXT_ATTR_SID "sid"
#define TXT_ATTR_URL "url"

struct TagDef {
	std::string name;
	int id;
};
typedef struct TagDef TagDef;

struct AttrDef {
	std::string name;
	int id;
};
typedef struct AttrDef AttrDef;

static const TagDef tags[] = {
	{TXT_TAG_COLLADA, TAG_COLLADA},
	{TXT_TAG_ASSET, TAG_ASSET},
	{TXT_TAG_CONTRIBUTOR, TAG_CONTRIBUTOR},
	{TXT_TAG_AUTHOR, TAG_AUTHOR},
	{TXT_TAG_AUTHORING_TOOL, TAG_AUTHORING_TOOL},
	{TXT_TAG_COMMENTS, TAG_COMMENTS},
	{TXT_TAG_COPYRIGHT, TAG_COPYRIGHT},
	{TXT_TAG_SOURCE_DATA, TAG_SOURCE_DATA},
	{TXT_TAG_CREATED, TAG_CREATED},
	{TXT_TAG_MODIFIED, TAG_MODIFIED},
	{TXT_TAG_UINT, TAG_UINT},
	{TXT_TAG_UP_AXIS, TAG_UP_AXIS},
	{TXT_TAG_LIBRARY_EFFECTS, TAG_LIBRARY_EFFECTS},
	{TXT_TAG_EFFECT, TAG_EFFECT},
	{TXT_TAG_PROFILE_COMMON, TAG_PROFILE_COMMON},
	{TXT_TAG_NEWPARAM, TAG_NEWPARAM},
	{TXT_TAG_SURFACE, TAG_SURFACE},
	{TXT_TAG_SAMPLER2D, TAG_SAMPLER2D},
	{TXT_TAG_SOURCE, TAG_SOURCE},
	{TXT_TAG_LIBRARY_IMAGES, TAG_LIBRARY_IMAGES},
	{TXT_TAG_IMAGE, TAG_IMAGE},
	{TXT_TAG_INIT_FROM, TAG_INIT_FROM},
	{TXT_TAG_LIBRARY_MATERIALS, TAG_LIBRARY_MATERIALS},
	{TXT_TAG_MATERIAL, TAG_MATERIAL},
	{TXT_TAG_INSTANCE_EFFECT, TAG_INSTANCE_EFFECT},
	{TXT_TAG_LIBRARY_GEOMETRIES, TAG_LIBRARY_GEOMETRIES},
	{TXT_TAG_GEOMETRY, TAG_GEOMETRY},
	{TXT_TAG_MESH, TAG_MESH},
	{TXT_TAG_FLOAT_ARRAY, TAG_FLOAT_ARRAY},
	{TXT_TAG_TECHNIQUE_COMMON, TAG_TECHNIQUE_COMMON},
	{TXT_TAG_ACCESSOR, TAG_ACCESSOR},
	{TXT_TAG_PARAM, TAG_PARAM},
	{TXT_TAG_VERTICES, TAG_VERTICES},
	{TXT_TAG_TRIANGLES, TAG_TRIANGLES},
	{TXT_TAG_INPUT, TAG_INPUT},
	{TXT_TAG_P, TAG_P},
	{"", -1}
};

static const AttrDef attrs[] = {
	{TXT_ATTR_ID, ATTR_ID},
	{TXT_ATTR_TYPE, ATTR_TYPE},
	{TXT_ATTR_NAME, ATTR_NAME},
	{TXT_ATTR_COUNT, ATTR_COUNT},
	{TXT_ATTR_MATERIAL, ATTR_MATERIAL},
	{TXT_ATTR_SOURCE, ATTR_SOURCE},
	{TXT_ATTR_SEMANTIC, ATTR_SEMANTIC},
	{TXT_ATTR_OFFSET, ATTR_OFFSET},
	{TXT_ATTR_SID, ATTR_SID},
	{TXT_ATTR_URL, ATTR_URL},
	{"", -1}
};

ColladaLoader::ColladaLoader() {
}

ColladaLoader::~ColladaLoader() {
}
void ColladaLoader_startElement(void *userData, const XML_Char *name, const XML_Char *atts[]);
void ColladaLoader_startElement(void *userData, const XML_Char *name, const XML_Char *atts[]) {
	LOGE("startElement: <%s>", name);
	for (int i = 0; atts[i]; i += 2) {
		LOGD("Attribute %s='%s'\n", atts[i], atts[i + 1]);
	}

	ColladaLoader *loader = (ColladaLoader *) userData;
	loader->startElement(name, atts);
}

void ColladaLoader_endElement(void *userData, const XML_Char *name);
void ColladaLoader_endElement(void *userData, const XML_Char *name) {
	LOGE("endElement: </%s>", name);

	ColladaLoader *loader = (ColladaLoader *) userData;
	loader->endElement(name);
}

void ColladaLoader_bodyElement(void *userData, const XML_Char *s, int len);
void ColladaLoader_bodyElement(void *userData, const XML_Char *s, int len) {
	ColladaLoader *loader = (ColladaLoader *) userData;
	loader->bodyElement(s, len);
}


//////////////////////////////////////////////////////////////////////////////////////
// publicな関数
//////////////////////////////////////////////////////////////////////////////////////

void ColladaLoader::parse(const char *data, int len) {
	collada = new Collada();

	XML_Parser parser;
	if ((parser = XML_ParserCreate(NULL)) == NULL) {
		// エラー処理
		return;
	}
	XML_SetUserData(parser, (void *) this);
	XML_SetElementHandler(parser, ColladaLoader_startElement, ColladaLoader_endElement);
	XML_SetCharacterDataHandler(parser, ColladaLoader_bodyElement);

	state.push_back(TAG_NONE);

	if ((XML_Parse(parser, data, len, true)) == XML_STATUS_ERROR) {
		// エラー
		switch (XML_GetErrorCode(parser)) {
		case XML_ERROR_NO_MEMORY:	// メモリ不足
			break;
		case XML_ERROR_SYNTAX:		// シンタックスエラー
			break;
		case XML_ERROR_INVALID_TOKEN:
			break;
        default:
            break;
		}
	}
	XML_ParserFree(parser);
}

void ColladaLoader::startElement(const XML_Char *name, const XML_Char *atts[]) {
	int id = getTagID(name);
	switch (id) {
	case TAG_LIBRARY_MATERIALS:
		break;
	case TAG_LIBRARY_GEOMETRIES:
		break;
	case TAG_GEOMETRY:
		parseGeometry(atts);
		break;
	case TAG_MESH:
		break;
	case TAG_SOURCE:
		parseSource(atts);
		break;
	case TAG_FLOAT_ARRAY:
		parseFloatArray(atts);
		break;
	case TAG_TRIANGLES:
		parseTriangles(atts);
		break;
	}
	state.push_back(id);
}

void ColladaLoader::endElement(const XML_Char *name) {
	state.pop_back();
}

void ColladaLoader::bodyElement(const XML_Char *s, int len) {
	switch (state.back()) {
	case TAG_FLOAT_ARRAY:
		parseFloatArrayBody(s, len);
		break;
	case TAG_P:
		parsePBody(s, len);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// privateな関数
//////////////////////////////////////////////////////////////////////////////////////

int ColladaLoader::getTagID(const char *name) {
	int len = strlen(name);
	int i = 0;
	while (tags[i].id != -1) {
		if (len == tags[i].name.length() &&
			strncmp(name, tags[i].name.c_str(), tags[i].name.length()) == 0) {
			return tags[i].id;
		}
		i++;
	}
	return -1;
}

int ColladaLoader::getAttrID(const char *name) {
	int i = 0;
	while (attrs[i].id != -1) {
		if (strncmp(name, attrs[i].name.c_str(), attrs[i].name.length()) == 0) {
			return attrs[i].id;
		}
		i++;
	}
	return -1;
}

int ColladaLoader::nextToken(const char *data, int len) {
	int cnt = 0;
	do {
		if (data[cnt] == ' ' || data[cnt] == '\n' || data[cnt] == '\t') {
			return cnt + 1;
		}
		cnt++;
	} while (cnt < len);
	return len;
}

int ColladaLoader::skipWhitespace(const char *data, int len) {
	int cnt = 0;
	do {
		if (data[cnt] != ' ' && data[cnt] != '\n' && data[cnt] != '\t') {
			return cnt;
		}
		cnt++;
	} while (cnt < len);
	return len;
}

void ColladaLoader::parseGeometry(const XML_Char *atts[]) {
	Geometry geometry;
	for (int i = 0; atts[i]; i += 2) {
		switch (getAttrID(atts[i])) {
		case ATTR_ID:
			geometry.id.append(atts[i + 1]);
			break;
		case ATTR_NAME:
			geometry.name.append(atts[i + 1]);
			break;
		}
	}
	collada->geometryLibrary.addGeometry(geometry);
}

void ColladaLoader::parseSource(const XML_Char *atts[]) {
	Source source;
	for (int i = 0; atts[i]; i += 2) {
		switch (getAttrID(atts[i])) {
		case ATTR_ID:
			source.id.append(atts[i + 1]);
			break;
		}
	}

	// 一つ上のタグが何かのかによって処理を分ける
	int id = state.at(state.size() - 1);
	switch (id) {
	case TAG_MESH:
		collada->geometryLibrary.getLastGeometry()->mesh.addSource(source);
		break;
	}
}

void ColladaLoader::parseFloatArray(const XML_Char *atts[]) {
	Source *source = collada->geometryLibrary.getLastGeometry()->mesh.getLastSource();
	for (int i = 0; atts[i]; i += 2) {
		switch (getAttrID(atts[i])) {
		case ATTR_COUNT:
			source->floatArray.count = atoi(atts[i + 1]);
			break;
		case ATTR_ID:
			source->id.append(atts[i + 1]);
			break;
		case ATTR_NAME:
			break;
		}
	}
}

void ColladaLoader::parseFloatArrayBody(const char *data, int len) {
	Source *source = collada->geometryLibrary.getLastGeometry()->mesh.getLastSource();
//	int i = 0;
	int cnt = 0;
	do {
		source->floatArray.floats.push_back(atof(&data[cnt]));
		cnt += nextToken(&data[cnt], len - cnt);
	} while (cnt < len);

	if (source->floatArray.floats.size() != source->floatArray.count) {
		// 定義と実際の数値の数が合っていない
	}
}

void ColladaLoader::parseTriangles(const XML_Char *atts[]) {
	Triangles t;
	for (int i = 0; atts[i]; i += 2) {
		switch (getAttrID(atts[i])) {
		case ATTR_COUNT:
			t.p.count = t.count = atoi(atts[i + 1]);
			break;
		case ATTR_MATERIAL:
			t.material.append(atts[i + 1]);
			break;
		}
	}
	collada->geometryLibrary.getLastGeometry()->mesh.addTriangles(t);
}

void ColladaLoader::parsePBody(const char *data, int len) {
//	P *p = &collada->geometryLibrary.getLastGeometry()->mesh.getLastTriangles()->p;
//	int i = 0;
//	int cnt = 0;
//	do {
//		p->indices[i++] = atoi(&data[cnt]);
//		cnt += nextToken(&data[cnt], len - cnt);
//	} while (cnt < len);
//
//	if (i != p->count) {
//		// 定義と実際の数値の数が合っていない
//	}
}
