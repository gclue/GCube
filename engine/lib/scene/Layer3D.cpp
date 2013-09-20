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
#include "ShadowShader.h"
#include "SimpleShader.h"
#include "BoneShader.h"
#include "Animation.h"
#include "AnimationSet.h"

FigureSet::FigureSet() {
	removeFlag = false;
	useEdge = false;
	figure = NULL;
	texture = NULL;
	animation = NULL;
	userId = 0;
	matrix.loadIdentity();
	edgeColor.r = 0;
	edgeColor.g = 0;
	edgeColor.b = 0;
	edgeColor.a = 1;
	edgeSize = 0.4;
}

FigureSet::~FigureSet() {
}

void FigureSet::startAnimation(IAnimation *a) {
	if (animation) {
		delete animation;
	}
	animation = a;
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
	return NULL;
}

void FigureSet::makeIdentity() {
	matrix.loadIdentity();
}

void FigureSet::translate(float x, float y, float z) {
	matrix.translate(x, y, z);
}

void FigureSet::scale(float x, float y, float z) {
	matrix.scale(x, y, z);
}

void FigureSet::rotate(float angle, float x, float y, float z) {
//	matrix.rotate(angle, x, y, z);
	matrix.rotate(angle, RotateDirX);
}

void FigureSet::setFigure(Figure *fig) {
	this->figure = fig;
	this->figure->build();
}

void FigureSet::setTexture(Texture *texture) {
	this->texture = texture;
}


void FigureSet::testMatrix(Matrix3D *m) {
	if (animation) {
		animation->multiply(m);
	}
	m->multiply(&matrix);
}


void FigureSet::render(float dt, GC3DContext &context) {
	figure->bind();

	switch (context.type) {
		case DepthStorageShaderType: {
			DepthStorageShader *shader = context.depthShader;
			shader->setMVPMatrix(context.lightcamera, &matrix);
			shader->setSkinningMatrix(figure);
			figure->draw(dt);
		}	break;
		case ShadowShaderType: {
			ShadowShader *shader = context.shadowShader;
			shader->setModelMatrix(&matrix);
			shader->setMVPMatrix(context.camera, &matrix);
			shader->setLightMatrix(context.lightcamera, &matrix);
			shader->setInverseMatrix(&matrix);
			shader->setSkinningMatrix(figure);
			shader->setUseShadow(shadowFlag);
			if (texture) {
				shader->bindTexture(texture->texName);
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
		case BoneShaderType: {
			
			if (animation) {
				animation->step(dt);
			}
			
			Matrix3D mtx;
			testMatrix(&mtx);
			
			BoneShader *shader = context.boneShader;
			shader->setMVPMatrix(context.camera, &mtx);
			shader->setSkinningMatrix(figure);
			if (texture) {
				shader->bindTexture(texture->texName);
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
	camera.aspect = 0.67; // 320x480
	camera.loadPerspective();
	camera.lookAt(eye, at, up);
	
	fbWidth = 512;
	fbHeight = 512;
	
	depthShader = new DepthStorageShader();
	shadowShader = new ShadowShader();
	simpleShader = new SimpleShader();
	boneShader = new BoneShader();
	
	lightfigure = new FigureSet();
	lightfigure->setFigure(createSphere(0.5, 8, 8));
	
	light.setPosition(0, 35, 0);
	Vector3D lightup(0,0,-1);
	
	lightcamera.zNear = 0.1;
	lightcamera.zFar = 300.0;
	lightcamera.fieldOfView = 100.0;
	lightcamera.aspect = 1;
	lightcamera.lookAt(light.position, at, lightup);
	lightcamera.loadPerspective();
	
	createFrameBuffer(fbWidth, fbHeight);
	
	gc3dcontext.depthShader = depthShader;
	gc3dcontext.simpleShader = simpleShader;
	gc3dcontext.shadowShader = shadowShader;
	gc3dcontext.boneShader = boneShader;
	gc3dcontext.camera = &camera;
	gc3dcontext.lightcamera = &lightcamera;
	gc3dcontext.shadowFlag = false;
}

Layer3D::~Layer3D() {
	removeAllFigureSet();
	DELETE(lightfigure);
	DELETE(depthShader);
	DELETE(shadowShader);
	DELETE(simpleShader);
	if (fb.fb) {
		glDeleteFramebuffers(1, &fb.fb);
	}
	if (fb.rb) {
		glDeleteRenderbuffers(1, &fb.rb);
	}
	if (fb.t) {
		glDeleteTextures(1 , &fb.t);
	}
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
	figures.push_back(set);
	set->retain();
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
	if (flag) {
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
	depthShader->setDepthBuffer(depthBufferFlag);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fb.fb);
	
	// フレームバッファを初期化
	glViewport(0, 0, fbWidth, fbHeight);
	glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepthf(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	gc3dcontext.type = DepthStorageShaderType;
	
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
	shadowShader->useProgram();
	shadowShader->setDepthBuffer(depthBufferFlag);
	
	gc3dcontext.type = ShadowShaderType;
	
	shadowShader->bindShadowTexture(fb.t);
	shadowShader->setLightPosition(&light);
	shadowShader->setTextureMatrix(&lightcamera);
	
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
