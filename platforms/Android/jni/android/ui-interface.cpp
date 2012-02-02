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

#include <jni.h>
#include "UIController.h"
#include "Figure.h"
#include "Texture.h"
#include "PrimitiveObjectBuilder.h"
#include "ApplicationController.h"
#include "Scene.h"
#include "Layer2D.h"
#include "View.h"
#include "ViewGroup.h"
#include "ImageView.h"
#include "GridView.h"
#include "Log.h"
#include "APIGlue.h"

extern "C" {

/**
 * UIを管理するためのクラス.
 */
static UIController *uiController = NULL;

/**
 * UIの初期化関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_UI_createUI(
		JNIEnv *env, jobject thiz) {
	if (!uiController) {
		uiController = new UIController();
	}
}

JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_UI_disposeUI(
		JNIEnv * env, jobject thiz) {
	DELETE(uiController);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// GCApplicationController
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * GCApplicatoinController#addScene(int, GCScene)に対応する関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param id シーンID
 * @param pear Sceneが格納されているUIContainerへのアドレス
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_app_GCApplicationController_nativeAddScene(
		JNIEnv *env, jobject thiz, jint id, jint pear) {
	ApplicationController *ctr = ApplicationController::getInstance();
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		Scene* scene = (Scene *) container->ui;
		if (scene) {
			ctr->addScene(id, scene);
		}
	}
}

/**
 * GCApplicatoinController#sceneChange(int)に対応する関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param id 遷移先のID
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_app_GCApplicationController_nativeSceneChange(
		JNIEnv *env, jobject thiz, jint id) {
	ApplicationController *ctr = ApplicationController::getInstance();
	ctr->sceneChange(id);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// GCScene
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * GCLayer2Dの初期化,
 * @param env 環境変数
 * @param thiz オブジェクト変数
 */
JNIEXPORT jint JNICALL
Java_com_gclue_gl_scene_GCScene_nativeCreate(
		JNIEnv *env, jobject thiz) {
	if (uiController) {
		ApplicationController *ctr = ApplicationController::getInstance();
		UIContainer *ui = new UIContainer();
		ui->type = UIType_Scene;
		ui->ui = new Scene(ctr);
		uiController->add(ui);
		return (int) ui;
	}
	return NULL;
}

/**
 * GCScene#addLayer(int, GCLayer)に対応する関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear GCSceneを格納するUIContainerへのアドレス
 * @param pearLayer GCLayerを格納するUIContainerへのアドレス
 * @param id 判別用ID
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_scene_GCScene_nativeAddLayer(
		JNIEnv *env, jobject thiz, jint pear, jint pearLayer, jint id) {
	UIContainer *container1 = (UIContainer *) pear;
	UIContainer *container2 = (UIContainer *) pearLayer;
	if (container1 && container2) {
		Scene* scene = (Scene *) container1->ui;
		Layer* layer = (Layer *) container2->ui;
		if (layer && scene) {
			scene->addLayer(id, layer);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// GCLayer2D
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * GCLayer2Dの初期化,
 * @param env 環境変数
 * @param thiz オブジェクト変数
 */
JNIEXPORT jint JNICALL
Java_com_gclue_gl_scene_GCLayer2D_nativeCreate(
		JNIEnv *env, jobject thiz) {
	if (uiController) {
		ApplicationController *ctr = ApplicationController::getInstance();
		UIContainer *ui = new UIContainer();
		ui->type = UIType_Layer2D;
		ui->ui = new Layer2D(ctr);
		uiController->add(ui);
		return (int) ui;
	}
	return NULL;
}

/**
 * GCLayer2D#setContentView(GCView)に対応する関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear Layer2Dを格納するUIContainerへのアドレス
 * @param pearView Viewを格納するUIContainerへのアドレス
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_scene_GCLayer2D_nativeSetContentView(
		JNIEnv *env, jobject thiz, int pear, int pearView) {
	UIContainer *container1 = (UIContainer *) pear;
	UIContainer *container2 = (UIContainer *) pearView;
	if (container1 && container2) {
		Layer2D* layer = (Layer2D *) container1->ui;
		View* view = (View *) container2->ui;
		if (layer && view) {
			layer->setContentView(view);
		}
	}
}

/**
 * GCLayer2D#openDialog(GCView)に対応する関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear Layer2Dを格納するUIContainerへのアドレス
 * @param pearView Viewを格納するUIContainerへのアドレス
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_scene_GCLayer2D_nativeOpenDialog(
		JNIEnv *env, jobject thiz, int pear, int pearView) {
	UIContainer *container1 = (UIContainer *) pear;
	UIContainer *container2 = (UIContainer *) pearView;
	if (container1 && container2) {
		Layer2D* layer = (Layer2D *) container1->ui;
		View* view = (View *) container2->ui;
		if (layer && view) {
			layer->openDialog(view);
		}
	}
}

/**
 * GCLayer2D#openDialog(GCView)に対応する関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear Layer2Dを格納するUIContainerへのアドレス
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_scene_GCLayer2D_nativeCloseDialog(
		JNIEnv *env, jobject thiz, int pear) {
	UIContainer *container1 = (UIContainer *) pear;
	if (container1) {
		Layer2D* layer = (Layer2D *) container1->ui;
		if (layer) {
			layer->closeDialog();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// GCTexture
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
/// GCSharedTexture
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
/// GCView
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * GCView#setPosition(float, float)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param x x座標
 * @param y y座標
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeSetPosition(
		JNIEnv * env, jobject thiz, jint pear, jfloat x, jfloat y) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			view->setPosition(x, y);
		}
	}
}

/**
 * GCView#setScale(float)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param scale スケール
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeSetScale(
		JNIEnv * env, jobject thiz, jint pear, jfloat xscale, jfloat yscale) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			view->setScale(xscale, yscale);
		}
	}
}

/**
 * GCView#setRotate(float)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param rotate 角度
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeSetRotate(
		JNIEnv * env, jobject thiz, jint pear, jfloat rotate) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			view->setRotate(rotate);
		}
	}
}

/**
 * GCView#setAlpha(float)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param alpha 角度
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeSetAlpha(
		JNIEnv * env, jobject thiz, jint pear, jfloat alpha) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			view->setAlpha(alpha);
		}
	}
}

/**
 * GCView#setSize(float, float)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param w 横幅
 * @param h 縦幅
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeSetSize(
		JNIEnv * env, jobject thiz, jint pear, jfloat w, jfloat h) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			view->setSize(w, h);
		}
	}
}

/**
 * GCView#setClickable(boolean)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param clickable クリック判定フラグ
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeSetClickable(
		JNIEnv * env, jobject thiz, jint pear, jboolean clickable) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			view->setClickable(clickable);
		}
	}
}

/**
 * GCView#setVisible(boolean)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param visible クリック判定フラグ
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeSetVisible(
		JNIEnv * env, jobject thiz, jint pear, jboolean visible) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			view->setVisible(visible);
		}
	}
}

/**
 * GCView#getPosition()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 */
JNIEXPORT jfloat JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetX(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			return view->point.x;
		}
	}
	return 0.0;
}

/**
 * GCView#getPosition()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 */
JNIEXPORT jfloat JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetY(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			return view->point.y;
		}
	}
	return 0.0;
}

/**
 * GCView#getWidth()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @return 横幅
 */
JNIEXPORT jfloat JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetWidth(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			return view->size.x;
		}
	}
	return 0.0;
}

/**
 * GCView#getHeight()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @return 縦幅
 */
JNIEXPORT jfloat JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetHeight(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			return view->size.y;
		}
	}
	return 0.0;
}

/**
 * GCView#getScale()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @return スケール
 */
JNIEXPORT jfloat JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetScaleX(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			return view->scale.x;
		}
	}
	return 0.0;
}

/**
 * GCView#getScale()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @return スケール
 */
JNIEXPORT jfloat JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetScaleY(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			return view->scale.y;
		}
	}
	return 0.0;
}

/**
 * GCView#getRotate()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @return 角度
 */
JNIEXPORT jfloat JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetRotate(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			return view->rotate;
		}
	}
	return 0.0;
}

/**
 * GCView#getAlpha()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @return アルファ値
 */
JNIEXPORT jfloat JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetAlpha(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			return view->alpha;
		}
	}
	return 0.0;
}

/**
 * GCView#getClickable()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @return クリック判定
 */
JNIEXPORT jboolean JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetClickable(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			if (view->clickable) {
				return JNI_TRUE;
			} else {
				return JNI_FALSE;
			}
		}
	}
	return JNI_FALSE;
}

/**
 * GCView#getVisible()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @return クリック判定
 */
JNIEXPORT jboolean JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetVisible(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			if (view->visible) {
				return JNI_TRUE;
			} else {
				return JNI_FALSE;
			}
		}
	}
	return JNI_FALSE;
}

/**
 * GCView#startAnimation(GCAnimation)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param pearAnimation アニメーションへのアドレス
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeStartAnimation(
		JNIEnv * env, jobject thiz, jint pear, jint pearAnimation) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
		}
	}
}

/**
 * GCView#stopAnimation()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeStopAnimation(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			view->stopAnimation();
		}
	}
}

/**
 * GCView#setUserID(int)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param id ユーザID
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCView_nativeSetUserID(
		JNIEnv * env, jobject thiz, jint pear, jint id) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			view->setUserID(id);
		}
	}
}
/**
 * GCView#stopAnimation()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @return ユーザID
 */
JNIEXPORT jint JNICALL
Java_com_gclue_gl_ui_GCView_nativeGetUserID(
		JNIEnv * env, jobject thiz, jint pear) {
	UIContainer *container = (UIContainer *) pear;
	if (container) {
		View* view = (View *) container->ui;
		if (view) {
			return view->getUserID();
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// GCViewGroup
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * GCViewGroup#GCViewGroup()に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @return UIContainerへのアドレス
 */
JNIEXPORT jint JNICALL
Java_com_gclue_gl_ui_GCViewGroup_nativeCreate(
		JNIEnv * env, jobject thiz) {
	if (uiController) {
//		ApplicationController *ctr = ApplicationController::getInstance();
//		UIContainer *ui = new UIContainer();
//		ui->type = UIType_ViewGroup;
//		ui->ui = new ViewGroup(ctr);
//		uiController->add(ui);
//		return (int) ui;
	}
	return NULL;
}

/**
 * GCViewGroup#addView(GCView)に対応するnative関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContainerへのアドレス
 * @param pearView 追加するUIContainerへのアドレス
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCViewGroup_nativeAddView(
		JNIEnv * env, jobject thiz, jint pear, jint pearView) {
	UIContainer *container1 = (UIContainer *) pear;
	UIContainer *container2 = (UIContainer *) pearView;
	if (container1 && container2) {
//		ViewGroup* vg = (ViewGroup *) container1->ui;
//		View* view = (View *) container2->ui;
//		if (vg && view) {
//			vg->addView(view);
//		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// GCImageView
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * GCImageView#GCImageView()の初期化関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @return ポインタ
 */
JNIEXPORT jint JNICALL
Java_com_gclue_gl_ui_GCImageView_nativeCreate(
		JNIEnv *env, jobject thiz) {
	if (uiController) {
//		ApplicationController *ctr = ApplicationController::getInstance();
//		UIContainer *ui = new UIContainer();
//		ui->type = UIType_ImageView;
//		ui->ui = new ImageView(ctr);
//		uiController->add(ui);
//		return (int) ui;
	}
	return NULL;
}

/**
 * GCImageView#setTexture(GCTexture)に対応する関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContanierへのアドレス
 * @param pearTexture Textureが格納されたUIContainerへのアドレス
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCImageView_nativeSetTexture(
		JNIEnv *env, jobject thiz, jint pear, jint pearTexture) {
}

/**
 * GCImageView#setSharedTexture(GCTexture, String)に対応する関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @param pear UIContanierへのアドレス
 * @param pearTexture SharedTextureが格納されたUIContainerへのアドレス
 * @param fname 名前
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_ui_GCImageView_nativeSetSharedTexture(
		JNIEnv *env, jobject thiz, jint pear, jint pearTexture, jstring fname) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// GridView
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * GCGridView#GCGridView()の初期化関数.
 * @param env 環境変数
 * @param thiz オブジェクト変数
 * @return ポインタ
 */
JNIEXPORT jint JNICALL
Java_com_gclue_gl_ui_GCGridView_nativeCreate(
		JNIEnv *env, jobject thiz, jint row, jint column) {
	if (uiController) {
//		ApplicationController *ctr = ApplicationController::getInstance();
//		UIContainer *ui = new UIContainer();
//		ui->type = UIType_GridView;
//		ui->ui = new GridView(ctr, row, column);
//		uiController->add(ui);
//		return (int) ui;
	}
	return NULL;
}


}	// end of extern "C"
