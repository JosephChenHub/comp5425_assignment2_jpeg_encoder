#include "JpegColor.hpp"
#include <cmath>
#include <stdexcept>


template <typename T>
inline T bound(T min, T value, T max) {
    return value <= min ? min : (value >= max ? max: value);
}

inline int div_up(int a, int b) {
    return (a + b - 1) / b;
}

Image<uint8_t> JpegColor::rgbToYUV444(const Image<uint8_t> &rgb) {
    if (rgb.channels() != 3) {
        throw std::runtime_error(" input image's channels != 3 "); 
    }

    Image<uint8_t> yuv(rgb);
    /* TODO: implement color transform here : RGB -> YUV ,
     * Image class API ref.:  
     *  - rows(), cols(), channels()  
     *  - rgb.(y, x, c) -> return pixel at (y, x, c)
     */

    throw std::runtime_error("TODO: rgbToYUV444 was not implemented!"); // delete this line when you implement the conversion

    return yuv;
}

void JpegColor::sampleToBlocks(const Image<uint8_t> &image, 
                               std::vector<uint8_t> &y_blocks, 
                               std::vector<uint8_t> &u_blocks, 
                               std::vector<uint8_t> &v_blocks, 
                               const int block_w, const int block_h, 
                               const int sx, const int sy 
                               ) {
    const int w = image.cols();
    const int h = image.rows();
    int block_nw = div_up(w, block_w);
    int block_nh = div_up(h, block_h);
    size_t block_stride = int(block_h / sy) * int(block_w / sx); 
    y_blocks.resize(block_stride * block_nw * block_nh * sx * sy);
    u_blocks.resize(block_stride * block_nw * block_nh );
    v_blocks.resize(block_stride * block_nw * block_nh );

    for (size_t by = 0; by < block_nh; ++by) {
         for(size_t bx = 0; bx < block_nw; ++bx) {
             size_t block_id = bx + by * block_nw; 
             // scan each block of ROI, from left to right, from top to bottom
             /* TODO: implement here */
             throw std::runtime_error("TODO: sampleToBlocks was not implemented!"); 
         }
    }
}
