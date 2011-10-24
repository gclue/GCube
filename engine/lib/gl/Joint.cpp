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

#include "Joint.h"
#include "BoneShader.h"

// コンストラクタ
Joint::Joint() {
	transForm = new Matrix3D();
	baseMatrix = new Matrix3D();
	invBindMatrix = new Matrix3D();
	workingMtx = new Matrix3D();
	parent = NULL;
	hasMesh = true;
}

// デストラクタ
Joint::~Joint() {
	// 子供を削除
	std::vector<Joint*>::iterator it = children.begin();
	while (it != children.end()) {
		delete *it;
		it++;
	}
	// 行列削除
	delete transForm;
	delete baseMatrix;
	delete invBindMatrix;
	delete workingMtx;
}

// 子追加
void Joint::addChild(Joint *child) {
	child->parent = this;
	children.push_back(child);
}

// 子供配列を返す
std::vector<Joint*> Joint::getChildren() const {
    return children;
}

// 親を返す
Joint *Joint::getParent() const {
    return parent;
}

// 変換行列を適用
int Joint::applyMatrix(Matrix3D* parentMtx, std::vector<Matrix3D*> *visitor) {

	// 要素数
	int count = 0;

	// 自身の行列を適用
	workingMtx->loadIdentity();
	if (parentMtx) workingMtx->multiply(parentMtx);
	workingMtx->multiply(transForm);
	workingMtx->multiply(baseMatrix);
	// 必要ならば行列を返す
	if(hasMesh) {
		visitor->push_back(workingMtx);
		count = 1;
	}

	// 各子供にも適用
	std::vector<Joint*>::iterator it = children.begin();
	while (it != children.end()) {
		Joint *j = (Joint*) *it;
		count += j->applyMatrix(workingMtx, visitor);
		it++;
	}

	// バインドポーズ逆行列を適用してワールド座標に変換
	if(hasMesh) workingMtx->multiply(invBindMatrix);

	return count;
}

// シェーダーにスキニング用行列を設定
void Joint::setSkinningMatrix(BoneShader *shader) {
	std::vector<Matrix3D*> visitor;
	int count = this->applyMatrix(NULL, &visitor);
	shader->setSkinningMatrix(&visitor[0], count);
}
