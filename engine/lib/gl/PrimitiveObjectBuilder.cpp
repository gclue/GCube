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

#include "PrimitiveObjectBuilder.h"
#include "glcommon.h"
#include "Figure.h"

#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <math.h>


Figure* createTriangle(float ax, float ay, float bx, float by, float cx,
		float cy, float z, float max, float sax, float say, float sbx,
		float sby, float scx, float scy) {
	const GLfloat posArray[] = { ax, ay, z, bx, by, z, cx, cy, z };

	const GLfloat texcoordArray[] = { sax / max, say / max, sbx / max, sby
			/ max, scx / max, scy / max };

	const GLfloat normalArray[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f };

	const GLushort elementArray[] = { 0, 1, 2 };

	Figure *fig = new Figure();
	fig->addVertices(posArray, 9);
	fig->addNormal(normalArray, 9);
	fig->addTextureCoords(texcoordArray, 6);
	fig->addVertexIndexes(elementArray, 3);

	return fig;
}

Figure *createPoint(float x, float y) {
	const GLfloat posArray[] = { x, y, 0 };
	const GLushort elementArray[] = { 0 };
	Figure *fig = new Figure();
	fig->addVertices(posArray, 3);
	fig->addVertexIndexes(elementArray, 1);
	return fig;
}

Figure *createPoint2(double x, double y, double max, double width, double height, int kind) {
	const GLfloat posArray[] = {x, y, 0};
	const GLushort elementArray[] = {0};

	double stx = 0 + kind * (width);
	double sty = 0;
	double ttx = 0 + kind * (width) + width;
	double tty = height;

	const GLfloat texcoordArray[] = {
				stx / max, tty / max,
				ttx / max, tty / max,
				ttx / max, sty / max,
				stx / max, sty / max
		};
	Figure *fig = new Figure();
	fig->addVertices(posArray, 3);
	fig->addVertexIndexes(elementArray, 1);
	fig->addTextureCoords(texcoordArray, 8);

	return fig;

}

Figure *createPlate(float x, float y) {
	const GLfloat posArray[] = {
		-x, -y, 0.0f,
		 x, -y, 0.0f,
		 x,  y, 0.0f,
		-x,  y, 0.0f
	};

	const GLfloat texcoordArray[] = {
			0.0f,  1.0f,
			1.0f,  1.0f,
			1.0f,  0.0f,
			0.0f,  0.0f,
	};

	const GLfloat normalArray[] = {
		0.0f, 0.0f, 1.0,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	const GLushort elementArray[] = {
		0, 1, 2,
		0, 2, 3
	};

	Figure *fig = new Figure();
	fig->addVertices(posArray, 12);
	fig->addNormal(normalArray, 12);
	fig->addTextureCoords(texcoordArray, 8);
	fig->addVertexIndexes(elementArray, 6);

	return fig;
}

Figure *createPlateWithTexpos(float x, float y, float v1, float t1, float v2, float t2) {
	const GLfloat posArray[] = {
		-x, -y, 0.0f,
		 x, -y, 0.0f,
		 x,  y, 0.0f,
		-x,  y, 0.0f
	};

	const GLfloat texcoordArray[] = {
			v1,  t2,
			v2,  t2,
			v2,  t1,
			v1,  t1
	};

	const GLfloat normalArray[] = {
		0.0f, 0.0f, 1.0,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	const GLushort elementArray[] = {
		0, 1, 2,
		0, 2, 3
	};

	Figure *fig = new Figure();
	fig->addVertices(posArray, 12);
	fig->addNormal(normalArray, 12);
	fig->addTextureCoords(texcoordArray, 8);
	fig->addVertexIndexes(elementArray, 6);

	return fig;
}


Figure *createPlateWithParam(float sx, float sy, float ex, float ey, float z) {
	const GLfloat posArray[] = { sx, sy, z, ex, sy, z, ex, ey, z, sx, ey, z };

	const GLfloat texcoordArray[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.0f };

	const GLfloat normalArray[] = { 0.0f, 0.0f, 1.0, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, };

	const GLushort elementArray[] = { 0, 1, 2, 0, 2, 3 };

	Figure *fig = new Figure();
	fig->addVertices(posArray, 12);
	fig->addNormal(normalArray, 12);
	fig->addTextureCoords(texcoordArray, 8);
	fig->addVertexIndexes(elementArray, 6);

	return fig;
}
Figure *createPlateWithParam2(float sx, float sy, float ex, float ey, float z,
		int index) {
	const GLfloat posArray[] = { sx, sy, z, ex, sy, z, ex, ey, z, sx, ey, z };

	//		const GLfloat texcoordArray[] = {
	//				(27.0f*(float)index)/256.0f, 1.0f,
	//				(27.0f * (float)(index + 1))/256.0f, 1.0f,
	//				(27.0f * (float)(index + 1))/256.0f, 1.0f-(30.0f/256.0f),
	//				(27.0f*(float)index)/256.0f, 1.0f-(30.0f/256.0f)
	//		};

	const GLfloat texcoordArray[] = {
			(27.0f * (float) index) / 256.0f, (29.0f/ 256.0f),
			(27.0f * (float) (index + 1)) / 256.0f, (29.0f / 256.0f),
			(27.0f * (float) (index + 1)) / 256.0f, 0.0f,
			(27.0f * (float) index) / 256.0f, 0.0f,

	};
	const GLfloat texcoordArray2[] = {
			0.0f, 60.0f / 256.0f,
			27.0f / 256.0f, (60.0f / 256.0f),
			27.0f / 256.0f, (30.0f / 256.0f),
			0.0f, (30.0f / 256.0f),
	};

	const GLfloat normalArray[] = { 0.0f, 0.0f, 1.0, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, };

	const GLushort elementArray[] = { 0, 1, 2, 0, 2, 3 };

	Figure *fig = new Figure();
	fig->addVertices(posArray, 12);
	fig->addNormal(normalArray, 12);
	if (index == 9) {
		fig->addTextureCoords(texcoordArray2, 8);
	} else {
		fig->addTextureCoords(texcoordArray, 8);
	}
	fig->addVertexIndexes(elementArray, 6);

	return fig;
}

Figure* createPlateWithParam3(float sx, float sy, float ex, float ey, float z, float max, float stx, float sty, float ttx, float tty){
	const GLfloat posArray[] = { sx, sy, z, ex, sy, z, ex, ey, z, sx, ey, z };

	const GLfloat texcoordArray[] = {
			stx / max, tty / max,
			ttx / max, tty / max,
			ttx / max, sty / max,
			stx / max, sty / max
	};

	const GLfloat normalArray[] = { 0.0f, 0.0f, 1.0, 0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 1.0f, };

		const GLushort elementArray[] = { 0, 1, 2, 0, 2, 3 };

		Figure *fig = new Figure();
		fig->addVertices(posArray, 12);
		fig->addNormal(normalArray, 12);
		fig->addTextureCoords(texcoordArray, 8);
		fig->addVertexIndexes(elementArray, 6);

		return fig;
}
Figure* createTexturalPlate(float sx, float sy, float ex, float ey, float z, float max, float stx, float sty, float ttx, float tty){
	const GLfloat posArray[] = { sx, sy, z, ex, sy, z, ex, ey, z, sx, ey, z };

	const GLfloat texcoordArray[] = {
			stx / max, tty / max,
			ttx / max, tty / max,
			ttx / max, sty / max,
			stx / max, sty / max
	};

	const GLfloat normalArray[] = { 0.0f, 0.0f, 1.0, 0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 1.0f, };

		const GLushort elementArray[] = { 0, 1, 2, 0, 2, 3 };

		Figure *fig = new Figure();
		fig->addVertices(posArray, 12);
		fig->addNormal(normalArray, 12);
		fig->addTextureCoords(texcoordArray, 8);
		fig->addVertexIndexes(elementArray, 6);

		return fig;
}

Figure* createBox(float x, float y, float z) {
	const GLfloat posArray[] = {
		 x, y, z,  -x, y, z,  -x,-y, z,   x,-y, z,  // v0-v1-v2-v3 front
		 x, y, z,   x,-y, z,   x,-y,-z,   x, y,-z,  // v0-v3-v4-v5 right
		 x, y, z,   x, y,-z,  -x, y,-z,  -x, y, z,  // v0-v5-v6-v1 top
		-x, y, z,  -x, y,-z,  -x,-y,-z,  -x,-y, z,  // v1-v6-v7-v2 left
		-x,-y,-z,   x,-y,-z,   x,-y, z,  -x,-y, z,  // v7-v4-v3-v2 bottom
		 x,-y,-z,  -x,-y,-z,  -x, y,-z,   x, y,-z   // v4-v7-v6-v5 back
	};

	const GLfloat texcoordArray[] = {
		1.0, 1.0,   0.0, 1.0,   0.0, 0.0,   1.0, 0.0,  // v0-v1-v2-v3 front
		0.0, 1.0,   0.0, 0.0,   1.0, 0.0,   1.0, 1.0,  // v0-v3-v4-v5 right
		1.0, 0.0,   1.0, 1.0,   0.0, 1.0,   0.0, 0.0,  // v0-v5-v6-v1 top
		1.0, 1.0,   0.0, 1.0,   0.0, 0.0,   1.0, 0.0,  // v1-v6-v7-v2 left
		0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0,  // v7-v4-v3-v2 bottom
		0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0   // v4-v7-v6-v5 back
	};

	const GLfloat normalArray[] = {
		 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0-v1-v2-v3 front
		 1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0-v3-v4-v5 right
		 0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0-v5-v6-v1 top
		-1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1-v6-v7-v2 left
		 0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7-v4-v3-v2 bottom
		 0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1    // v4-v7-v6-v5 back
	};

	const GLushort elementArray[] = {
		 0, 1, 2,   0, 2, 3,   // front
		 4, 5, 6,   4, 6, 7,   // right
		 8, 9,10,   8,10,11,   // top
		12,13,14,  12,14,15,   // left
		16,17,18,  16,18,19,   // bottom
		20,21,22,  20,22,23    // back
	};

	Figure *fig = new Figure();
	fig->addVertices(posArray, 72);
	fig->addNormal(normalArray, 72);
	fig->addTextureCoords(texcoordArray, 48);
	fig->addVertexIndexes(elementArray, 36);

	return fig;
}

Figure *createSphere(float radius, int slices, int stacks) {
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> textureCoords;

	Figure *fig = new Figure();
	GLfloat pos[2];
	GLfloat tmpV[3 * 3] = { 0 };

	for (int i = 0 ; i <= stacks ; ++i) {
		GLfloat v = i / (float) stacks;
		GLfloat y = cosf(M_PI * v) * radius;
		GLfloat r = sinf(M_PI * v) * radius;
		for(int j = 0 ; j <= slices ; ++j) {
			GLfloat u = j / (float) slices;
			// 頂点
			tmpV[0] = cosf(2 * M_PI * u) * r;
			tmpV[1] = y;
			tmpV[2] = sinf(2 * M_PI * u) * r;
			vertices.push_back(tmpV[0]);
			vertices.push_back(tmpV[1]);
			vertices.push_back(tmpV[2]);
			// テクスチャ
			pos[0] = u;
			pos[1] = v;
			textureCoords.push_back(pos[0]);
			textureCoords.push_back(pos[1]);
		}
	}

	int vsize = 3;
	int psize = 2;
	GLfloat tmpVt[3 * 2];
	GLushort cnt = 0;
	for(int j = 0 ; j < stacks ; ++j) {
		int base = j * (slices + 1);
		for(int i = 0 ; i < slices ; ++i) {
			// face1
			GLushort idx = base + i;
			tmpV[0] = vertices.at(idx * vsize + 0);
			tmpV[1] = vertices.at(idx * vsize + 1);
			tmpV[2] = vertices.at(idx * vsize + 2);
			tmpVt[0] = textureCoords.at(idx * psize + 0);
			tmpVt[1] = textureCoords.at(idx * psize + 1);

			idx = base + i + 1;
			tmpV[3] = vertices.at(idx * vsize + 0);
			tmpV[4] = vertices.at(idx * vsize + 1);
			tmpV[5] = vertices.at(idx * vsize + 2);
			tmpVt[2] = textureCoords.at(idx * psize + 0);
			tmpVt[3] = textureCoords.at(idx * psize + 1);

			idx = base + i + (slices + 1);
			tmpV[6] = vertices.at(idx * vsize + 0);
			tmpV[7] = vertices.at(idx * vsize + 1);
			tmpV[8] = vertices.at(idx * vsize + 2);
			tmpVt[4] = textureCoords.at(idx * psize + 0);
			tmpVt[5] = textureCoords.at(idx * psize + 1);

			fig->addVertices(tmpV, 3 * 3);
			fig->addNormal(tmpV, 3 * 3);
			fig->addTextureCoords(tmpVt, 2 * 3);

			// face2
			idx = base + i + (slices + 1);
			tmpV[0] = vertices.at(idx * vsize + 0);
			tmpV[1] = vertices.at(idx * vsize + 1);
			tmpV[2] = vertices.at(idx * vsize + 2);
			tmpVt[0] = textureCoords.at(idx * psize + 0);
			tmpVt[1] = textureCoords.at(idx * psize + 1);

			idx = base + i + 1;
			tmpV[3] = vertices.at(idx * vsize + 0);
			tmpV[4] = vertices.at(idx * vsize + 1);
			tmpV[5] = vertices.at(idx * vsize + 2);
			tmpVt[2] = textureCoords.at(idx * psize + 0);
			tmpVt[3] = textureCoords.at(idx * psize + 1);

			idx = base + i + 1 + (slices + 1);
			tmpV[6] = vertices.at(idx * vsize + 0);
			tmpV[7] = vertices.at(idx * vsize + 1);
			tmpV[8] = vertices.at(idx * vsize + 2);
			tmpVt[4] = textureCoords.at(idx * psize + 0);
			tmpVt[5] = textureCoords.at(idx * psize + 1);

			fig->addVertices(tmpV, 3 * 3);
			fig->addNormal(tmpV, 3 * 3);
			fig->addTextureCoords(tmpVt, 2 * 3);

			// index
			for (int k = 0; k < 6; k++) {
				fig->addVertexIndexes(&cnt, 1);
				cnt++;
			}
		}
	}

	return fig;
}
