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
 * UIController
 *
 *  Created on: 2011/09/16
 *      Author: GClue, Inc.
 */
#ifndef UICONTROLLER_H_
#define UICONTROLLER_H_

#include <string>
#include <vector>

#include "View.h"

/**
 * UIタイプを定義します.
 */
enum UIType {
	UIType_Texture = 0,
	UIType_SharedTexture,
	UIType_ImageView,		//!< ImageViewを表すタイプ
	UIType_Button,			//!< Buttonを表すタイプ
	UIType_ViewGroup,		//!< ViewGroupを表すタイプ
	UIType_ScrollView,		//!< ScrollViewを表すタイプ
	UIType_GridView,		//!< GridViewを表すタイプ
	UIType_Scene,			//!< Sceneを表すタイプ
	UIType_Layer2D,			//!< Layer2Dを表すタイプ
	UIType_Layer3D,			//!< Layer3Dを表すタイプ
};

/**
 * UIコンテナ.
 */
class UIContainer {
public:
	int type;			//!< UIのタイプ
	void* ui;			//!< UIのポインタ
	std::string name;	//!< 名前

	UIContainer();
	~UIContainer();
};

/**
 *
 */
class UIController {
private:
	std::vector<UIContainer*> containers;
public:
	UIController();
	virtual ~UIController();

	void add(UIContainer *container);
	void remove(UIContainer *container);
};

#endif /* UICONTROLLER_H_ */
