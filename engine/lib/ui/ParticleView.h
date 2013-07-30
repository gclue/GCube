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
 * ParticleView.h
 *
 *  Created on: 2011/08/16
 *      Author: GClue, Inc.
 */
#ifndef PARTICLEVIEW_H_
#define PARTICLEVIEW_H_

#include "View.h"
#include "ParticleShader.h"

#include <vector>

// Particle type
enum kParticleTypes {
	kParticleTypeGravity,
	kParticleTypeRadial
};

// Structure that holds the location and size for each point sprite
typedef struct {
	GLfloat x;
	GLfloat y;
    GLfloat s;
    GLfloat t;
	Colorf color;
} PointSprite;

typedef struct {
    Pointf vertex;
    Pointf texture;
    Colorf color;
} TexturedColoredVertex;

typedef struct {
    TexturedColoredVertex bl;
    TexturedColoredVertex br;
    TexturedColoredVertex tl;
    TexturedColoredVertex tr;
} ParticleQuad;

// Structure used to hold particle specific information
typedef struct {
	Pointf position;
	Pointf direction;
    Pointf startPos;
	Colorf color;
	Colorf deltaColor;
    GLfloat rotation;
    GLfloat rotationDelta;
    GLfloat radialAcceleration;
    GLfloat tangentialAcceleration;
	GLfloat radius;
	GLfloat radiusDelta;
	GLfloat angle;
	GLfloat degreesPerSecond;
	GLfloat particleSize;
	GLfloat particleSizeDelta;
	GLfloat timeToLive;
} Particle;

#define MAXIMUM_UPDATE_RATE 90.0f	// The maximum number of updates that occur per frame



class ParticleView : public View {
private:
	Texture *texture;
	ParticleShader *shader;
	
	/////////////////// Particle iVars
    int emitterType;
	Pointf sourcePosition, sourcePositionVariance;
	GLfloat angle, angleVariance;
	GLfloat speed, speedVariance;
    GLfloat radialAcceleration, tangentialAcceleration;
    GLfloat radialAccelVariance, tangentialAccelVariance;
	Pointf gravity;
	GLfloat particleLifespan, particleLifespanVariance;
	Colorf startColor, startColorVariance;
	Colorf finishColor, finishColorVariance;
	GLfloat startParticleSize, startParticleSizeVariance;
	GLfloat finishParticleSize, finishParticleSizeVariance;
	GLuint maxParticles;
	GLint particleCount;
	GLfloat emissionRate;
	GLfloat emitCounter;
	GLfloat elapsedTime;
	GLfloat duration;
    GLfloat rotationStart, rotationStartVariance;
    GLfloat rotationEnd, rotationEndVariance;
	bool rotationIsDir;
	
	int blendFuncSource, blendFuncDestination;
	
	//////////////////// Particle ivars only used when a maxRadius value is provided.  These values are used for
	//////////////////// the special purpose of creating the spinning portal emitter
	GLfloat maxRadius;						// Max radius at which particles are drawn when rotating
	GLfloat maxRadiusVariance;				// Variance of the maxRadius
	GLfloat radiusSpeed;					// The speed at which a particle moves from maxRadius to minRadius
	GLfloat minRadius;						// Radius from source below which a particle dies
	GLfloat rotatePerSecond;				// Numeber of degress to rotate a particle around the source pos per second
	GLfloat rotatePerSecondVariance;		// Variance in degrees for rotatePerSecond
	
	//////////////////// Particle Emitter iVars
	bool active;
	bool useTexture;
	GLint particleIndex;		// Stores the number of particles that are going to be rendered
    GLint vertexIndex;         // Stores the index of the vertices being used for each particle
	
	
	///////////////////// Render
	GLuint indeicsID;
	GLuint verticesID;			// Holds the buffer name of the VBO that stores the color and vertices info for the particles
    GLuint vertexObjectID;
	Particle *particles;		// Array of particles that hold the particle emitters particle details
	ParticleQuad *quads;        // Array holding quad information for each particle;
    GLushort *indices;          // Array holding an index reference into an array of quads for rendering
	
	void initParticleView();
	void initParticle(Particle *particle);
	
	void setupArrays();
	
	bool addParticle();
	bool isFull();
	void update(float dt);
	
public:
	/**
	 * コンストラクタ.
	 *
	 * ParticleDesignerで作成したデータ(plist)を読み込む事ができます。
	 * 今のところは、他のフォーマットには対応していません。
	 *
	 * @param[in] fname ファイル名
	 */
	ParticleView(const char *fname);
	
	/**
	 * コンストラクタ.
	 * @param[in] context View用のコンテキスト
	 * @deprecated コンテキストを使用しないように変更したので、コンストラクタで設定する必要がない
	 */
	ParticleView(GCContext *context);
	
	/**
	 * デストラクタ.
	 */
	virtual ~ParticleView();
	
	/**
	 * パーティクルデータを読み込みます.
	 * @param[in] fname ファイル名
	 */
	void loadParticleData(const char *fname);
	
	/**
	 * 現在のパーティクルが有効かチェックします.
	 * @return 有効の場合はtrue, それ以外はfalse
	 */
	bool isActive();
	
	/**
	 * パーティクルを再スタートします.
	 */
	void restart();
	
	/**
	 * パーティクルを追加することを停止します.
	 * 既に発生しているパーティクルについては消えるまで残ります.
	 * すべてのパーティクルを消したい場合にはvisibleをfalseにしてください。
	 */
	void stop();
	
	/////////////////////////////////////////////////////////////////
	// Viewからの継承
	/////////////////////////////////////////////////////////////////
	
	/**
	 * 指定されたアニメーションを反映して描画を行います.
	 * @param[in] dt 前回描画からの差分時間
	 * @param[in] animation 反映するアニメーション
	 */
	virtual void draw(double dt, ViewContext *context);
};

#endif /* PARTICLEVIEW_H_ */
