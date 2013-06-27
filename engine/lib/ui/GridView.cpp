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
 * GridView.cpp
 *
 *  Created on: 2011/08/08
 *      Author: GClue, Inc.
 */
#include "GridView.h"

GridView::GridView(GCContext *context, int row, int column) : ViewGroup(context) {
	this->row = row;
	this->column = column;
	aspect = 1.0;
}

GridView::~GridView() {
}

void GridView::draw(double dt, ViewContext *context) {
	float w = size.x / row;
	float h = size.y / column;
	float px = - w * row + w;
	float py = size.y - h;
	int cnt = 0;
	std::vector<View *>::iterator it = views.begin();
	while (!views.empty() && it != views.end()) {
		View *view = *it;

		int r = cnt % row;
		int c = cnt / row;

		float x = px + r * w * 2;
		float y = py - c * h * 2;

		view->setPosition(x, y);
		view->render(dt, context);
		it++;
		cnt++;
	}
}
