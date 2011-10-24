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
 * Mesh.cpp
 *
 *  Created on: 2011/08/27
 *      Author: GClue, Inc.
 */
#include "Mesh.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
}

void Mesh::addSource(Source source) {
	sources.push_back(source);
}

int Mesh::getSourceCount() {
	return sources.size();
}

Source* Mesh::getSource(int index) {
	return &sources.at(index);
}

Source* Mesh::getLastSource() {
	return &sources.at(sources.size() - 1);
}

void Mesh::addTriangles(Triangles triagnle) {
	triangles.push_back(triagnle);
}

int Mesh::getTrianglesCount() {
	return triangles.size();
}

Triangles* Mesh::getTriangles(int index) {
	return &triangles.at(index);
}

Triangles* Mesh::getLastTriangles() {
	return &triangles.at(triangles.size() - 1);
}
