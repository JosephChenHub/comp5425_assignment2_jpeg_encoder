#pragma once

#include <string>
#include "JpegQuant.hpp"
#include "JpegColor.hpp"
#include "image.hpp"

class JpegEncoder {
public:
    JpegEncoder(std::string outputPath): mOutputPath(outputPath) { };
    ~JpegEncoder()=default;

    void encodeRGB(const Image<uint8_t> &rgb_img,
                   const int quality, 
                   YUVFormat format, 
                   const bool force_baseline=true 
                   );

private:

    std::vector<int> blocksToFDCT(const std::vector<uint8_t> &blocks, 
                                  const int block_stride);

    void fdctToQuant(JpegQuant* quantizer,
                     std::vector<int> &dct,  
                     const int block_stride, 
                     const bool luminance
                     );

    void quantToZigzag(std::vector<int> &quant, const int block_stride);

private:
    std::string mOutputPath;
     
};
