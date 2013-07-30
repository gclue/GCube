/*
 * PngData.cpp
 *
 *  Created on: 2011/11/05
 *      Author: GClue, inc.
 */
#include "PngData.h"
#include <string.h>
#include <png.h>
#include "AssetManager.h"

typedef struct my_png_buffer_ {
	unsigned char *data;
	unsigned long data_len;
	unsigned long data_offset;
} my_png_buffer;

static void png_memread_func(png_structp png_ptr, png_bytep buf, png_size_t size) {
	my_png_buffer *png_buff = (my_png_buffer *) png_get_io_ptr(png_ptr);
	if (png_buff->data_offset + size <= png_buff->data_len) {
		memcpy(buf, png_buff->data + png_buff->data_offset, size);
		png_buff->data_offset += size;
	} else {
		png_error(png_ptr, "png_mem_read_func failed");
	}
}

static void png_data_read(png_structp png_ptr, my_png_buffer *png_buff) {
	png_set_read_fn(png_ptr, (png_voidp) png_buff, (png_rw_ptr) png_memread_func);
}

//static uint16_t make565(int r, int g, int b) {
//	return (uint16_t) (((r << 8) & 0xf800) |
//					   ((g << 2) & 0x03e0) |
//					   ((b >> 3) & 0x001f));
//}

static uint32_t makeRGBA(int r, int g, int b) {
	return (uint32_t) (((r << 16) & 0x00FF0000) |
					   ((g <<  8) & 0x0000FF00) |
					   ((b <<  0) & 0x000000FF) | 0xFF000000);
}

PngData::PngData() {
	data = NULL;
	width = 0;
	height = 0;
}

PngData::~PngData() {
	delete data;
}

bool PngData::loadData(unsigned char *fdata, int fsize)
{
	my_png_buffer png_buff;
	png_buff.data_offset = 0;
	png_buff.data_len = fsize;
	png_buff.data = fdata;
	
	bool is_png = png_check_sig((png_bytep) png_buff.data, 8);
	if (!is_png) {
		return false;
	}
	
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		return false;
	}
	
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}
	// pngのデータを読み込む
	png_data_read(png_ptr, &png_buff);
	// 情報を読み込む
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	
	// 情報を取得
	width  = info_ptr->width;
	height = info_ptr->height;
	bpp = info_ptr->channels;
	color_type = info_ptr->color_type;
	
	// テクスチャ用のバッファ
	data = new unsigned char[width * height * 4];
	if (data == NULL) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}
	
	png_byte** row_pointers = png_get_rows(png_ptr, info_ptr);
	
	// libpngからテクスチャ用に変換
	switch (color_type) {
		case PNG_COLOR_TYPE_RGB: {
			for (int yy = 0; yy < height; yy++) {
				uint32_t* line = (uint32_t *) &data[yy * width * 4];
				char *pixel = (char *) (row_pointers[yy]);
				for (int xx = 0; xx < width; xx++) {
					line[xx] = makeRGBA(pixel[2], pixel[1], pixel[0]);
					pixel += 3;
				}
			}
		}	break;
		case PNG_COLOR_TYPE_RGBA: {
			for (int yy = 0; yy < height; yy++) {
				uint32_t* line = (uint32_t *) &data[yy * width * 4];
				uint32_t *pixel = (uint32_t *) (row_pointers[yy]);
				memcpy(line, pixel, sizeof(uint32_t) * width);
			}
		}	break;
	}
	
	int *a = (int *)data;
	
	// libpngのインスタンスを削除
	png_destroy_read_struct(&png_ptr, NULL, NULL);
	a++;
	return true;
}

bool PngData::load(const char *name) {
	AssetManager mgr = AssetManager::getInstance();
	std::vector<unsigned char>*fdata = (std::vector<unsigned char> *)mgr.open(name);
	if (fdata) {
		bool result = load(fdata);
		delete fdata;
		return result;
	} else {
		return false;
	}
}

bool PngData::load(std::vector<unsigned char>*fdata) {
	return loadData((unsigned char *) &(*fdata)[0], fdata->size());
}
