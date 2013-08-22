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

#ifndef JOINT_H_
#define JOINT_H_

#include "glcommon.h"
#include <vector>
#include <string>

/**
 * ジョイントクラス.
 */
class Joint {
private:
	std::vector<Joint*> children;	//!< 子供.
	Joint *parent;					//!< 親.
	Matrix3D *workingMtx;			//!< 作業用行列.

public:
	std::string sid;				//!< SID
	bool hasMesh;					//!< メッシュと関連しているか.
	Matrix3D *transForm;			//!< 変形行列.
	Matrix3D *baseMatrix;			//!< 初期形状行列.
	Matrix3D *invBindMatrix;		//!< バインドポーズ逆行列.

	/**
	 * コンストラクタ.
	 */
    Joint();

	/**
	 * デストラクタ.
	 */
    virtual ~Joint();

	/**
	 * 子ジョイントを追加します.
	 * @param[in] child 追加するジョイント
	 */
    void addChild(Joint *child);

	/**
	 * 子ジョイント配列を返します.
	 * @return ジョイント配列
	 */
    std::vector<Joint*> getChildren() const;

	/**
	 * 親ジョイントを返します.
	 * @return 親ジョイント
	 */
    Joint *getParent() const;
	
	/**
	 * 指定されたsidのジョイントを返します.
	 * 指定されたsidのジョイントが見つからない場合はNULLを返します.
	 * @param[in] sid ジョイントのID
	 * @return ジョイント
	 */
	Joint *findJointBySID(std::string sid);

	/**
	 * 変換行列を適用します.
	 * @param[in] parentMtx 親の変換行列
	 * @param[in] visitor 行列格納用の配列
	 * @return 要素数
	 */
    int applyMatrix(Matrix3D* parentMtx, std::vector<Matrix3D*> *visitor);
	
	/**
	 * Jointのデータを出力します.
	 * @param tab タブ
	 */
	void printf(int tab);
};

#endif /* JOINT_H_ */
