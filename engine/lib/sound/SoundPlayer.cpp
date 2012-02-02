//
//  SoundPlayer.cpp
//

#include "SoundPlayer.h"
#include "Log.h"

#ifndef __ANDROID__
#include <OpenAL/oalStaticBufferExtension.h>
static	alBufferDataStaticProcPtr	proc = NULL;
#endif


// staticメンバ変数の実体
SoundPlayer* SoundPlayer::instance = NULL;

// インスタンスを取得します.
SoundPlayer* SoundPlayer::getInstance() {
	if(!instance){
		instance = new SoundPlayer();
	}
	return instance;
}

// コンストラクタ
SoundPlayer::SoundPlayer() {
	// デバイスオープン
	device = alcOpenDevice(NULL);
	// コンテキスト取得
	context = alcCreateContext(device, NULL);
	// コンテキストをアクティブにする
	alcMakeContextCurrent(context);
	// エラーチェック
	if (alGetError() != AL_NO_ERROR) {
		LOGE("*****ERROR(SoundPlayer::SoundPlayer)**********");
//		exit(1);
	}
}

// 読み込み
int SoundPlayer::loadSound(SoundData *data) {
	//
	SoundPlayer::getInstance();
	
	// バッファとソース
	ALuint buffer, source;
	
	// ソース作成
	alGenSources(1, &source);
	
	// バッファ作成
	alGenBuffers(1, &buffer);
	
	// TODO dataをキャッシュ

	// データをバッファへ格納
#ifdef __ANDROID__
	alBufferData(buffer, data->fileType, data->data, data->dataSize, data->sampleRate);
	// TODO データが2重になる...
#else
	// alBufferDataStatic を使用した方が内部でデータのコピーを作成しないのでメモリに優しい
	if (proc == NULL) {
		proc = (alBufferDataStaticProcPtr) alcGetProcAddress(NULL, (const ALCchar*) "alBufferDataStatic");
	}

	if (proc) {
		proc(buffer, data->fileType, data->data, data->dataSize, data->sampleRate);
	} else {
		LOGE("***ERROR***(SoundPlayer::loadSound:proc)");
		exit(1);
	}
#endif
	
	// ソースへバッファを関連づける
	alSourcei(source, AL_BUFFER, buffer);
	
	// TODO source,bufferをキャッシュ
	
	// エラーチェック
	if (alGetError() != AL_NO_ERROR) {
		LOGE("*****ERROR(SoundPlayer::loadSound)");
//		exit(1);
	}
	
	return source;
}

// 削除
void SoundPlayer::removeSound(int source) {
	// TODO
}

// リセット
void SoundPlayer::resetAllSounds() {
	// TODO
}

// 再生
void SoundPlayer::play(int source) {
	alSourcePlay(source);
}

// 停止
void SoundPlayer::stop(int source) {
	alSourceStop(source);
}

// 音量
void SoundPlayer::volume(int source, float volume) {
	alSourcef(source, AL_GAIN, volume);
}

