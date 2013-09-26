//
//  Layer3D.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/08/21.
//
//

#include "Layer3D.h"
#include "glcommon.h"
#include "DepthStorageShader.h"
#include "DepthShadowShader.h"
#include "SimpleShader.h"
#include "BoneShader.h"
#include "Animation.h"
#include "AnimationSet.h"

FigureSet::FigureSet() {
	parent = NULL;
	visible = true;
	removeFlag = false;
	useEdge = false;
	shadowFlag = true;
	figure = NULL;
	texture = NULL;
	textureMlt = NULL;
	animation = NULL;
	userId = 0;
	matrix.loadIdentity();
	edgeColor.r = 0;
	edgeColor.g = 0;
	edgeColor.b = 0;
	edgeColor.a = 1;
	edgeSize = 0.4;
	alpha = 1.0f;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	scaleSize.x = 1;
	scaleSize.y = 1;
	scaleSize.z = 1;
}

FigureSet::~FigureSet() {
	for (int i = 0; i < childrens.size(); i++) {
		childrens.at(i)->release();
	}
	childrens.clear();
}

void FigureSet::addChildern(FigureSet *set) {
	if (set && !set->parent) {
		childrens.push_back(set);
		set->parent = this;
		set->retain();
	}
}

void FigureSet::removeChildern(FigureSet *set) {
	std::vector<FigureSet *>::iterator it = childrens.begin();
	while (!childrens.empty() && it != childrens.end()) {
		FigureSet *a = *it;
		if (a == set) {
			it = childrens.erase(it);
            a->parent = NULL;
			a->release();
		} else {
			it++;
		}
	}
}

void FigureSet::removeChildern(int userId) {
	FigureSet *set = findFigureSetByID(userId);
	if (set) {
		removeChildern(set);
	}
}

void FigureSet::startAnimation(IAnimation *a) {
	if (animation) {
		delete animation;
	}
	animation = a;
}

void FigureSet::setVisible(bool visible) {
	this->visible = visible;
}

bool FigureSet::isVisible() {
	return visible;
}

bool FigureSet::isRemoveFlag() {
	return removeFlag;
}

void FigureSet::setRemoveFlag(bool flag) {
	removeFlag = flag;
}

void FigureSet::setUseEdge(bool flag) {
	useEdge = flag;
}

bool FigureSet::isUseEdge() {
	return useEdge;
}

void FigureSet::setEdgeColor(float r, float g, float b, float a) {
	edgeColor.r = r;
	edgeColor.g = g;
	edgeColor.b = b;
	edgeColor.a = a;
}

void FigureSet::setEdgeSize(float size) {
	edgeSize = size;
}

void FigureSet::setShadowFlag(bool flag) {
	shadowFlag = flag;
}

void FigureSet::setAlpha(float alpha) {
	this->alpha = alpha;
}

void FigureSet::setUserID(int userId) {
	this->userId = userId;
}

int FigureSet::getUserID() {
	return userId;
}

FigureSet *FigureSet::findFigureSetByID(int userId) {
	if (this->userId == userId) {
		return this;
	}
	for (int i = 0; i < childrens.size(); i++) {
		FigureSet *set = childrens.at(i)->findFigureSetByID(userId);
		if (set) {
			return set;
		}
	}
	return NULL;
}

void FigureSet::makeIdentity() {
	matrix.loadIdentity();
}

void FigureSet::translate(float x, float y, float z) {
	matrix.translate(x, y, z);
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void FigureSet::scale(float x, float y, float z) {
	matrix.scale(x, y, z);
	scaleSize.x = x;
	scaleSize.y = y;
	scaleSize.z = z;
}

void FigureSet::rotate(float angle, RotateDir dir) {
	matrix.rotate(angle, dir);
}

void FigureSet::setFigure(Figure *fig) {
	this->figure = fig;
	this->figure->build();
}

void FigureSet::setTexture(Texture *texture) {
	this->texture = texture;
}

void FigureSet::setTextureMlt(Texture *texture) {
	this->textureMlt = texture;
}

void FigureSet::testMatrix(Matrix3D *m) {
	if (animation) {
		animation->multiply(m);
	}
	m->multiply(&matrix, true);
	if (parent) {
		parent->testMatrix(m);
	}
}

static float  shadowMat[16];
static void
myShadowMatrix(float ground[4], float light[4])
{
    float  dot;
	
    dot = ground[0] * light[0] +
	ground[1] * light[1] +
	ground[2] * light[2] +
	ground[3] * light[3];
    
    shadowMat[0] = dot - light[0] * ground[0];
    shadowMat[1] = 0.0 - light[0] * ground[1];
    shadowMat[2] = 0.0 - light[0] * ground[2];
    shadowMat[3] = 0.0 - light[0] * ground[3];
    
    shadowMat[4] = 0.0 - light[1] * ground[0];
    shadowMat[5] = dot - light[1] * ground[1];
    shadowMat[6] = 0.0 - light[1] * ground[2];
    shadowMat[7] = 0.0 - light[1] * ground[3];
    
    shadowMat[8] = 0.0 - light[2] * ground[0];
    shadowMat[9] = 0.0 - light[2] * ground[1];
    shadowMat[10] = dot - light[2] * ground[2];
    shadowMat[11] = 0.0 - light[2] * ground[3];
    
    shadowMat[12] = 0.0 - light[3] * ground[0];
    shadowMat[13] = 0.0 - light[3] * ground[1];
    shadowMat[14] = 0.0 - light[3] * ground[2];
    shadowMat[15] = dot - light[3] * ground[3];
}

void FigureSet::render(float dt, GC3DContext &context) {
	if (!visible) {
		return;
	}
	
	if (animation) {
		animation->step(dt);
	}
	
	Matrix3D mtx;
	testMatrix(&mtx);
	
	figure->bind();

	switch (context.type) {
		case DepthStorageShaderType: {
			if (shadowFlag) {
				DepthStorageShader *shader = context.depthShader;
				shader->setMVPMatrix(context.lightcamera, &mtx);
				shader->setSkinningMatrix(figure);
				figure->draw(dt);
			}
		}	break;
		case DepthShadowShaderType: {
			DepthShadowShader *shader = context.shadowShader;
			shader->setAlpha(alpha);
			shader->setModelMatrix(&mtx);
			shader->setMVPMatrix(context.camera, &mtx);
			shader->setLightMatrix(context.lightcamera, &mtx);
			shader->setInverseMatrix(&mtx);
			shader->setSkinningMatrix(figure);
			shader->setUseShadow(shadowFlag);
			if (texture) {
				shader->bindTexture(texture->texName);
			} else {
				shader->bindTexture(0);
			}
			
			if (textureMlt) {
				shader->bindTextureMlt(textureMlt->texName);
			} else {
				shader->bindTextureMlt(0);
			}

			if (useEdge) {
				glCullFace(GL_FRONT);
				shader->setUseEdge(true);
				shader->setEdgeColor(edgeColor.r, edgeColor.g, edgeColor.b, edgeColor.a);
				shader->setEdgeSize(edgeSize);
				figure->draw(dt);
				
				glCullFace(GL_BACK);
				shader->setUseEdge(false);
				shader->setEdgeColor(0, 0, 0, 0);
				figure->draw(0);
			} else {
				figure->draw(0);
			}
		}	break;
		case BoneShaderType: {
			BoneShader *shader = context.boneShader;
			shader->setMVPMatrix(context.camera, &mtx);
			shader->setSkinningMatrix(figure);
			if (texture) {
				shader->bindTexture(texture->texName);
			} else {
				shader->bindTexture(0);
			}
			
			if (textureMlt) {
				shader->bindTextureMlt(textureMlt->texName);
			} else {
				shader->bindTextureMlt(0);
			}
			
			if (shadowFlag) {
				shader->setNormalMatrix(&mtx);
				shader->setLight(context.light);
			} else {
				shader->setLight(NULL);
			}
			
			if (useEdge) {
				glCullFace(GL_FRONT);
				shader->setUseEdge(true);
				shader->setEdgeColor(edgeColor.r, edgeColor.g, edgeColor.b, edgeColor.a);
				shader->setEdgeSize(edgeSize);
				figure->draw(dt);
				
				glCullFace(GL_BACK);
				shader->setUseEdge(false);
				shader->setEdgeColor(0, 0, 0, 0);
				figure->draw(0);
			} else {
				figure->draw(dt);
			}
		}	break;
		case SimpleShaderType:
		default: {
			SimpleShader *shader = context.simpleShader;
			shader->setMVPMatrix(context.camera, &matrix);
//			if (texture) {
//				shader->bindTexture(texture->texName);
//			}
			figure->draw(dt);
		}	break;
	}
	
	for (int i = 0; i < childrens.size(); i++) {
		childrens.at(i)->render(dt, context);
	}
}

/////////////////////////////////////////////////////////////////////////////////////

Layer3D::Layer3D() {
	lockflag = false;
	depthBufferFlag = false;
	useShadowFlag = false;
	renderMode = 0;
	
	memset(&fb, 0x0, sizeof(GCFrameBuffer));
	
	Vector3D eye(40,30,40);
	Vector3D at(0,0,0);
	Vector3D up(0,1,0);
	
	camera.zNear = 0.1;
	camera.zFar = 1000.0;
	camera.fieldOfView = 60.0;
	camera.aspect = 0.67;
	camera.loadPerspective();
	camera.lookAt(eye, at, up);
	
	fbWidth = 1024;
	fbHeight = 1024;
	near = 0.1;
	far = 500;
	
	boneShader = new BoneShader();
	depthShader = NULL;
	shadowShader = NULL;
	
	light.setPosition(0, 35, 0);
	Vector3D lightup(0,0,-1);
	
	lightcamera.zNear = near;
	lightcamera.zFar = far;
	lightcamera.fieldOfView = 60.0;
	lightcamera.aspect = 1;
	lightcamera.lookAt(light.position, at, lightup);
	lightcamera.loadPerspective();
	
	gc3dcontext.boneShader = boneShader;
	gc3dcontext.depthShader = NULL;
	gc3dcontext.shadowShader = NULL;
	gc3dcontext.simpleShader = simpleShader;
	gc3dcontext.camera = &camera;
	gc3dcontext.lightcamera = NULL;
	gc3dcontext.shadowFlag = false;
	gc3dcontext.light = &light;
	
	createDepthShadow(1024, 1024);
	
	// ここから先はデバック用データ
	// デバック用シェーダ
	simpleShader = new SimpleShader();
	// ライト表示用フィギュア
	lightfigure = new FigureSet();
	lightfigure->setFigure(createSphere(0.5, 8, 8));
}

Layer3D::~Layer3D() {
	removeAllFigureSet();
	
	DELETE(lightfigure);
	DELETE(boneShader);
	DELETE(simpleShader);
	
	destroyDepthShadow();
}

void Layer3D::removeInternal() {
	std::vector<FigureSet*>::iterator it = figures.begin();
	while (!figures.empty() && it != figures.end()) {
		FigureSet *a = *it;
		if (a->isRemoveFlag()) {
			it = figures.erase(it);
			RELEASE(a);
		} else {
			it++;
		}
	}
}

void Layer3D::addFigureSet(FigureSet *set) {
	if (set && !set->getParent()) {
		figures.push_back(set);
		set->retain();
	}
}

void Layer3D::removeFigureSet(int userId) {
	FigureSet *set = findFigureSetByID(userId);
	if (set) {
		removeFigureSet(set);
	}
}

void Layer3D::removeFigureSet(FigureSet *set) {
	std::vector<FigureSet*>::iterator it = figures.begin();
	while (!figures.empty() && it != figures.end()) {
		FigureSet *a = *it;
		if (a == set) {
			if (lockflag) {
				a->setRemoveFlag(true);
			} else {
				it = figures.erase(it);
				RELEASE(a);
			}
		} else {
			it++;
		}
	}
}

void Layer3D::removeAllFigureSet() {
	for (int i = 0; i < figures.size(); i++) {
		RELEASE(figures.at(i));
	}
	figures.clear();
}

FigureSet *Layer3D::findFigureSetByID(int userId) {
	std::vector<FigureSet *>::iterator it = figures.begin();
	while (!figures.empty() && it != figures.end()) {
		FigureSet *a = *it;
		FigureSet *v = a->findFigureSetByID(userId);
		if (v != NULL) {
			return v;
		}
		it++;
	}
	return NULL;
}

void Layer3D::createDepthShadow(int width, int height)
{
	fbWidth = width;
	fbHeight = height;
	
	depthShader = new DepthStorageShader();
	shadowShader = new DepthShadowShader();
	
	lightfigure = new FigureSet();
	lightfigure->setFigure(createSphere(0.5, 8, 8));
	
	light.setPosition(0, 35, 0);
	Vector3D lightup(0,0,-1);
	Vector3D at(0,0,0);
	
	lightcamera.zNear = 0.1;
	lightcamera.zFar = 300.0;
	lightcamera.fieldOfView = 100.0;
	lightcamera.aspect = 1;
	lightcamera.lookAt(light.position, at, lightup);
	lightcamera.loadPerspective();
	
	createFrameBuffer(width, height);
	
	gc3dcontext.depthShader = depthShader;
	gc3dcontext.simpleShader = simpleShader;
	gc3dcontext.shadowShader = shadowShader;
	gc3dcontext.boneShader = boneShader;
	gc3dcontext.camera = &camera;
	gc3dcontext.lightcamera = &lightcamera;
	gc3dcontext.shadowFlag = false;
	gc3dcontext.light = &light;
}

void Layer3D::destroyDepthShadow()
{
	DELETE(depthShader);
	DELETE(shadowShader);
	if (fb.fb) {
		glDeleteFramebuffers(1, &fb.fb);
		fb.fb = 0;
	}
	if (fb.rb) {
		glDeleteRenderbuffers(1, &fb.rb);
		fb.rb = 0;
	}
	if (fb.t) {
		glDeleteTextures(1 , &fb.t);
		fb.t = 0;
	}
	
	gc3dcontext.depthShader = NULL;
	gc3dcontext.shadowShader = NULL;
	gc3dcontext.lightcamera = NULL;
	gc3dcontext.shadowFlag = false;
	gc3dcontext.light = &light;
}

void Layer3D::createFrameBuffer(int width, int height)
{
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	GLuint depthRenderBuffer;
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
	
	GLuint fTexture;
	glGenTextures(1, &fTexture);
	
	glBindTexture(GL_TEXTURE_2D, fTexture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fTexture, 0);
	
	glBindTexture(GL_TEXTURE_2D, NULL);
	glBindRenderbuffer(GL_RENDERBUFFER, NULL);
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	
	fb.fb = frameBuffer;
	fb.rb = depthRenderBuffer;
	fb.t = fTexture;
}

void Layer3D::setLightPos(float x, float y, float z) {
	light.setPosition(x, y, z);
	Vector3D at(0,0,0);
	Vector3D up(0,0,-1);
	lightcamera.lookAt(light.position, at, up);
}

void Layer3D::setShadowFlag(bool flag) {
	if (flag && gc3dcontext.depthShader) {
		renderMode = RenderTypeShadow;
	} else {
		renderMode = RenderTypeNone;
	}
}

void Layer3D::drawScene(float dt) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	
	boneShader->useProgram();
	
	gc3dcontext.type = BoneShaderType;
	
	for (int i = 0; i < figures.size(); i++) {
		figures.at(i)->render(dt, gc3dcontext);
	}
}

void Layer3D::drawSceneWithShadow(float dt) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	
	// 現在のviewportを退避
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	// 現在のフレームバッファを退避
	int oldId;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldId);
	
	// depthで影の描画
	depthShader->useProgram();
	
	glBindFramebuffer(GL_FRAMEBUFFER, fb.fb);
	
	// フレームバッファを初期化
	glViewport(0, 0, fbWidth, fbHeight);
	glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepthf(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	gc3dcontext.type = DepthStorageShaderType;
	
	depthShader->setNear(near);
	depthShader->setFar(far);
	depthShader->setTextureSize(fbWidth);
	
	int size = figures.size();
	for (int i = 0; i < size; i++) {
		figures.at(i)->render(dt, gc3dcontext);
	}
	
	// viewportを元に戻す
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	// フレームバッファを元に戻す
	glBindFramebuffer(GL_FRAMEBUFFER, oldId);
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepthf(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
#if 0
	simpleShader->useProgram();
	
	gc3dcontext.type = SimpleShaderType;
	simpleShader->bindTexture(fb.t);
	
	for (int i = 0; i < figures.size(); i++) {
		figures.at(i)->render(0, gc3dcontext);
	}
#else
	glEnable(GL_BLEND);

	shadowShader->useProgram();
	
	gc3dcontext.type = DepthShadowShaderType;
	
	shadowShader->bindShadowTexture(fb.t);
	shadowShader->setLightPosition(&light);
	shadowShader->setTextureMatrix(&lightcamera);
	shadowShader->setNear(near);
	shadowShader->setFar(far);

	for (int i = 0; i < figures.size(); i++) {
		figures.at(i)->render(0, gc3dcontext);
	}
#endif
	
#if 0
	// デバック用にライトの位置を表示
	simpleShader->useProgram();
	
	gc3dcontext.type = -1;
	
	lightfigure->makeIdentity();
	lightfigure->translate(light.getX(), light.getY(), light.getZ());
	lightfigure->render(0, gc3dcontext);
#endif
}


//////////////////////////////////////////////////////////
// Layer の実装
//////////////////////////////////////////////////////////

void Layer3D::setup() {
}

void Layer3D::resize(float aspect) {
	camera.aspect = aspect;
}

void Layer3D::render(double dt) {
	lockflag = true;
	switch (renderMode) {
		case RenderTypeNone:
			drawScene(dt);
			break;
		case RenderTypeShadow:
			drawSceneWithShadow(dt);
			break;
	}
		
	lockflag = false;
	removeInternal();
}

bool Layer3D::onTouchEvent(TouchEvent &event) {
	return false;
}
