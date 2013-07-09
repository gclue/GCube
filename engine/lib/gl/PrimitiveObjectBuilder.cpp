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

Figure *createPlateWithRotateTexpos(float x, float y, float v1, float t1, float v2, float t2) {
	const GLfloat posArray[] = {
		-x, -y, 0.0f,
        x, -y, 0.0f,
        x,  y, 0.0f,
		-x,  y, 0.0f
	};
    
	const GLfloat texcoordArray[] = {
        v1,  t1,
        v1,  t2,
        v2,  t2,
        v2,  t1
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

static float angleToRadian(float angle) {
	return 2 * M_PI * angle / 360.0;
}

static Pointf* crossPoint(Pointf &a, Pointf &b, Pointf &c, Pointf &d) {
	
	float dBunbo = (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);
	if (dBunbo == 0) {
		return NULL;
	}
	Pointf vectorAC;
	vectorAC.x = c.x - a.x;
	vectorAC.y = c.y - a.y;
	
	float dR = ((d.y - c.y) * vectorAC.x - (d.x - c.x) * vectorAC.y) / dBunbo;
	float x = a.x + dR * (b.x - a.x);
	float y = a.y + dR * (b.y - a.y);
	
	Pointf *p = new Pointf();
	p->x = x;
	p->y = y;
	return p;
}

Figure *createStar(float size) {
	Pointf a[5];
	float posArray[32];
	int j = 0;
	int k = 0;
	for (int i = 0; i < 5; i++) {
		float t = angleToRadian(360 * i / 5);
		float x = size * sinf(t);
		float y = size * cosf(t);
		float z = 0;
		posArray[k++] = x;
		posArray[k++] = y;
		posArray[k++] = z;
		a[j].x = x;
		a[j].y = y;
		j++;
	}
	Pointf *p1 = crossPoint(a[0], a[3], a[1], a[4]);
	posArray[k++] = p1->x;
	posArray[k++] = p1->y;
	posArray[k++] = 0;
	Pointf *p2 = crossPoint(a[0], a[3], a[2], a[4]);
	posArray[k++] = p2->x;
	posArray[k++] = p2->y;
	posArray[k++] = 0;
	Pointf *p3 = crossPoint(a[1], a[3], a[2], a[4]);
	posArray[k++] = p3->x;
	posArray[k++] = p3->y;
	posArray[k++] = 0;
	Pointf *p4 = crossPoint(a[0], a[2], a[1], a[3]);
	posArray[k++] = p4->x;
	posArray[k++] = p4->y;
	posArray[k++] = 0;
	Pointf *p5 = crossPoint(a[0], a[2], a[1], a[4]);
	posArray[k++] = p5->x;
	posArray[k++] = p5->y;
	posArray[k++] = 0;
	
	delete p1;
	delete p2;
	delete p3;
	delete p4;
	delete p5;

	float texcoordArray[] = {
		0.5, 0.0,
		0.9755282581475768, 0.3454915028125263,
		0.7938926261462367, 0.9045084971874737,
		0.2061073738537635, 0.9045084971874737,
		0.02447174185242318, 0.3454915028125264,
		0.3877430058551037, 0.3454915028125263,
		0.31836436799865975, 0.5590169943749476,
		0.5, 0.6909830056250525,
		0.6816356320013403, 0.5590169943749473,
		0.6122569941448964, 0.3454915028125263
	};
	float normalArray[] = {
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0
	};
	unsigned short elementArray[] = {
		0, 5, 9,
		1, 9, 8,
		2, 8, 7,
		3, 7, 6,
		4, 6, 5,
		5, 6, 9,
		6, 8, 9,
		6, 7, 8
	};
	
	Figure *fig = new Figure();
	fig->addVertices(posArray, k);
	fig->addNormal(normalArray, sizeof(normalArray));
	fig->addTextureCoords(texcoordArray, sizeof(texcoordArray));
	fig->addVertexIndexes(elementArray, sizeof(elementArray));
	return fig;
}

Figure *createCircle(int count, float size) {
	std::vector<float> posArray;
	for (int i = 0; i < count; i++) {
		float t = angleToRadian(360 * i / count);
		float x = size * sinf(t);
		float y = size * cosf(t);
		float z = 0;
		posArray.push_back(x);
		posArray.push_back(y);
		posArray.push_back(z);
	}
	// 原点を頂点に追加
	posArray.push_back(0);
	posArray.push_back(0);
	posArray.push_back(0);
	
	// テクスチャ
	std::vector<float> texcoordArray;
	for (int i = 0; i < count; i++) {
		float t = angleToRadian(360 * i / count);
		float x = 0.5 + 0.5 * sinf(t);
		float y = 1.0 - (0.5 + 0.5 * cosf(t));
		texcoordArray.push_back(x);
		texcoordArray.push_back(y);
	}
	texcoordArray.push_back(0.5);
	texcoordArray.push_back(0.5);
	
	// 法線ベクトル
	std::vector<float> normalArray;
	for (int i = 0; i < count; i++) {
		normalArray.push_back(0);
		normalArray.push_back(0);
		normalArray.push_back(1);
	}
	normalArray.push_back(0);
	normalArray.push_back(0);
	normalArray.push_back(1);
	
	// 頂点の配列
	std::vector<unsigned short> elementArray;
	for (int i = 0; i < count; i++) {
		elementArray.push_back(i);
		elementArray.push_back(count);
		elementArray.push_back((i + 1) % count);
	}

	Figure *fig = new Figure();
	fig->addVertices((float *)&(posArray)[0], posArray.size());
	fig->addNormal((float *)&(normalArray)[0], normalArray.size());
	fig->addTextureCoords((float *)&(texcoordArray)[0], texcoordArray.size());
	fig->addVertexIndexes((unsigned short *)&(elementArray)[0], elementArray.size());
	return fig;
}
