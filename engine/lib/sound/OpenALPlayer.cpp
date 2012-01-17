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
 * OpenALPlayer.cpp
 *
 *  Created on: 2011/11/10
 *      Author: GClue, Inc.
 */
#include "OpenALPlayer.h"

#include <stdlib.h>
#include <math.h>

#include "AssetManager.h"
#include "Log.h"

/** Wavファイルのヘッダーを定義. */
typedef struct {
	char riff[4];					//!< 'RIFF'
	unsigned int riffSize;			//!< (ファイルのバイト数)-8
	char wave[4];					//!< 'WAVE'
	char fmt[4];					//!< 'fmt '
	unsigned int fmtSize;			//!< PCMWAVEFORMAT構造体のバイト数=常に16
	// PCMWAVEFORMAT構造
	unsigned short format;
	unsigned short channels;
	unsigned int samplesPerSec;
	unsigned int bytesPerSec;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char data[4];					//!< 'data'
	unsigned int dataSize;
} WAVHEADER;

static const ALint context_attribs[] = { ALC_FREQUENCY, 22050, 0 };

static size_t ogg_read_func(void *ptr, size_t size, size_t nmemb, void *datasource) {
	MemoryStream* stream = (MemoryStream*) datasource;

	size_t length = size * nmemb;
	if (length > stream->size - stream->offset) {
		length = stream->size - stream->offset;
	}

	if (length > 0) {
		memcpy(ptr, stream->buffer + stream->offset, length);
		stream->offset += length;
	} else {
		LOGE("ERRORc");
	}
	return length;
}

static int ogg_seek_func(void *datasource, ogg_int64_t offset, int whence) {
	MemoryStream* stream = (MemoryStream*) datasource;

	switch (whence) {
	case SEEK_CUR:
		stream->offset += offset;
		break;
	case SEEK_END:
		stream->offset = stream->size + offset;
		break;
	case SEEK_SET:
		stream->offset = offset;
		break;
	default:
		LOGD("ERROR %ld", offset);
		return -1;
	}

	if (stream->offset > stream->size) {
		stream->offset = stream->size;
		LOGE("ERRORa");
		return -1;
	} else if (stream->offset < 0) {
		stream->offset = 0;
		LOGE("ERRORb");
		return -1;
	}
	return 0;
}

static long ogg_tell_func(void *datasource) {
	MemoryStream* stream = (MemoryStream*) datasource;
	return (long) stream->offset;
}

static int ogg_close_func(void *datasource) {
//	LOGE("ogg_close_func");
	return 0;
}

static ov_callbacks cb = { ogg_read_func, ogg_seek_func, ogg_close_func, ogg_tell_func};

OpenALPlayer::OpenALPlayer() {
	device = NULL;
	context = NULL;
	source = 0;
	volume = 1.0f;
	vf.datasource = NULL;
}

OpenALPlayer::~OpenALPlayer() {
	dispose();
}


/**
 * 指定されたサウンドファイルをOpenALに設定します.
 * @param fname ファイル名
 * @return true: 成功
 */
bool OpenALPlayer::load(const char *fname) {
	AssetManager mgr = AssetManager::getInstance();
	std::vector<char> *fdata = mgr.open(fname);
	if (fdata) {
		bool result = load(fdata);
		delete fdata;
		return result;
	}
	return false;
}

bool OpenALPlayer::load(std::vector<char> *data) {
	unsigned char *fdata = (unsigned char *) &(*data)[0];
	if (memcmp("RIFF", fdata, 4) == 0) {
		return load1(data);
	} else if (memcmp("Ogg", fdata, 3) == 0) {
		return load2(data);
	} else {
		return false;
	}
}

bool OpenALPlayer::load1(std::vector<char> *data) {
	WAVHEADER *header;

	stream.offset = 0;
	stream.size = data->size();
	stream.buffer = (unsigned char *) &(*data)[0];

	// OpenALの初期化
	if (!initialize()) {
		goto ERROR;
	}

	// WAVのヘッダーチェック
	header = (WAVHEADER *) &(*data)[0];
	if ((memcmp("RIFF", header->riff, 4) ||
		 memcmp("WAVE", header->wave, 4) ||
		 memcmp("fmt ", header->fmt,  4) ||
		 memcmp("data", header->data, 4))) {
		LOGE("Illegal File Format.");
		goto ERROR;
	}

	switch (header->bitsPerSample) {
	case 8:
		format = (header->channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
		break;
	case 16:
		format = (header->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		break;
	default:
		goto ERROR;
	}
	freq = header->samplesPerSec;

	alBufferData(buffers[0], format, &stream.buffer[sizeof(WAVHEADER)], header->dataSize, freq);
	if (alGetError() != AL_NO_ERROR) {
		LOGE("Failed to alBufferData");
		goto ERROR;
	}
	return true;

ERROR:
	dispose();
	return false;
}

bool OpenALPlayer::load2(std::vector<char> *data) {
	int channels;
	std::vector<char> buf;

	stream.offset = 0;
	stream.size = data->size();
	stream.buffer = (unsigned char *) &(*data)[0];

	// OpenALの初期化
	if (!initialize()) {
		goto ERROR;
	}

	// oggのファイルを開きます.
	if (ov_open_callbacks(&stream, &vf, NULL, 0, cb) != 0) {
		LOGE("Failed to open ogg file");
		goto ERROR;
	}

	channels = ov_info(&vf, 0)->channels;
	if (channels != 1 && channels != 2) {
		LOGE("Unsupported ogg file - %i channels. Only mono or stereo supported", channels);
		goto ERROR;
	}

	format = (channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
	freq = ov_info(&vf, -1)->rate;

	// oggをデコード
	while (stream.offset < stream.size) {
		size_t decoded = decode(&vf, temp, AL_BUFFER_SIZE);
		std::vector<char> tmp(temp, temp+decoded);
		buf.insert(buf.end(), tmp.begin(), tmp.end());
	}


	if (buf.size() <= 0) {
		// デコードに失敗
		goto ERROR;
	}

	alBufferData(buffers[0], format, (unsigned char *) &(buf)[0], buf.size(), freq);
	if (alGetError() != AL_NO_ERROR) {
		LOGE("Failed to alBufferData");
		goto ERROR;
	}
	return true;

ERROR:
	dispose();
	return false;
}

bool OpenALPlayer::load3(std::vector<char> *data) {
	int channels;

	stream.offset = 0;
	stream.size = data->size();
	stream.buffer = (unsigned char *) &(*data)[0];

	// OpenALの初期化
	if (!initialize()) {
		goto ERROR;
	}

	if (ov_open_callbacks(&stream, &vf, NULL, 0, cb) != 0) {
		LOGE("Failed to open ogg file");
		goto ERROR;
	}

	channels = ov_info(&vf, 0)->channels;
	if (channels != 1 && channels != 2) {
		LOGE("Unsupported ogg file - %i channels. Only mono or stereo supported", channels);
		goto ERROR;
	}

	format = (channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
	freq = ov_info(&vf, 0)->rate;

	for (int i = 0; i < AL_BUFFER_COUNT; i++) {
		size_t decoded = decode(&vf, temp, AL_BUFFER_SIZE);
		if (decoded) {
			alBufferData(buffers[i], format, temp, decoded, freq);
			if (alGetError() != AL_NO_ERROR) {
				goto ERROR;
			}
			alSourceQueueBuffers(source, 1, &buffers[i]);
			if (alGetError() != AL_NO_ERROR) {
				goto ERROR;
			}
		}
	}
	return true;

ERROR:
	dispose();
	return false;
}


void OpenALPlayer::setVolume(float volume) {
	this->volume = volume;
	if (source) {
		alSourcef(source, AL_GAIN, volume);
	}
}

bool OpenALPlayer::play() {
	if (source) {
		alSourcef(source, AL_GAIN, volume);
		alSourcei(source, AL_BUFFER, buffers[0]);
		alSourcePlay(source);
		return isPlaying();
	}
	return false;
}

bool OpenALPlayer::stop() {
	if (source) {
		if (isPlaying()) {
			alSourceStop(source);
		}
		return true;
	}
	return false;
}

bool OpenALPlayer::tick() {
	if (!source) {
		return false;
	}

	if (isPlaying()) {
		return true;
	}

	if (stream.offset < stream.size) {
		ALint processed;

		alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
		if (alGetError() != AL_NO_ERROR) {
			goto ERROR;
		}

		while (processed-- > 0) {
			ALuint buf;

			alSourceUnqueueBuffers(source, 1, &buf);
			if (alGetError() != AL_NO_ERROR) {
				LOGE("Failed to alSourceUnqueueBuffers.");
				goto ERROR;
			}

			size_t decoded = decode(&vf, temp, AL_BUFFER_SIZE);
			if (decoded) {
				alBufferData(buf, format, temp, decoded, freq);
				if (alGetError() != AL_NO_ERROR) {
					LOGE("Failed to alBufferData.");
					goto ERROR;
				}
				alSourceQueueBuffers(source, 1, &buf);
				if (alGetError() != AL_NO_ERROR) {
					LOGE("Failed to alSourceQueueBuffers.");
					goto ERROR;
				}
			}
		}

		if (!isPlaying()) {
			alSourcef(source, AL_GAIN, volume);
			alSourcePlay(source);
		}
	}
	return true;
ERROR:
	return false;
}

bool OpenALPlayer::isPlaying() {
	if (source == 0) {
		return false;
	}
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void OpenALPlayer::dispose() {
	if (buffers[0]) {
		alDeleteBuffers(AL_BUFFER_COUNT, buffers);
	}
	buffers[0] = NULL;

	if (source) {
		alDeleteSources(1, &source);
	}
	source = 0;

	if (context) {
		alcMakeContextCurrent( NULL);
		alcDestroyContext(context);
	}
	context = NULL;

	if (device) {
		alcCloseDevice(device);
	}
	device = NULL;

	if (vf.datasource) {
		ov_clear(&vf);
	}
	vf.datasource = NULL;
}

bool OpenALPlayer::initialize() {
	device = alcOpenDevice(NULL);
	if (!device) {
		LOGE("Failed to open OpenAL device");
		return false;
	}

	context = alcCreateContext(device, context_attribs);
	if (!context) {
		LOGE("Failed to create OpenAL context");
		return false;
	}
	alcMakeContextCurrent(context);

	alGenSources(1, &source);
	if (alGetError() != AL_NO_ERROR) {
		LOGE("Failed to generate OpenAL Source");
		return false;
	}

	alGenBuffers(AL_BUFFER_COUNT, buffers);
	if (alGetError() != AL_NO_ERROR) {
		LOGE("Failed to generate OpenAL Buffer");
		return false;
	}

	return true;
}

size_t OpenALPlayer::decode(OggVorbis_File* vf, char* buffer, int length) {
	size_t left = length;
	int bitstream = 0;
	while (left != 0) {
		long decoded = ov_read(vf, buffer, left, 0, 2, 1, &bitstream);
		if (decoded > 0) {
			buffer += decoded;
			left -= decoded;
		} else if (decoded == 0) {
			break;
		} else {
			LOGE("Ogg decoding error, will try to resume decoding");
		}
	}

	return length - left;
}
