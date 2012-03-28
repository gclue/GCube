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
 * ndk-interface.cpp
 *
 * JavaとC++のインターフェースをするクラス
 *
 *  Created on: 2011/04/25
 *      Author: haga
 */
#include "APIGlue.h"
//#include "lib/porting/APIGlue.h"

#include "ApplicationController.h"
#include "Figure.h"
#include "Texture.h"
#include "PackerTexture.h"
#include "HttpClient.h"
#include "defines.h"
#include "main.h"

#include <jni.h>
#include <string>
#include <map>
#include <vector>

/**
 * JNIを操作する為のデータを保持する構造体.
 */
struct JNIInterface {
	JNIEnv *env;					//!< Java環境へのポインタ
	jobject obj;					//!< NDKInterfae.javaのインスタンス

	jmethodID loadFigureMethod;		//!< NDKInterfae#loadFigure()のID
	jmethodID loadImageMethod;		//!< NDKInterfae#loadImage()のID
	jmethodID loadAssetMethod;		//!< NDKInterfae#loadAsset()のID
	jmethodID soundEventMethod;		//!< NDKInterfae#soundEvent()のID
	jmethodID onGameEventMethod;	//!< NDKInterfae#onGameEvent()のID
	jmethodID drawTextMethod;		//!< NDKInterfae#drawTextMethod()のID
	jmethodID getTextureMethod;		//!< NDKInterfae#getTextureMethod()のID
	jmethodID requestHttpMethod;	//!< NDKInterface#requestHttp()のID
	jmethodID requestHttpAsyncMethod;	//!< NDKInterface#requestHttp()のID
	jmethodID sendWebViewEventMethod;	//!< NDKInterface#onWebViewEvent() id
};

/**
 * シーンを管理するクラス.
 */
ApplicationController *controller = NULL;

/**
 * HttpRequestの要求を保持するマップ.
 */
static std::map<int, IHttpRquestListener*> httpRequestMap;

/**
 * HttpRequestの要求のIDを作成するためのカウンター.
 * 要求がある度にインクリメントしてIDがかぶらないようにする。
 */
static int httpRequestId = 0;

/**
 * JNIインターフェイス.
 */
static JNIInterface jni = {0};

extern "C" {

/**
 * Javaから取得したImageDataをTextureに変換します.
 * @param image Javaから取得したImageData
 * @return Texture
 */
static bool createTexture(Texture *texture, jobject image) {
	JNIEnv* env = jni.env;

	// javaメインクラスの取得
	jclass classImgDat = env->FindClass("com/gclue/gl/ImageData");

	jmethodID getPixelsID = env->GetMethodID(classImgDat, "getPixels", "()[I");
	if (getPixelsID == 0) {
		LOGE( "**ERROR(JNIGetTextTexture)**:getPixels id is Zero");
		return false;
	}
	// 画像の縦サイズを取得するメソッドのＩＤを取得
	jmethodID getHeightID = env->GetMethodID(classImgDat, "getHeight", "()I");
	if (getHeightID == 0) {
		LOGE( "**ERROR(JNIGetTextTexture)**:getHeight id is Zero");
		return false;
	}
	// 画像の横サイズを取得するメソッドのＩＤを取得
	jmethodID getWidthID = env->GetMethodID(classImgDat, "getWidth", "()I");
	if (getWidthID == 0) {
		LOGE( "**ERROR(JNIGetTextTexture)**:getWidth id is Zero");
		return false;
	}

	jintArray ja = (jintArray) env->CallObjectMethod(image, getPixelsID);
	int jasize = env->GetArrayLength(ja);
	int width = env->CallIntMethod(image, getWidthID);
	int height = env->CallIntMethod(image, getHeightID);

	jint *arr1 = env->GetIntArrayElements(ja, 0);
	int cnt;
	unsigned char *imageData = (unsigned char *) malloc(width * height * 4);
	for (cnt = 0; cnt < width * height; cnt++) {
		imageData[4*cnt + 0] = (arr1[cnt] & 0x00FF0000) >> 16;	// R
		imageData[4*cnt + 1] = (arr1[cnt] & 0x0000FF00) >> 8;	// G
		imageData[4*cnt + 2] = (arr1[cnt] & 0x000000FF);		// B
		imageData[4*cnt + 3] = (arr1[cnt] & 0xFF000000) >> 24;	// A
	}

	// テクスチャ作成
	texture->setImageData(imageData, width, height);

	// メモリを開放
	free(imageData);
	env->DeleteLocalRef(image);
	env->DeleteLocalRef(classImgDat);
	env->ReleaseIntArrayElements(ja, arr1, 0);

	return true;
}

/**
 * Java側のtexture/SpriteのデータをTexDataに変換します.
 * @param data Javaから取得したtexture/Sprite
 * @return 変換されたTexData
 */
static TexData createTexData(jobject data) {
	JNIEnv* env = jni.env;
	jclass clazz = env->FindClass("com/gclue/gl/texture/Sprite");
	if (clazz) {
		jmethodID getLeftID = env->GetMethodID(clazz, "getLeft", "()I");
		if (getLeftID == 0) {
			LOGE( "getLeftID is Zero");
		}

		jmethodID getRightID = env->GetMethodID(clazz, "getRight", "()I");
		if (getRightID == 0) {
			LOGE( "getRightID is Zero");
		}

		jmethodID getTopID = env->GetMethodID(clazz, "getTop", "()I");
		if (getTopID == 0) {
			LOGE( "getTopID is Zero");
		}

		jmethodID getBottomID = env->GetMethodID(clazz, "getBottom", "()I");
		if (getBottomID == 0) {
			LOGE( "getBottomID is Zero");
		}

		jmethodID getRotateID = env->GetMethodID(clazz, "getRotate", "()I");
		if (getRotateID == 0) {
			LOGE( "getRotateID is Zero");
		}

		int left = env->CallIntMethod(data, getLeftID);
		int top = env->CallIntMethod(data, getTopID);
		int right = env->CallIntMethod(data, getRightID);
		int bottom = env->CallIntMethod(data, getBottomID);
		int rotate = env->CallIntMethod(data, getBottomID);

		env->DeleteLocalRef(clazz);

		TexData tex;
		tex.rect.left = left;
		tex.rect.top = top;
		tex.rect.right = right;
		tex.rect.bottom = bottom;
		tex.rotate = rotate;

		LOGI("(%d, %d, %d, %d)", left, top, right, bottom);

		return tex;
	}
	TexData t;
	return t;
}

static HttpResponse* createHttpResponse(jobject res) {
	JNIEnv* env = jni.env;
	if (env) {
		jclass clazz = env->FindClass("com/gclue/gl/HttpResponse");
		if (clazz) {
			jmethodID getStatusCodeID = env->GetMethodID(clazz, "getStatusCode", "()I");
			if (getStatusCodeID == 0) {
				LOGE( "getStatusCodeID is Zero");
			}
			jmethodID getLengthID = env->GetMethodID(clazz, "getLength", "()I");
			if (getLengthID == 0) {
				LOGE( "getLengthID is Zero");
			}
			jmethodID getBodyID = env->GetMethodID(clazz, "getBody", "()[B");
			if (getBodyID == 0) {
				LOGE( "getBodyID is Zero");
			}

			int statusCode = env->CallIntMethod(res, getStatusCodeID);
			int length = env->CallIntMethod(res, getLengthID);
			if (length > 0) {

			}

			HttpResponse *response = new HttpResponse();
			response->setStatusCode(statusCode);
			if (length > 0) {
				jbyteArray ja = (jbyteArray) env->CallObjectMethod(res, getBodyID);
				int jasize = env->GetArrayLength(ja);
				jbyte *arr1 = env->GetByteArrayElements(ja, 0);
				response->setData((const char *) arr1, jasize);
				env->ReleaseByteArrayElements(ja, arr1, 0);
			}

			env->DeleteLocalRef(clazz);

			return response;
		}
	}
	return NULL;
}


/**
 * 文字列を描画したテクスチャを取得します.
 * もし文字列の描画が行われていなかった場合にはNULLを返却します.
 * @return Textureクラス
 */
PackerTexture* JNIGetTextTexture() {
	JNIEnv* env = jni.env;
	if (env) {
		jobject texture = env->CallObjectMethod(jni.obj, jni.getTextureMethod);
		if (!texture) {
			LOGE( "**ERROR(JNIGetTextTexture)**");
			return NULL;
		}

		jclass clazz = env->FindClass("com/gclue/gl/texture/Texture");

		jmethodID getCountID = env->GetMethodID(clazz, "getCount", "()I");
		if (!getCountID) {
			LOGE( "**ERROR(getCountID)**");
			return NULL;
		}

		jmethodID getSpriteID = env->GetMethodID(clazz, "getSprite", "(I)Lcom/gclue/gl/texture/Sprite;");
		if (!getSpriteID) {
			LOGE( "**ERROR(getSpriteID)**");
			return NULL;
		}

		jmethodID getImageDataID = env->GetMethodID(clazz, "getImageData", "()Lcom/gclue/gl/ImageData;");
		if (!getImageDataID) {
			LOGE( "**ERROR(getImageDataID)**");
			return NULL;
		}

		PackerTexture *packerTexture = new PackerTexture();

		// 文字列の個数を取得
		int count = env->CallIntMethod(texture, getCountID);
		for (int i = 0; i < count; i++) {
			jobject data = env->CallObjectMethod(texture, getSpriteID, i);
			packerTexture->addTexData(createTexData(data));
			env->DeleteLocalRef(data);
		}

		// テクスチャデータの取得
		// この関数を呼び出すとJavaでBitmapなどを破棄してしまうので
		// この関数は最後に呼び出すこと。
		jobject image = env->CallObjectMethod(texture, getImageDataID);
		if (!image) {
			LOGE( "**ERROR(image is NULL.)**");
			return NULL;
		}

		Texture* tex = new Texture();
		createTexture(tex, image);
		packerTexture->setTexture(tex);

		// 後始末
		env->DeleteLocalRef(texture);
		env->DeleteLocalRef(image);
		env->DeleteLocalRef(clazz);

		return packerTexture;
	}
	return NULL;
}

/**
 * 指定された文字列をテクスチャに書き込みます.
 * @param[in] text 文字列
 * @param[in] fontSize フォントサイズ
 * @param[in] r 赤色成分
 * @param[in] g 緑色成分
 * @param[in] b 青色成分
 * @return テクスチャデータ
 */
void JNIDrawText(const char *text, float fontSize, float r, float g, float b) {
	JNIEnv* env = jni.env;
	if (env) {
		jstring str = env->NewStringUTF(text);
		jint red = (jint) 255 * r;
		jint green = (jint) 255 * g;
		jint blue = (jint) 255 * b;

		jobject data = env->CallObjectMethod(jni.obj, jni.drawTextMethod, str, fontSize, red, green, blue);
		env->DeleteLocalRef(str);
		if (data) {
			env->DeleteLocalRef(data);
		}
	}
}

/**
 * SE,BGM再生
 * mode: 0:BGM再生
 *       1:BGM停止
 *       2:SE登録
 *       3:SE再生
 * @param[in] fname ファイル名
 * @param[in] mode モード
 */
void GCSoundEvent(const char *fileName, int mode) {
	JNIEnv* env = jni.env;
	if (env) {
		jstring str = env->NewStringUTF(fileName);
		env->CallVoidMethod(jni.obj, jni.soundEventMethod, mode, str);
		if (str) {
			env->DeleteLocalRef(str);
		}
	}
}

/**
 * Java側にイベントを配送します.
 * @param[in] type イベントタイプ
 * @param[in] param1 イベントパラメータ
 * @param[in] param2 イベントパラメータ
 * @param[in] param3 イベントパラメータ
 * @param[in] param4 イベントパラメータ
 * @param[in] param5 イベントパラメータ
 */
void GCSendGameEvent(int type, int param1, long param2, double param3, int param4, const char *param5) {
	JNIEnv* env = jni.env;
	if (env) {
		LOGD( "**JNISendGameEvent**:%d, %d, %d, %d, %d, %s", type, param1, param2, param3, param4, param5);
		jstring str = NULL;
		if (param4) {
			str = env->NewStringUTF(param5);
		}
		env->CallVoidMethod(jni.obj, jni.onGameEventMethod, type, param1, param2, param3, param4, str);
		if (str) {
			env->DeleteLocalRef(str);
		}
	}
}

/**
 * Figureを読み込みます.
 * @param[in] name フィギュア名前
 */
Figure* GCLoadFigure(const char *fname) {
	JNIEnv* env = jni.env;
	if (env) {
		jstring str = env->NewStringUTF(fname);
		jobject figure = env->CallObjectMethod(jni.obj, jni.loadFigureMethod, str);
		env->DeleteLocalRef(str);
		if (!figure) {
			LOGE( "**ERROR(JNILoadFigure)**");
			return NULL;
		}

		jclass clazz = env->FindClass("com/gclue/gl/Figure");

		jmethodID getVerticesID = env->GetMethodID(clazz, "getVertices", "()[F");
		if (getVerticesID == 0) {
			LOGI( "getVerticesID is Zero");
			return NULL;
		}

		jmethodID getNormalID = env->GetMethodID(clazz, "getNormal", "()[F");
		if (getNormalID == 0) {
			LOGI( "getNormalID is Zero");
			return NULL;
		}

		jmethodID getTextureCoordsID = env->GetMethodID(clazz, "getTextureCoords", "()[F");
		if (getTextureCoordsID == 0) {
			LOGI( "getTextureCoordsID is Zero");
			return NULL;
		}

		jmethodID getVertexIndexesID = env->GetMethodID(clazz, "getVertexIndexes", "()[S");
		if (getVertexIndexesID == 0) {
			LOGI( "getVertexIndexesID is Zero");
			return NULL;
		}

		// 頂点データ
		jfloatArray vertics = (jfloatArray) env->CallObjectMethod(figure, getVerticesID);
		int verticsSize = env->GetArrayLength(vertics);
		jfloat *v = env->GetFloatArrayElements(vertics, 0);

		// 法線データ
		jfloatArray normals = (jfloatArray) env->CallObjectMethod(figure, getNormalID);
		int normalsSize = env->GetArrayLength(normals);
		jfloat *n = env->GetFloatArrayElements(normals, 0);

		// テクスチャデータ
		jfloatArray textureCoords = (jfloatArray) env->CallObjectMethod(figure, getTextureCoordsID);
		int textureCoordsSize = env->GetArrayLength(textureCoords);
		jfloat *t = env->GetFloatArrayElements(textureCoords, 0);

		// 頂点インデックスデータ
		jshortArray vertexIndexes = (jshortArray) env->CallObjectMethod(figure, getVertexIndexesID);
		int vertexIndexesSize = env->GetArrayLength(vertexIndexes);
		jshort *i = env->GetShortArrayElements(vertexIndexes, 0);

		// Figureクラス作成
		Figure *f = new Figure();
		f->addVertices(v, verticsSize);
		f->addNormal(n, normalsSize);
		f->addTextureCoords(t, textureCoordsSize);
		f->addVertexIndexes((const GLushort *) i, vertexIndexesSize);
		f->build();

		// メモリを開放
		env->DeleteLocalRef(figure);
		env->DeleteLocalRef(clazz);
		env->ReleaseFloatArrayElements(vertics, v, 0);
		env->ReleaseFloatArrayElements(normals, n, 0);
		env->ReleaseFloatArrayElements(textureCoords, t, 0);
		env->ReleaseShortArrayElements(vertexIndexes, i, 0);

		return f;
	}
	return NULL;
}

/**
 * assetsからテクスチャを読み込みます.
 * @param[in] fileName ファイル名
 * @return テクスチャオブジェクト
 */
bool GCLoadTexture(Texture *texture, const char *fileName) {
	JNIEnv* env = jni.env;
	if (env) {
		jstring str = env->NewStringUTF(fileName);
		jobject image = env->CallObjectMethod(jni.obj, jni.loadImageMethod, str);
		env->DeleteLocalRef(str);
		if (!image) {
			LOGE( "**ERROR(JNILoadTexture)**");
			return false;
		}
		return createTexture(texture, image);
	}
	return false;
}

/**
 * Assetの中を読み込みます.
 * @param[in] fileName ファイル名
 * @return charのvectorオブジェクト
 */
std::vector<char>* GCLoadAsset(const char *fileName) {
	JNIEnv* env = jni.env;
	if (env) {
		jstring str = env->NewStringUTF(fileName);
		jbyteArray data = (jbyteArray)env->CallObjectMethod(jni.obj, jni.loadAssetMethod, str);
		env->DeleteLocalRef(str);
		if (!data) {
			return NULL;
		}

		int datasize = env->GetArrayLength(data);
		jbyte *arr = env->GetByteArrayElements(data, 0);

		std::vector<char> *buff = new std::vector<char>();
		for (int i = 0; i < datasize; ++i) {
			buff->push_back(arr[i]);
		}

		// メモリを開放
		env->ReleaseByteArrayElements(data, arr, 0);
		env->DeleteLocalRef(data);

		return buff;
	}
	return NULL;
}

/**
 * 保存領域へのパスを取得します.
 * @return パス
 */
const char* GCGetStoragePath(const char* fileName) {
	return "";
}

int GCHttpRequestAsync(std::string url, std::map<std::string, std::string> headers, std::string body,  IHttpRquestListener *callback) {
	JNIEnv* env = jni.env;
	if (env) {
		jstring urlString = env->NewStringUTF(url.c_str());
		jstring postString = env->NewStringUTF(body.c_str());

		int i = 0;
		int size = headers.size();
		jclass clazz = env->FindClass("java/lang/String");
		jobjectArray arrj = (jobjectArray)env->NewObjectArray(size * 2, clazz, NULL);
		std::map<std::string, std::string>::iterator itr;
		for (itr = headers.begin(); itr != headers.end(); itr++) {
			jstring key = env->NewStringUTF(itr->first.c_str());
			jstring value = env->NewStringUTF(itr->second.c_str());
			env->SetObjectArrayElement(arrj, i++, key);
			env->SetObjectArrayElement(arrj, i++, value);
		}

		jint response = env->CallIntMethod(jni.obj, jni.requestHttpAsyncMethod, urlString, arrj, postString, ++httpRequestId);
		env->DeleteLocalRef(urlString);
		env->DeleteLocalRef(postString);
		env->DeleteLocalRef(arrj);

		if (response != 0) {
			// コールバック登録
			httpRequestMap[httpRequestId] = callback;
			// 登録したIDを返却
			return httpRequestId;
		}
		return response;
	}
	return 0;
}

HttpResponse* GCHttpRequest(std::string url, std::map<std::string, std::string> headers, std::string body) {
	JNIEnv* env = jni.env;
	if (env) {
		jstring urlString = env->NewStringUTF(url.c_str());
		jstring postString = env->NewStringUTF(body.c_str());

		int i = 0;
		int size = headers.size();
		jclass clazz = env->FindClass("java/lang/String");
		jobjectArray arrj = (jobjectArray)env->NewObjectArray(size * 2, clazz, NULL);
		std::map<std::string, std::string>::iterator itr;
		for (itr = headers.begin(); itr != headers.end(); itr++) {
			jstring key = env->NewStringUTF(itr->first.c_str());
			jstring value = env->NewStringUTF(itr->second.c_str());
			env->SetObjectArrayElement(arrj, i++, key);
			env->SetObjectArrayElement(arrj, i++, value);
		}

		// Javaの関数を呼び出す
		jobject response = env->CallObjectMethod(jni.obj, jni.requestHttpMethod, urlString, arrj, postString, httpRequestId);
		env->DeleteLocalRef(urlString);
		env->DeleteLocalRef(postString);
		env->DeleteLocalRef(arrj);

		HttpResponse *resp = createHttpResponse(response);
		env->DeleteLocalRef(response);
		return resp;
	}
	return NULL;
}

/**
 * WebViewイベントをJavaに送ります.
 * @param[in] type イベントタイプ.
 * @param[in] viewId ビューのID.
 * @param[in] param1 パラメータ(x座標)
 * @param[in] param2 パラメータ(y座標)
 * @param[in] param3 パラメータ(width)
 * @param[in] param4 パラメータ(height)
 * @param[in] param5 文字列パラメータ(URL)
 */
void GCSendWebViewEvent(int type, int viewId, double param1, double param2, double param3, double param4, const char *param5) {
	JNIEnv* env = jni.env;
		if (env) {
			LOGD( "**JNIsendWebViewEvent**:%d, %d, %f, %f, %f, %f, %s", type,viewId, param1, param2, param3, param4, param5);
			jstring str = NULL;
			if (param5) {
				str = env->NewStringUTF(param5);
			}
			env->CallVoidMethod(jni.obj, jni.sendWebViewEventMethod, type, viewId, param1, param2, param3, param4, str);
			if (str) {
				env->DeleteLocalRef(str);
			}
		}
}

/**
 * 初期化関数.
 *
 * @param env 環境変数
 * @param obj オブジェクト変数
 * @param width 画面横サイズ
 * @param height 画面縦サイズ
 */
JNIEXPORT jboolean JNICALL
Java_com_gclue_gl_JNILib_init(
		JNIEnv * env, jobject obj, jint width, jint height) {
	LOGD("Java_com_gclue_gl_JNILib_init");

//	DELETE(controller);
//	controller = ApplicationController::getInstance();
//	controller->resize(width, height);
//	controller->resetup();
//	GCInitApplicationController(controller);
//	return true;

	// メインクラスの作成します.
	if (!controller) {
		controller = ApplicationController::getInstance();
		controller->resize(width, height);
		controller->resetup();
		controller->main->initApplicationController();
		return JNI_TRUE;
	} else {
		controller->resetup();
		return JNI_TRUE;
	}
}

/**
 * CからJavaのメソッドを実行するためのインターフェースをセットします.
 *
 * @param env 環境変数.
 * @param thiz thisポインタ
 * @param ndk インターフェースクラスのオブジェクト.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_JNILib_setInterface(
		JNIEnv *env, jobject thiz, jobject ndk) {
	LOGD("Java_com_gclue_gl_JNILib_setInterface");

	// インターフェースクラスのロード.
	jclass clazz = env->FindClass("com/gclue/gl/app/NDKInterface");

	// クラスに環境変数を格納します.
	jni.env = env;
	jni.obj = env->NewGlobalRef(ndk);

	// NDKInterfaceクラスのメソッドを登録します.
	jni.loadFigureMethod = env->GetMethodID(clazz, "loadFigure", "(Ljava/lang/String;)Lcom/gclue/gl/Figure;");
	if (!jni.loadFigureMethod) {
		LOGE("jni.loadFigureMethod not found.");
	}
	jni.loadImageMethod = env->GetMethodID(clazz, "loadImage", "(Ljava/lang/String;)Lcom/gclue/gl/ImageData;");
	if (!jni.loadImageMethod) {
		LOGE("jni.loadImageMethod not found.");
	}
	jni.loadAssetMethod = env->GetMethodID(clazz, "loadAsset", "(Ljava/lang/String;)[B");
	if (!jni.loadAssetMethod) {
		LOGE("jni.loadAssetMethod not found.");
	}
	jni.soundEventMethod = env->GetMethodID(clazz, "soundEvent", "(ILjava/lang/String;)V");
	if (!jni.soundEventMethod) {
		LOGE("Mehtod not found!! (soundEvent)");
	}
	jni.onGameEventMethod = env->GetMethodID(clazz, "onGameEvent", "(IIIIILjava/lang/String;)V");
	if (!jni.onGameEventMethod) {
		LOGE("Mehtod not found!! (onGameEventMethod)");
	}
	jni.drawTextMethod = env->GetMethodID(clazz, "drawText", "(Ljava/lang/String;FIII)Lcom/gclue/gl/texture/Sprite;");
	if (!jni.drawTextMethod) {
		LOGE("Mehtod not found!! (drawTextMethod)");
	}
	jni.getTextureMethod = env->GetMethodID(clazz, "getTexture", "()Lcom/gclue/gl/texture/Texture;");
	if (!jni.getTextureMethod) {
		LOGE("Mehtod not found!! (getTextureMethod)");
	}
	jni.requestHttpAsyncMethod = env->GetMethodID(clazz, "requestHttpAsync", "(Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;I)I");
	if (!jni.requestHttpAsyncMethod) {
		LOGE("Mehtod not found!! (requestHttpAsyncMethod)");
	}
	jni.requestHttpMethod = env->GetMethodID(clazz, "requestHttp", "(Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Lcom/gclue/gl/HttpResponse;");
	if (!jni.requestHttpMethod) {
		LOGE("Mehtod not found!! (requestHttpMethod)");
	}
	jni.sendWebViewEventMethod = env->GetMethodID(clazz, "onWebViewEvent", "(IIDDDDLjava/lang/String;)V");
	if (!jni.sendWebViewEventMethod) {
		LOGE("Mehtod not found!! (onGameEventMethod)");
	}

	env->DeleteLocalRef(clazz);

}

/**
 * C側をシャットダウンします.
 * <br><br>
 * JavaのメインアクティビティのonDestroyが呼ばれるタイミングでこのshutDownが呼ばれます.
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_JNILib_shutDown(
		JNIEnv * env, jobject obj) {
	// superPaintインスタンスをdeleteします.
	DELETE(controller);
	exit(0);
}

/**
 * 一時停止処理を行います.
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] thiz JavaのThisポインタ
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_JNILib_onPause(
		JNIEnv* env, jobject thiz)
{
	if (controller) {
		controller->onPause();
	}
}

/**
 * 再開処理を行います.
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] thiz JavaのThisポインタ
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_JNILib_onResume(
		JNIEnv* env, jobject thiz)
{
	if (controller) {
		controller->onResume();
	}
}

/**
 * 描画ステップを進めます.
 * <br><br>
 * 実際に描画をする関数を呼び出します.
 * @param[in] env Java環境変数
 * @param[in] obj JavaのThisポインタ
 * @param[in] dt 差分時間
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_JNILib_step(
		JNIEnv * env, jobject obj, jdouble dt) {
	if (controller) {
		controller->step(dt);
	}
}

/**
 * バックキーを押した時のイベント.
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] thiz JavaのThisポインタ
 */
JNIEXPORT jboolean JNICALL
Java_com_gclue_gl_JNILib_onPressBackKey(
		JNIEnv* env, jobject thiz)
{
	if (controller) {
		return controller->onPressBackKey();
	}
	return false;
}

/**
 * 端末にタッチした時に呼ばれます
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] obj JavaのThisポインタ
 * @param[in] action アクション
 * @param[in] x x座標
 * @param[in] y y座標
 * @param[in] time イベントが発生した時間
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_JNILib_touchEvent(
		JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jlong time) {
	LOGD("Java_com_gclue_gl_JNILib_touchEvent: %d, (%f, %f) %d", action, x, y, time);
	if (controller) {
		controller->onTouch(action, x, y, time);
	}
}

/**
 * Javaからイベントが渡されてきたときの処理を行います.
 * <br><br>
 * @param[in] env Java環境変数
 * @param[in] thiz JavaのThisポインタ
 * @param[in] type イベントタイプ
 * @param[in] param1 イベントパラメータ
 * @param[in] param2 イベントパラメータ
 * @param[in] param3 イベントパラメータ
 * @param[in] param4 イベントパラメータ
 */
JNIEXPORT void JNICALL
Java_com_gclue_gl_JNILib_sendGameEvent(
		JNIEnv* env, jobject thiz, jint type, jint param1, jint param2, jint param3, jint param4, jstring param5)
{
	if (controller) {
		const char* str = env->GetStringUTFChars(param5, NULL);
		controller->onGameEvent(type, param1, param2, param3, param4, str);
		if (str) {
			env->ReleaseStringUTFChars(param5, str);
		}
	}
}

};	// end of extern "C"


