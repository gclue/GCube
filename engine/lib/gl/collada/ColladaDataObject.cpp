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

#include "ColladaDataObject.h"
#include "Figure.h"
#include "Joint.h"

Joint *ColladaDataObject::makeJoint(ColladaJointData &data, BindMtxMap &bindMtxMap) {

	// Joint作成
	Joint *jt = new Joint();
	jt->baseMatrix->translate(data.location.x, data.location.y, data.location.z, true);
	jt->baseMatrix->rotate(data.rotation.z, RotateDirZ, true);
	jt->baseMatrix->rotate(-data.rotation.y, RotateDirY, true);
	jt->baseMatrix->rotate(data.rotation.x, RotateDirX, true);
	jt->baseMatrix->scale(data.scale.x, data.scale.y, data.scale.z);
	jt->sid = data.sid;
	LOGD("%s", data.sid.c_str());
	//
	BindMtxMap::iterator ret = bindMtxMap.find(data.sid);
	if(ret != bindMtxMap.end()) {
		std::vector<float> data = ret->second;
		jt->invBindMatrix->setElements(&data[0]);
	} else {
		jt->hasMesh = false;
	}

	// 子供作成
	std::vector<ColladaJointData>::iterator it = data.childrenArray.begin();
	while (it != data.childrenArray.end()) {
		jt->addChild(this->makeJoint((ColladaJointData&)*it, bindMtxMap));
		it++;
	}

	return jt;
}

Figure *ColladaDataObject::makeFigure() {

	// バインド逆行列を編成（Colladaは行列が逆なので入れ替える必要アリ）
	int jointCount = bindPosesArray.size() / 16;
	BindMtxMap bindMtxMap;
	for (int k = 0; k < jointCount; ++k) {
		std::vector<float> mtx = std::vector<float>(16);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				mtx[i*4+j] = bindPosesArray[j*4+i+16*k];
//				mtx[i*4+j] = bindPosesArray[i*4+j+16*k];
			}
		}
		//printf("%s,", jointsIdArray[k].c_str());
		if (jointsIdArray.size()>k) {
			bindMtxMap[jointsIdArray[k]] = mtx;
		}
	}

	// Joint作成
	Joint *jt = this->makeJoint(joint, bindMtxMap);

	// JointとWeightを編成
	int maxJoint = weightCountsArray.size();
	GLushort joint1Array[maxJoint];
	GLushort joint2Array[maxJoint];
	GLfloat weight1Array[maxJoint];
	GLfloat weight2Array[maxJoint];
	int jidx = 0;
	for (GLushort i = 0; i < maxJoint; ++i) {
		// Joint
		int jc = weightCountsArray[i];
//		if (jc==0) {
//			joint1Array[i] = 0;
//			joint2Array[i] = 0;
//			weight1Array[i] = 1;
//			weight2Array[i] = 0;
//			continue;
//		}
		joint1Array[i] = weightIndexesArray[jidx*2];
		// weight
		weight1Array[i] = weightsArray[weightIndexesArray[jidx*2+1]];
		if (jc>1) {
			joint2Array[i] = weightIndexesArray[jidx*2+2];
			weight2Array[i] = weightsArray[weightIndexesArray[jidx*2+3]];
		} else {
			joint2Array[i] = 0;
			weight2Array[i] = 0;
		}
//		LOGD("[%d/%d] 1: %d, %f", i, maxJoint, joint1Array[i], weight1Array[i]);
//		LOGD("[%d/%d] 2: %d, %f", i, maxJoint, joint2Array[i], weight2Array[i]);
		jidx += jc;
	}


	int elements = 1;
	bool hasNormal = false;
	bool hasUV = false;
	if (normalsArray.size()>0) {
		elements++;
		hasNormal = true;
	}
	if (uvArray.size()>0) {
		elements++;
		hasUV = true;
	}
	elements = 5;

	// 再構成してFigureにセット
	Figure *fig = new Figure();
	for (GLushort i = 0; i < meshIndexesArray.size()/elements; ++i) {
		int idx = meshIndexesArray[i*elements];
		LOGD("***:%d, %d, %d", i, idx, elements);
		LOGD("****:%d, %d, %d, %f", i, idx, joint1Array[idx], weight1Array[idx]);
		fig->addVertices(&positionsArray[idx*3], 3);
		int offset = 1;
		if (hasNormal) {
			int nidx = meshIndexesArray[i*elements+offset];
			fig->addNormal(&normalsArray[nidx*3], 3);
			offset++;
		}
		if (hasUV) {
			int uidx = meshIndexesArray[i*elements+offset];
			fig->addTextureCoords(&uvArray[uidx*2], 2);
		}
//		LOGD("%d", idx);
		fig->addJoints(&joint1Array[idx], &weight1Array[idx], &joint2Array[idx], &weight2Array[idx], 1);
//		fig->addWeights(&weight1Array[idx], &weight2Array[idx], 1);
		//
//		GLushort vi[] = {i*3, i*3+1, i*3+2};
//		fig->addVertexIndexes(vi, 3);
		fig->addVertexIndexes(&i, 1);
	}
	fig->joint = jt;

	// もはやいらないデータを削除
	positionsArray.clear();
	normalsArray.clear();
	meshIndexesArray.clear();
	bindPosesArray.clear();
	weightsArray.clear();
	weightCountsArray.clear();
	weightIndexesArray.clear();

	return fig;
}
