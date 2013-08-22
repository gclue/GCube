//
//  JointKeyFrame.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/01/08.
//
//

#include "JointAnimation.h"

JointKeyFrame::JointKeyFrame() {
}

JointKeyFrame::~JointKeyFrame() {
	std::vector<Matrix3D*>::iterator it = matrixs.begin();
	while (it != matrixs.end()) {
		delete *it;
		it++;
	}
}

void JointKeyFrame::addTime(const float *v, int len) {
	std::vector<float> tmp(v, v + len);
	times.insert(times.end(), tmp.begin(), tmp.end());
}

void JointKeyFrame::addMatrix(const float *v, int len) {
	for (int i = 0; i < len; i++) {
		Matrix3D *m = new Matrix3D();
		m->setElements((const float *)&v[sizeof(float) * 16 * i]);
		matrixs.push_back(m);
	}
}

Matrix3D* JointKeyFrame::getMatrix(float time) {
	for (int i = 0; i < matrixs.size(); i++) {
		if (time < times.at(i)) {
			return matrixs.at(i);
		}
	}
	return matrixs.at(matrixs.size() - 1);
}

float JointKeyFrame::getAnimationTime() {
	return times.at(times.size() - 1);
}

int JointKeyFrame::getIndex(float time) {
	for (int i = 0; i < matrixs.size(); i++) {
		if (time < times.at(i)) {
			return i;
		}
	}
	return matrixs.size() - 1;
}

///////////////////////////////////////////////////////////////////////////


JointAnimation::JointAnimation() {
	animationTime = 0;
	repeat = true;
}

JointAnimation::~JointAnimation() {
	std::vector<JointKeyFrame*>::iterator it = animations.begin();
	while (it != animations.end()) {
		delete *it;
		it++;
	}
}

float JointAnimation::getAnimationTime() {
	return animationTime;
}

float JointAnimation::getTotalAnimationTime() {
	float t = 0;
	for (int i = 0; i < animations.size(); i++) {
		if (t < animations.at(i)->getAnimationTime()) {
			t = animations.at(i)->getAnimationTime();
		}
	}
	return t;
}

int JointAnimation::getFrameIndex(std::string& sid) {
	for (int i = 0; i < animations.size(); i++) {
		if (animations.at(i)->sid.compare(sid) == 0) {
			return animations.at(i)->getIndex(animationTime);
		}
	}
	return -1;
}

bool JointAnimation::isAnimationEnd(){
	return getTotalAnimationTime() < animationTime;
}

