#pragma once

#include "image.hpp"

enum class YUVFormat{
   YUV444,
   YUV420,
   YUV422
};

class JpegColor {
public:
   JpegColor()=default;
   ~JpegColor()=default;

   static Image<uint8_t> rgbToYUV444(const Image<uint8_t> &rgb);

   static void sampleToBlocks(const Image<uint8_t> &img, 
                        std::vector<uint8_t> &y_blocks, 
                        std::vector<uint8_t> &u_blocks, 
                        std::vector<uint8_t> &v_blocks, 
                        const int block_w, const int block_h,
                        const int sx, const int sy 
                        );
}; // end of class


// end of file

