#pragma once

#include <cstdint>
#include <cassert>

#include "JpegColor.hpp"

class JpegIO {
public:
  JpegIO()=default;
  ~JpegIO()=default;

public:
   static bool writeToFile(const char* dst_file, /* destination file, e.g., 001.jpg */
                    const char* buffer,   /* encoded image data */
                    long dataLength,
                    const int* quant_tab[2], /* quantization table : luminance, chrominance */
                    const uint8_t* huf_ac_tab[2], /* huffman coding table: AC */
                    const uint8_t* huf_dc_tab[2], /* huffman coding table: DC */
                    const int w, const int h,
                    YUVFormat format);
};
