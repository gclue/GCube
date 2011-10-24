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
 * UIController.cpp
 *
 *  Created on: 2011/09/16
 *      Author: GClue, Inc.
 */
#include "UIController.h"
#include "ApplicationController.h"
#include "ImageView.h"
#include "Texture.h"
#include "Scene.h"
#include "Log.h"
#include "Figure.h"
#include "PrimitiveObjectBuilder.h"
#include "APIGlue.h"
#include "Layer2D.h"
#include "ViewGroup.h"

UIContainer::UIContainer() {
}

UIContainer::~UIContainer() {
}

///////////////////////////////////////////////////////////////////////////
/// UIControllerの実装
///////////////////////////////////////////////////////////////////////////

UIController::UIController() {
}

UIController::~UIController() {
	int size = containers.size();
	for (int i = 0; i < size; i++) {
		delete containers.at(i);
	}
	containers.clear();
}

void UIController::add(UIContainer *container) {
	containers.push_back(container);
}

void UIController::remove(UIContainer *container) {
	std::vector<UIContainer *>::iterator it = containers.begin();
	while (!containers.empty() && it != containers.end()) {
		UIContainer *a = *it;
		if (a == container) {
			it = containers.erase(it);
			delete a;
		} else {
			it++;
		}
	}
}
