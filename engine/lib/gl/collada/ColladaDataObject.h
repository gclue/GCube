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

#ifndef COLLADADATAOBJECT_H_
#define COLLADADATAOBJECT_H_

#include <vector>
#include <string>
#include <map>
#include "defines.h"

typedef std::map<std::string, std::vector<float> > BindMtxMap;

class Figure;
class Joint;

struct ColladaJointData {
	std::string sid;
	Point3f location;
	Point3f rotation;
	Point3f scale;
	Point3f orient;
	std::vector<ColladaJointData> childrenArray;
};
typedef struct ColladaJointData ColladaJointData;

class ColladaDataObject {
public:
	// mesh
	std::vector<float> positionsArray;
	std::vector<float> normalsArray;
	std::vector<float> uvArray;
	std::vector<int> meshIndexesArray;
	// weight
	std::vector<float> weightsArray;
	std::vector<int> weightCountsArray;
	std::vector<int> weightIndexesArray;
	// bind matrix
	std::vector<float> bindPosesArray;
	// joints
	std::vector<std::string> jointsIdArray;
	ColladaJointData joint;

	Figure *makeFigure();
	Joint *makeJoint(ColladaJointData &data, BindMtxMap &bindMtxMap);
};

#endif /* COLLADADATAOBJECT_H_ */
