//
//  Base64.cpp
//  GCube
//
//  Created by 小林 伸郎 on 2013/07/24.
//
//

#include "Base64.h"

bool Base64::encode(const std::vector<unsigned char>& src, std::string& dst)
{
	const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    if (src.empty()) return false;
	int in_size = src.size();
    for (int i = 0; i < (in_size - 1) / 3 + 1; i++) {
        unsigned char* c = (unsigned char *) &(src)[i * 3];
        for (int j = 0; j < 4; j++) {
            if (in_size >= i * 3 + j) {
				dst.append(b64[(c[j - 1] << (6 - j * 2) | c[j] >> (j * 2 + 2)) & 0x3f], 1);
			} else {
				dst.append('=', 1);
			}
		}
    }
    return true;
}

bool Base64::decode(const std::string& src, std::vector<unsigned char>& dst)
{
	char b64[128] = {0x0};
	char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	for (char n = 0, *w = table; *w; b64[*w++] = n++ & 0x3f);
	
    char c[4];
	unsigned char *p = (unsigned char *) &(src)[0];
    int j, k;
    while (*p && *p != '=') {
        for (j = 0; j < 4 && *p != '='; j++) {
			c[j] = b64[*p++];
		}
        for (k = 0; k < j - 1; k++) {
			dst.push_back(c[k] << (k << 1) + 2 | c[k + 1] >> (2 - k << 1));
		}
    }
	return true;
}
