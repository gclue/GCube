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
 * ImageAnimationView.cpp
 *
 *  Created on: 2011/10/08
 *      Author: GClue, Inc.
 */
#include "ImageAnimationView.h"
#include "Log.h"
#include "picojson.h"
#include "AssetManager.h"
#include "ImageView.h"
#include "ApplicationController.h"
#include "TextureManager.h"

ImageAnimationView::ImageAnimationView() : ViewGroup() {
	initImageAnimationView();
}

ImageAnimationView::ImageAnimationView(const char *filename) : ViewGroup() {
	initImageAnimationView();
	loadFile(filename);
}

ImageAnimationView::ImageAnimationView(GCContext *context) : ViewGroup(context) {
	initImageAnimationView();
}

ImageAnimationView::~ImageAnimationView() {
	std::map<int, AnimationFrame*>::iterator it = frames.begin();
	while (it != frames.end()) {
		AnimationFrame *frame = (*it).second;
		delete frame;
		it++;
	}
}

void ImageAnimationView::initImageAnimationView() {
	frame = 0.2;
	index = 0;
	time = 0;
	frameId = 0;
	listener = NULL;
}

/**
 * Viewを切り替える時間を指定します.
 * @param frame フレーム
 */
void ImageAnimationView::setFrame(float frame) {
	this->frame = frame;
}

void ImageAnimationView::addAnimationFrame(int id, int index, float frame) {
	if (!frames[id]) {
		frames[id] = new AnimationFrame();
	}
	frames[id]->indexs.push_back(index);
	frames[id]->frames.push_back(frame);
}
/**
 * フレームアニメーションを追加します.
 * @param af アニメーション
 */
void ImageAnimationView::addAnimationFrame(int id, AnimationFrame *af) {
	frames[id] = af;
}

void ImageAnimationView::setAnimationFrameIndex(int id) {
	if (frames[id]) {
		frameId = id;
		index = 0;
		time = 0;
	}
}

void ImageAnimationView::setImageAnimationListener(ImageAnimationListener *listener) {
	this->listener = listener;
}

void ImageAnimationView::loadFile(const char *filename) {
	AssetManager mgr = AssetManager::getInstance();
	std::vector<char> *fdata = mgr.open(filename);
	if (!fdata) return ;
	fdata->push_back('\0');
	const char *json = (const char *) &(*fdata)[0];
	loadJson(json);
	delete fdata;
}

void ImageAnimationView::loadJson(const char *json) {
	ApplicationController *ctl = ApplicationController::getInstance();
	std::string err;
	picojson::value v;
	picojson::parse(v, json, json + strlen(json), &err);
	if (err.empty()) {
		picojson::object root = v.get<picojson::object>();
		
		picojson::value pngvalue = root["png"];
		picojson::value txtvalue = root["txt"];
		if (pngvalue.is<std::string>() && txtvalue.is<std::string>()) {
			std::string png = pngvalue.get<std::string>();
			std::string txt = txtvalue.get<std::string>();
			ctl->texMgr->loadSharedTexture(png.c_str(), txt.c_str());
		}
		
		picojson::value plistvalue = root["plist"];
		if (plistvalue.is<std::string>()) {
			std::string plist = plistvalue.get<std::string>();
			ctl->texMgr->loadSharedTextureFromPlist(plist.c_str());
		}
		
		picojson::array figures = root["figure"].get<picojson::array>();
		picojson::array::iterator it;
		for (it = figures.begin(); it != figures.end(); it++) {
			std::string abc = it->get<std::string>();
			ImageView *view = new ImageView(abc.c_str());
			addView(view);
			view->release();
		}
		
		picojson::array animations = root["animation"].get<picojson::array>();
		for (it = animations.begin(); it != animations.end(); it++) {
			picojson::object animation = it->get<picojson::object>();
			
			int id = animation["id"].get<double>();
			picojson::array frames = animation["frame"].get<picojson::array>();
			picojson::array::iterator it2;
			for (it2 = frames.begin(); it2 != frames.end(); it2++) {
				picojson::object frame = it2->get<picojson::object>();
				int index = frame["index"].get<double>();
				float time = frame["time"].get<double>();
				addAnimationFrame(id, index, time);
			}
		}
	}
}


//////////////////////////////////////////////////////////////
// Viewからの継承
//////////////////////////////////////////////////////////////

/**
 * 指定されたアニメーションを反映して描画を行います.
 * @param[in] dt 前回描画からの差分時間
 * @param[in] animation 反映するアニメーション
 */
void ImageAnimationView::draw(double dt, ViewContext *context) {
	time += dt;

	int idx;
	float frm;

	if (frames.size() > 0 && frameId > 0) {
		AnimationFrame *af = frames[frameId];
		frm = af->frames.at(index);
		idx = af->indexs.at(index);

		if (time > frm) {
			index++;
			index %= af->frames.size();
			time = 0;
			if (listener && index == 0) {
				listener->onFrameEnd(frameId);
			}

			frm = af->frames.at(index);
			idx = af->indexs.at(index);
		}
	} else {
		if (time > frame) {
			// 指定の時間よりも長い場合には切り替える
			index++;
			index %= views.size();
			// 時間を初期化
			time = 0;
			if (listener && index == 0) {
				listener->onFrameEnd(0);
			}
		}
		frm = frame;
		idx = index;
	}

	views.at(idx)->render(dt, context);
}
