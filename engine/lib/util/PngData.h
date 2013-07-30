/*
 * PngData.h
 *
 *  Created on: 2011/11/05
 *      Author: GClue, Inc.
 */
#ifndef PNGDATA_H_
#define PNGDATA_H_

#include <vector>

class PngData {
public:
	unsigned char *data;
	int length;
	int width;
	int height;
	int bpp;
	int color_type;
	
	PngData();
	virtual ~PngData();
	
	bool loadData(unsigned char *data, int size);
	bool load(std::vector<unsigned char>*fdata);
	bool load(const char *name);
};

#endif /* PNGDATA_H_ */
