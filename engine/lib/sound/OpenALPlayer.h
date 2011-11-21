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
 * OpenALPlayer.h
 *
 *  Created on: 2011/11/10
 *      Author: GClue, Inc.
 */
#ifndef OPENALPLAYER_H_
#define OPENALPLAYER_H_

#ifdef __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include <vector>
#include "ogg/ogg.h"
#include "vorbis/vorbisfile.h"

#define AL_BUFFER_SIZE (8 * 1024)
#define AL_BUFFER_COUNT 4

/**
 * @struct 簡易メモリストリーム.
 */
typedef struct _MemoryStream {
	unsigned char* buffer;	//!< バッファ
	ogg_int64_t size;			//!< バッファサイズ
	ogg_int64_t offset;			//!< オフセット
} MemoryStream;


/**
 * OpenALでサウンドを再生するためのPlayer.
 */
class OpenALPlayer {
private:
	ALCdevice* device;		//!< OpenALのデバイス
	ALCcontext* context;	//!< OpenALのコンテキスト
	ALint state;			//!< OpenALの状態
	ALuint source;			//!< OpenALのソース
	ALfloat volume;			//!< ボリューム
	char temp[AL_BUFFER_SIZE];		//!< ストリーミングするときのバッファ
	ALuint buffers[AL_BUFFER_COUNT];//!< OpenALのバッファ

	OggVorbis_File vf;		//!< Oggのファイルポインタ
	MemoryStream stream;	//!< 簡易メモリストリーム
	ALenum format;			//!< サウンドのフォーマット(mono8, mono16, stero8 or stero16)
	ALsizei freq;			//!< サウンドの周波数

	/**
	 * oggファイルをデコードします.
	 * @param vf oggファイル
	 * @param buf バッファ
	 * @param length バッファサイズ
	 * @return デコードサイズ
	 */
	size_t decode(OggVorbis_File* vf, char* buf, int length);

	/**
	 * OpenALの初期化を行います.
	 * @return true: 初期化成功
	 */
	bool initialize();

	/**
	 * WAVサウンドファイルを読み込みます.
	 * @param data WAVデータ
	 */
	bool load1(std::vector<char> *data);

	/**
	 * Oggサウンドファイルを読み込みます.
	 * @param data Oggデータ
	 */
	bool load2(std::vector<char> *data);

	/**
	 * Oggファイルのストリーミング用のデータを読み込みます.
	 * @param data
	 * @return
	 */
	bool load3(std::vector<char> *data);

public:
	/**
	 * コンストラクタ.
	 */
	OpenALPlayer();
	virtual ~OpenALPlayer();

	/**
	 * 指定されたサウンドファイルをOpenALに設定します.
	 * @param fname ファイル名
	 * @return true: 成功
	 */
	bool load(const char *fname);

	/**
	 * 指定されたサウンドデータをOpenALに設定します.
	 * @param data サウンドデータ
	 * @return true: 成功
	 */
	bool load(std::vector<char> *data);

	/**
	 * サウンドを再生します.
	 * @return true: 再生成功
	 */
	bool play();

	/**
	 * サウンドを停止します.
	 * @return true: 停止成功
	 */
	bool stop();

	/**
	 * ストリーミング再生を行います.
	 * この関数を刻む毎に再生されます。
	 * @return true: 再生成功
	 */
	bool tick();

	/**
	 * Playerが再生中かチェックします.
	 * @return true: 再生中
	 */
	bool isPlaying();

	/**
	 * サウンドのボリュームを設定します.
	 * @param volume ボリューム
	 */
	void setVolume(float volume);

	/**
	 * OpenALの破棄を行います.
	 */
	void dispose();
};

#endif /* OPENALPLAYER_H_ */
