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
 * ParticleView.cpp
 *
 *  Created on: 2011/08/16
 *      Author: GClue, Inc.
 */
#include "ParticleView.h"
#include "ApplicationController.h"
#include "Animation.h"
#include "AnimationSet.h"
#include "Figure.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "Texture.h"
#include "TextureManager.h"
#include "GCMap.h"
#include "GCString.h"
#include "GCVector.h"
#include "PlistHelper.h"
#include "Base64.h"
#include "ZipUtil.h"
#include "PngData.h"
#include "APIGlue.h"

// Macro which returns a random value between -1 and 1
#define RANDOM_MINUS_1_TO_1() ((random() / (GLfloat)0x3fffffff )-1.0f)

// Macro which returns a random number between 0 and 1
#define RANDOM_0_TO_1() ((random() / (GLfloat)0x7fffffff ))

// Macro that allows you to clamp a value within the defined bounds
#define CLAMP(X, A, B) ((X < A) ? A : ((X > B) ? B : X))

#pragma mark -
#pragma mark Inline Functions

// Return a Colorf structure populated with 1.0's
static const Colorf ColorfOnes = {1.0f, 1.0f, 1.0f, 1.0f};

// Return a zero populated Pointf
static const Pointf PointfZero = {0.0f, 0.0f};

// Return a populated Vector2d structure from the floats passed in
static inline Pointf PointfMake(GLfloat x, GLfloat y) {
	return (Pointf) {x, y};
}

// Return a Colorf structure populated with the color values passed in
static inline Colorf ColorfMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	return (Colorf) {red, green, blue, alpha};
}

// Return a Pointf containing v multiplied by s
static inline Pointf PointfMultiply(Pointf v, GLfloat s) {
	return (Pointf) {v.x * s, v.y * s};
}

// Return a Pointf containing v1 + v2
static inline Pointf PointfAdd(Pointf v1, Pointf v2) {
	return (Pointf) {v1.x + v2.x, v1.y + v2.y};
}

// Return a Pointf containing v1 - v2
static inline Pointf PointfSub(Pointf v1, Pointf v2) {
	return (Pointf) {v1.x - v2.x, v1.y - v2.y};
}

// Return the dot product of v1 and v2
static inline GLfloat PointfDot(Pointf v1, Pointf v2) {
	return (GLfloat) v1.x * v2.x + v1.y * v2.y;
}

// Return the length of the vector v
static inline GLfloat PointfLength(Pointf v) {
	return (GLfloat) sqrtf(PointfDot(v, v));
}

// Return a Pointf containing a normalized vector v
static inline Pointf PointfNormalize(Pointf v) {
	return PointfMultiply(v, 1.0f/PointfLength(v));
}

ParticleView::ParticleView(const char *fname) : View() {
	clickable = false;
	shader = new ParticleShader();
	initParticleView();
	loadParticleData(fname);
}

ParticleView::ParticleView(GCContext *context) : View(context) {
	clickable = false;
	shader = new ParticleShader();
	initParticleView();
}

ParticleView::~ParticleView() {
}

void ParticleView::initParticleView()
{
	angle = 0;
	angleVariance = 0;
	blendFuncDestination = 0;
	blendFuncSource = 0;
	duration = 0;
	emitterType = 0;
	finishColor.a = 0;
	finishColor.b = 0;
	finishColor.g = 0;
	finishColor.r = 0;
	finishColorVariance.a = 0;
	finishColorVariance.b = 0;
	finishColorVariance.g = 0;
	finishColorVariance.r = 0;
	finishParticleSize = 0;
	finishParticleSizeVariance = 0;
	gravity.x = 0;
	gravity.y = 0;
	maxParticles = 0;
	maxRadius = 0;
	maxRadiusVariance = 0;
	minRadius = 0;
	particleLifespan = 0;
	particleLifespanVariance = 0;
	radialAccelVariance = 0;
	radialAcceleration = 0;
	rotatePerSecond = 0;
	rotatePerSecondVariance = 0;
	rotationEnd = 0;
	rotationEndVariance = 0;
	rotationStart = 0;
	rotationStartVariance = 0;
	rotationIsDir = 0;
	sourcePositionVariance.x = 0;
	sourcePositionVariance.y = 0;
	sourcePosition.x = 0;
	sourcePosition.y = 0;
	speed = 0;
	speedVariance = 0;
	startColor.a = 0;
	startColor.b = 0;
	startColor.g = 0;
	startColor.r = 0;
	startColorVariance.a = 0;
	startColorVariance.b = 0;
	startColorVariance.g = 0;
	startColorVariance.r = 0;
	startParticleSize = 0;
	startParticleSizeVariance = 0;
	tangentialAccelVariance = 0;
	tangentialAcceleration = 0;
	
	particleCount = 0;
	emissionRate = 0;
	emitCounter = 0;
	
	active = true;
}

void ParticleView::loadParticleData(const char *fname)
{
	GCMap *plist = createMapFromFile(fname);
	
	std::vector<std::string> keys;
	plist->allKeys(keys);
	
	int size = keys.size();
	for (int i = 0; i < size; i++) {
		std::string key = keys.at(i);
		GCString *value = (GCString *) plist->objectForKey(key);
		if (key.compare("angle") == 0) {
			angle = value->doubleValue();
		} else if (key.compare("angleVariance") == 0) {
			angleVariance = value->doubleValue();
		} else if (key.compare("blendFuncDestination") == 0) {
			blendFuncDestination = value->intValue();
		} else if (key.compare("blendFuncSource") == 0) {
			blendFuncSource = value->intValue();
		} else if (key.compare("duration") == 0) {
			duration = value->doubleValue();
		} else if (key.compare("emitterType") == 0) {
			emitterType = value->intValue();
		} else if (key.compare("finishColorAlpha") == 0) {
			finishColor.a = value->doubleValue();
		} else if (key.compare("finishColorBlue") == 0) {
			finishColor.b = value->doubleValue();
		} else if (key.compare("finishColorGreen") == 0) {
			finishColor.g = value->doubleValue();
		} else if (key.compare("finishColorRed") == 0) {
			finishColor.r = value->doubleValue();
		} else if (key.compare("finishColorVarianceAlpha") == 0) {
			finishColorVariance.a = value->doubleValue();
		} else if (key.compare("finishColorVarianceBlue") == 0) {
			finishColorVariance.b = value->doubleValue();
		} else if (key.compare("finishColorVarianceGreen") == 0) {
			finishColorVariance.g = value->doubleValue();
		} else if (key.compare("finishColorVarianceRed") == 0) {
			finishColorVariance.r = value->doubleValue();
		} else if (key.compare("finishParticleSize") == 0) {
			finishParticleSize = value->doubleValue();
		} else if (key.compare("finishParticleSizeVariance") == 0) {
			finishParticleSizeVariance = value->doubleValue();
		} else if (key.compare("gravityx") == 0) {
			gravity.x = value->doubleValue();
		} else if (key.compare("gravityy") == 0) {
			gravity.y = value->doubleValue();
		} else if (key.compare("maxParticles") == 0) {
			maxParticles = value->intValue();
		} else if (key.compare("maxRadius") == 0) {
			maxRadius = value->doubleValue();
		} else if (key.compare("maxRadiusVariance") == 0) {
			maxRadiusVariance = value->doubleValue();
		} else if (key.compare("minRadius") == 0) {
			minRadius = value->doubleValue();
		} else if (key.compare("particleLifespan") == 0) {
			particleLifespan = value->doubleValue();
		} else if (key.compare("particleLifespanVariance") == 0) {
			particleLifespanVariance = value->doubleValue();
		} else if (key.compare("radialAccelVariance") == 0) {
			radialAccelVariance = value->doubleValue();
		} else if (key.compare("radialAcceleration") == 0) {
			radialAcceleration = value->doubleValue();
		} else if (key.compare("rotatePerSecond") == 0) {
			rotatePerSecond = value->doubleValue();
		} else if (key.compare("rotatePerSecondVariance") == 0) {
			rotatePerSecondVariance = value->doubleValue();
		} else if (key.compare("rotationEnd") == 0) {
			rotationEnd = value->doubleValue();
		} else if (key.compare("rotationStart") == 0) {
			rotationStart = value->doubleValue();
		} else if (key.compare("rotationStartVariance") == 0) {
			rotationStartVariance = value->doubleValue();
		} else if (key.compare("rotationIsDir") == 0) {
			rotationIsDir = value->intValue();
		} else if (key.compare("sourcePositionVariancex") == 0) {
			sourcePositionVariance.x = value->doubleValue();
		} else if (key.compare("sourcePositionVariancey") == 0) {
			sourcePositionVariance.y = value->doubleValue();
		} else if (key.compare("sourcePositionx") == 0) {
			sourcePosition.x = value->doubleValue();
		} else if (key.compare("sourcePositiony") == 0) {
			sourcePosition.y = value->doubleValue();
		} else if (key.compare("speed") == 0) {
			speed = value->doubleValue();
		} else if (key.compare("speedVariance") == 0) {
			speedVariance = value->doubleValue();
		} else if (key.compare("startColorAlpha") == 0) {
			startColor.a = value->doubleValue();
		} else if (key.compare("startColorBlue") == 0) {
			startColor.b = value->doubleValue();
		} else if (key.compare("startColorGreen") == 0) {
			startColor.g = value->doubleValue();
		} else if (key.compare("startColorRed") == 0) {
			startColor.r = value->doubleValue();
		} else if (key.compare("startColorVarianceAlpha") == 0) {
			startColorVariance.a = value->doubleValue();
		} else if (key.compare("startColorVarianceBlue") == 0) {
			startColorVariance.b = value->doubleValue();
		} else if (key.compare("startColorVarianceGreen") == 0) {
			startColorVariance.g = value->doubleValue();
		} else if (key.compare("startColorVarianceRed") == 0) {
			startColorVariance.r = value->doubleValue();
		} else if (key.compare("startParticleSize") == 0) {
			startParticleSize = value->doubleValue();
		} else if (key.compare("startParticleSizeVariance") == 0) {
			startParticleSizeVariance = value->doubleValue();
		} else if (key.compare("tangentialAccelVariance") == 0) {
			tangentialAccelVariance = value->doubleValue();
		} else if (key.compare("tangentialAcceleration") == 0) {
			tangentialAcceleration = value->doubleValue();
		}
	}
	
	emissionRate = maxParticles / particleLifespan;
	
	GCString *name = (GCString *) plist->objectForKey("textureFileName");
	GCString *str = (GCString *) plist->objectForKey("textureImageData");
	if (name && str) {
		std::string dat;
		std::vector<unsigned char> dst;
		Base64::decode(str->getString(), dst);
		ZipUtil::inflateZip(&(dst)[0], dst.size(), dat);
		
		// TODO: PNGにしか対応していないが、実際にはTIFFなどの他のフォーマットのデータが入っていることがある。
		
		PngData png;
		if (png.loadData((unsigned char *) dat.c_str(), dat.length())) {
			texture = new Texture();
			texture->setImageData(png.data, png.width, png.height);
		}
	}
	plist->release();
	
	setupArrays();
}

void ParticleView::setupArrays()
{
	// Allocate the memory necessary for the particle emitter arrays
	particles = (Particle *) malloc(sizeof(Particle) * maxParticles);
    quads = (ParticleQuad *) calloc(maxParticles, sizeof(ParticleQuad));
    indices = (GLushort *) calloc(maxParticles * 6, sizeof(GLushort));
    
    // Set up the indices for all particles. This provides an array of indices into the quads array that is used during
    // rendering. As we are rendering quads there are six indices for each particle as each particle is made of two triangles
    // that are each defined by three vertices.
    for( int i=0;i< maxParticles;i++) {
		indices[i*6+0] = i*4+0;
		indices[i*6+1] = i*4+1;
		indices[i*6+2] = i*4+2;
		
		indices[i*6+5] = i*4+3;
		indices[i*6+4] = i*4+2;
		indices[i*6+3] = i*4+1;
	}
	
    // Set up texture coordinates for all particles as these will not change.
    for(int i=0; i<maxParticles; i++) {
        quads[i].bl.texture.x = 0;
        quads[i].bl.texture.y = 0;
        
        quads[i].br.texture.x = 1.0;
        quads[i].br.texture.y = 0;
		
        quads[i].tl.texture.x = 0;
        quads[i].tl.texture.y = 1.0;
        
        quads[i].tr.texture.x = 1.0;
        quads[i].tr.texture.y = 1.0;
	}
	
	// Generate the vertices VBO
	glGenBuffers(1, &verticesID);
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleQuad) * maxParticles, quads, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &indeicsID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeicsID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * maxParticles * 6, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	// By default the particle emitter is active when created
	active = true;
	
	// Set the particle count to zero
	particleCount = 0;
	
	// Reset the elapsed time
	elapsedTime = 0;
}


bool ParticleView::isActive() {
	return active;
}

void ParticleView::initParticle(Particle *particle)
{
	// Init the position of the particle.  This is based on the source position of the particle emitter
	// plus a configured variance.  The GetRandom1_1 macro allows the number to be both positive
	// and negative
	particle->position.x = sourcePosition.x + sourcePositionVariance.x * GetRandom1_1();
	particle->position.y = sourcePosition.y + sourcePositionVariance.y * GetRandom1_1();
    particle->startPos.x = sourcePosition.x;
    particle->startPos.y = sourcePosition.y;
	
	// Init the direction of the particle.  The newAngle is calculated using the angle passed in and the
	// angle variance.
	float newAngle = (GLfloat)DEGREES_TO_RADIANS(angle + angleVariance * GetRandom1_1());
	
	// Create a new Pointf using the newAngle
	Pointf vector = PointfMake(cosf(newAngle), sinf(newAngle));
	
	// Calculate the vectorSpeed using the speed and speedVariance which has been passed in
	float vectorSpeed = speed + speedVariance * GetRandom1_1();
	
	// The particles direction vector is calculated by taking the vector calculated above and
	// multiplying that by the speed
	particle->direction = PointfMultiply(vector, vectorSpeed);
	
	// Set the default diameter of the particle from the source position
	particle->radius = maxRadius + maxRadiusVariance * GetRandom1_1();
	particle->radiusDelta = (maxRadius / particleLifespan) * (1.0 / MAXIMUM_UPDATE_RATE);
	particle->angle = DEGREES_TO_RADIANS(angle + angleVariance * GetRandom1_1());
	particle->degreesPerSecond = DEGREES_TO_RADIANS(rotatePerSecond + rotatePerSecondVariance * GetRandom1_1());
    
    particle->radialAcceleration = radialAcceleration;
    particle->tangentialAcceleration = tangentialAcceleration;
	
	// Calculate the particles life span using the life span and variance passed in
	particle->timeToLive = MAX(0, particleLifespan + particleLifespanVariance * GetRandom1_1());
	
	// Calculate the particle size using the start and finish particle sizes
	GLfloat particleStartSize = startParticleSize + startParticleSizeVariance * GetRandom1_1();
	GLfloat particleFinishSize = finishParticleSize + finishParticleSizeVariance * GetRandom1_1();
	particle->particleSizeDelta = ((particleFinishSize - particleStartSize) / particle->timeToLive) * (1.0 / MAXIMUM_UPDATE_RATE);
	particle->particleSize = MAX(0, particleStartSize);
	
	// Calculate the color the particle should have when it starts its life.  All the elements
	// of the start color passed in along with the variance are used to calculate the star color
	Colorf start = {0, 0, 0, 0};
	start.r = startColor.r + startColorVariance.r * GetRandom1_1();
	start.g = startColor.g + startColorVariance.g * GetRandom1_1();
	start.b = startColor.b + startColorVariance.b * GetRandom1_1();
	start.a = startColor.a + startColorVariance.a * GetRandom1_1();
	
	// Calculate the color the particle should be when its life is over.  This is done the same
	// way as the start color above
	Colorf end = {0, 0, 0, 0};
	end.r = finishColor.r + finishColorVariance.r * GetRandom1_1();
	end.g = finishColor.g + finishColorVariance.g * GetRandom1_1();
	end.b = finishColor.b + finishColorVariance.b * GetRandom1_1();
	end.a = finishColor.a + finishColorVariance.a * GetRandom1_1();
	
	// Calculate the delta which is to be applied to the particles color during each cycle of its
	// life.  The delta calculation uses the life span of the particle to make sure that the
	// particles color will transition from the start to end color during its life time.  As the game
	// loop is using a fixed delta value we can calculate the delta color once saving cycles in the
	// update method
	particle->color = start;
	particle->deltaColor.r = ((end.r - start.r) / particle->timeToLive) * (1.0 / MAXIMUM_UPDATE_RATE);
	particle->deltaColor.g = ((end.g - start.g) / particle->timeToLive) * (1.0 / MAXIMUM_UPDATE_RATE);
	particle->deltaColor.b = ((end.b - start.b) / particle->timeToLive) * (1.0 / MAXIMUM_UPDATE_RATE);
	particle->deltaColor.a = ((end.a - start.a) / particle->timeToLive) * (1.0 / MAXIMUM_UPDATE_RATE);
    
    // Calculate the rotation
    GLfloat startA = rotationStart + rotationStartVariance * GetRandom1_1();
    GLfloat endA = rotationEnd + rotationEndVariance * GetRandom1_1();
    particle->rotation = startA;
    particle->rotationDelta = (endA - startA) / particle->timeToLive;
}

bool ParticleView::addParticle()
{
	if (isFull()) {
		return false;
	}
	
	// Take the next particle out of the particle pool we have created and initialize it
	Particle *particle = &particles[particleCount];
	initParticle(particle);
	
	// Increment the particle count
	particleCount++;
	
	return true;
}

bool ParticleView::isFull()
{
	return particleCount >= maxParticles - 1;
}

void ParticleView::restart()
{
	active = true;
    elapsedTime = 0;
	particleCount = 0;
	for (int i = 0; i < maxParticles; i++) {
        Particle *p = &particles[i];
        p->timeToLive = 0;
    }
}

void ParticleView::stop()
{
	active = false;
	elapsedTime = duration;
	emitCounter = 0;
}

void ParticleView::update(float dt)
{
	if (active && emissionRate) {
		float rate = 1.0f / emissionRate;
		emitCounter += dt;
		while (particleCount < maxParticles && emitCounter > rate) {
			addParticle();
			emitCounter -= rate;
		}
		
		elapsedTime += dt;
		if (duration != -1 && duration < elapsedTime) {
			stop();
		}
	}
	
	particleIndex = 0;
	
	while (particleIndex < particleCount) {
		Particle *currentParticle = &particles[particleIndex];
        currentParticle->timeToLive -= dt;
		if (currentParticle->timeToLive > 0) {
			if (emitterType == kParticleTypeRadial) {
				currentParticle->angle += currentParticle->degreesPerSecond * dt;
				currentParticle->radius -= currentParticle->radiusDelta;
                
				Pointf tmp;
				tmp.x = sourcePosition.x - cosf(currentParticle->angle) * currentParticle->radius;
				tmp.y = sourcePosition.y - sinf(currentParticle->angle) * currentParticle->radius;
				currentParticle->position = tmp;
				
				if (currentParticle->radius < minRadius) {
					currentParticle->timeToLive = 0;
				}
			} else {
				Pointf tmp, radial, tangential;
                
                radial = PointfZero;
                Pointf diff = PointfSub(currentParticle->startPos, PointfZero);
                
                currentParticle->position = PointfSub(currentParticle->position, diff);
                
                if (currentParticle->position.x || currentParticle->position.y) {
                    radial = PointfNormalize(currentParticle->position);
				}
                
                tangential.x = radial.x;
                tangential.y = radial.y;
                radial = PointfMultiply(radial, currentParticle->radialAcceleration);
                
                GLfloat newy = tangential.x;
                tangential.x = -tangential.y;
                tangential.y = newy;
                tangential = PointfMultiply(tangential, currentParticle->tangentialAcceleration);
                
				tmp = PointfAdd( PointfAdd(radial, tangential), gravity);
                tmp = PointfMultiply(tmp, dt);
				currentParticle->direction = PointfAdd(currentParticle->direction, tmp);
				tmp = PointfMultiply(currentParticle->direction, dt);
				currentParticle->position = PointfAdd(currentParticle->position, tmp);
                currentParticle->position = PointfAdd(currentParticle->position, diff);
			}
			
			// Update the particles color
			currentParticle->color.r += currentParticle->deltaColor.r;
			currentParticle->color.g += currentParticle->deltaColor.g;
			currentParticle->color.b += currentParticle->deltaColor.b;
			currentParticle->color.a += currentParticle->deltaColor.a;
			
			// Update the particle size
			currentParticle->particleSize += currentParticle->particleSizeDelta;
			
            // Update the rotation of the particle
            currentParticle->rotation += (currentParticle->rotationDelta * dt);
			
            // As we are rendering the particles as quads, we need to define 6 vertices for each particle
            GLfloat halfSize = currentParticle->particleSize * 0.5f;
			
            // If a rotation has been defined for this particle then apply the rotation to the vertices that define
            // the particle
            if (currentParticle->rotation) {
                float x1 = -halfSize;
                float y1 = -halfSize;
                float x2 = halfSize;
                float y2 = halfSize;
                float x = currentParticle->position.x;
                float y = currentParticle->position.y;
                float r = DEGREES_TO_RADIANS(currentParticle->rotation);
                float cr = cosf(r);
                float sr = sinf(r);
                float ax = x1 * cr - y1 * sr + x;
                float ay = x1 * sr + y1 * cr + y;
                float bx = x2 * cr - y1 * sr + x;
                float by = x2 * sr + y1 * cr + y;
                float cx = x2 * cr - y2 * sr + x;
                float cy = x2 * sr + y2 * cr + y;
                float dx = x1 * cr - y2 * sr + x;
                float dy = x1 * sr + y2 * cr + y;
                
				// iPhoneの大きさを元に計算されているみたいなので、中心にくるようにしてある
                quads[particleIndex].bl.vertex.x = (ax - 160.0) / 320.0;
                quads[particleIndex].bl.vertex.y = (ay - 160.0) / 320.0;
                quads[particleIndex].bl.color = currentParticle->color;
                
                quads[particleIndex].br.vertex.x = (bx - 160.0) / 320.0;
                quads[particleIndex].br.vertex.y = (by - 160.0) / 320.0;
                quads[particleIndex].br.color = currentParticle->color;
                
                quads[particleIndex].tl.vertex.x = (dx - 160.0) / 320.0;
                quads[particleIndex].tl.vertex.y = (dy - 160.0) / 320.0;
                quads[particleIndex].tl.color = currentParticle->color;
                
                quads[particleIndex].tr.vertex.x = (cx - 160.0) / 320.0;
                quads[particleIndex].tr.vertex.y = (cy - 160.0) / 320.0;
                quads[particleIndex].tr.color = currentParticle->color;
            } else {
                // Using the position of the particle, work out the four vertices for the quad that will hold the particle
                // and load those into the quads array.
                quads[particleIndex].bl.vertex.x = (currentParticle->position.x - halfSize - 160.0) / 320.0;
                quads[particleIndex].bl.vertex.y = (currentParticle->position.y - halfSize - 160.0) / 320.0;
                quads[particleIndex].bl.color = currentParticle->color;
                
                quads[particleIndex].br.vertex.x = (currentParticle->position.x + halfSize - 160.0) / 320.0;
                quads[particleIndex].br.vertex.y = (currentParticle->position.y - halfSize - 160.0) / 320.0;
                quads[particleIndex].br.color = currentParticle->color;
                
                quads[particleIndex].tl.vertex.x = (currentParticle->position.x - halfSize - 160.0) / 320.0;
                quads[particleIndex].tl.vertex.y = (currentParticle->position.y + halfSize - 160.0) / 320.0;
                quads[particleIndex].tl.color = currentParticle->color;
                
                quads[particleIndex].tr.vertex.x = (currentParticle->position.x + halfSize - 160.0) / 320.0;
                quads[particleIndex].tr.vertex.y = (currentParticle->position.y + halfSize - 160.0) / 320.0;
                quads[particleIndex].tr.color = currentParticle->color;
            }
			
			// Update the particle and vertex counters
			particleIndex++;
		} else {
			
			// As the particle is not alive anymore replace it with the last active particle
			// in the array and reduce the count of particles by one.  This causes all active particles
			// to be packed together at the start of the array so that a particle which has run out of
			// life will only drop into this clause once
			if(particleIndex != particleCount - 1)
				particles[particleIndex] = particles[particleCount - 1];
			particleCount--;
		}
	}
}

void ParticleView::draw(double dt, ViewContext *context) {
	if (animation && !animation->isStarted()) {
		return;
	}
	
	update(dt);
	
	shader->useProgram();
	
	float alpha = 1.0;
	float bright = 1.0;
	Matrix3D mtx;
	testMatrix3D(&mtx, &alpha, &bright);
	
	shader->bindTexture(texture->texName);
	shader->setMVPMatrix(context->camera, &mtx);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeicsID);
	glBindBuffer(GL_ARRAY_BUFFER, verticesID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ParticleQuad) * particleIndex, quads);
	
	glEnableVertexAttribArray(PARTICLE_ATTR_VERT);
	glEnableVertexAttribArray(PARTICLE_ATTR_TEXCOORD);
	glEnableVertexAttribArray(PARTICLE_ATTR_COLOR);
	
	glVertexAttribPointer(PARTICLE_ATTR_VERT, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), 0);
	glVertexAttribPointer(PARTICLE_ATTR_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (GLvoid *) offsetof(TexturedColoredVertex, texture));
	glVertexAttribPointer(PARTICLE_ATTR_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (GLvoid *) offsetof(TexturedColoredVertex, color));
	
	glBlendFunc(blendFuncSource, blendFuncDestination);
	
	glDrawElements(GL_TRIANGLES, particleIndex * 6, GL_UNSIGNED_SHORT, NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	context->shader->useProgram();
}
