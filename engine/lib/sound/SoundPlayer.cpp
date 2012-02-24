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
	if (!instance) {
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
	}
}

SoundPlayer::~SoundPlayer() {
	if (context) {
		alcMakeContextCurrent( NULL);
		alcDestroyContext(context);
	}
	context = NULL;

	if (device) {
		alcCloseDevice(device);
	}
	device = NULL;
}

// 読み込み
int SoundPlayer::loadSound(SoundData *data) {
	// バッファとソース
	ALuint buffer, source;
	ALenum error;
	
	// ソース作成
	alGenSources(1, &source);
	if ((error = alGetError()) != AL_NO_ERROR) {
		LOGE("Failed to gen sources. - %d", error);
		return 0;
	}
	
	// バッファ作成
	alGenBuffers(1, &buffer);
	if ((error = alGetError()) != AL_NO_ERROR) {
		LOGE("Failed to gen buffers. - %d", error);
		if (source) alDeleteSources(1, &source);
		return 0;
	}
	
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
		if (source) alDeleteSources(1, &source);
		if (buffer) alDeleteBuffers(1, &buffer);
		return 0;
	}
#endif
	
	// ソースへバッファを関連づける
	alSourcei(source, AL_BUFFER, buffer);
	if ((error = alGetError()) != AL_NO_ERROR) {
		LOGE("Failed to set source. - %d", error);
		if (source) alDeleteSources(1, &source);
		if (buffer) alDeleteBuffers(1, &buffer);
		return 0;
	}

	// source,bufferをキャッシュ
	sounds.insert(std::map<ALuint, ALuint>::value_type(source, buffer));

	// エラーチェック
	if (alGetError() != AL_NO_ERROR) {
		LOGE("*****ERROR(SoundPlayer::loadSound)");
	}
	return source;
}

// 削除
void SoundPlayer::removeSound(int source) {
	if (source) {
		// ソース削除
		alDeleteSources(1, (ALuint*) &source);
		// バッファ削除
		std::map<ALuint, ALuint>::iterator it = sounds.find(source);
		if (it != sounds.end()) {
			ALuint buffer = (*it).second;
			if (buffer) {
				alDeleteBuffers(1, (ALuint*) &buffer);
			}
			// ハッシュから削除
			sounds.erase(it);
		}
	}
}

// リセット
void SoundPlayer::resetAllSounds() {
	std::map<ALuint, ALuint>::iterator it = sounds.begin();
	while (it != sounds.end()) {
		ALuint source = (*it).first;
		ALuint buffer = (*it).second;
		if (source) alDeleteSources(1, (ALuint*) &source);
		if (buffer) alDeleteBuffers(1, (ALuint*) &buffer);
		it++;
	}
	sounds.clear();
}

// 再生
void SoundPlayer::play(int source, bool loop) {
	if (source == 0) return;
	alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	alSourcePlay(source);
}

// 停止
void SoundPlayer::stop(int source) {
	if (source == 0) return;
	alSourceStop(source);
}

// 音量
void SoundPlayer::volume(int source, float volume) {
	if (source == 0) return;
	if (volume < 0) volume = 0;
	if (volume > 1) volume = 1;
	alSourcef(source, AL_GAIN, volume);
}

// 再生チェック
bool SoundPlayer::isPlaying(int source) {
	ALint state;
	if (source == 0) return false;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

// SoundPlayerを破棄します.
void SoundPlayer::dispose() {
	delete instance;
	instance = NULL;
}
