#define STB_IMAGE_IMPLEMENTATION
#include "../3rdparty/stb_image.h"

#include "image.hpp"


uint8_t* read_stb_rgb(const char* file, int &width, int &height, int &channels) {
    return stbi_load(file, &width, &height, &channels, STBI_rgb);
}


