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


///
/// convert RGB image to YUV format, YUV444 means that there is no sub-sampling for U,V channels
///
Image<uint8_t> JpegColor::rgbToYUV444(const Image<uint8_t> &rgb) {
    if (rgb.channels() != 3) {
        throw std::runtime_error(" input image's channels != 3 "); 
    }

    Image<uint8_t> yuv(rgb);
    /* TODO: implement color transform here : RGB -> YUV ,
     * Image class (see include/image.hpp) API ref.:  
     *  - rows(), cols(), channels()  
     *  - rgb.(y, x, c) -> return pixel at (y, x, c)
     */

    throw std::runtime_error("TODO: rgbToYUV444 was not implemented!"); // delete this line when you implement the conversion

    return yuv;
}

///
/// sample blocks from a given YUV image
///
void JpegColor::sampleToBlocks(const Image<uint8_t> &image, 
                               std::vector<uint8_t> &y_blocks, 
                               std::vector<uint8_t> &u_blocks, 
                               std::vector<uint8_t> &v_blocks, 
                               const int block_w, const int block_h, 
                               const int sx, const int sy 
                               ) {
    const int w = image.cols();
    const int h = image.rows();
    int block_nw = div_up(w, block_w); // make sure cover all pixels
    int block_nh = div_up(h, block_h);

    // sx, sy are sampling factors from horizontal and vertical directions, e.g., for YUV420, the macroblock size will be 16*16,
    // there will be four sequential 8*8 blocks for Y channel; for U, V channel, a macroblock (16*16) will be 
    // down-sampled to a 8*8 block. In this case, sx=2, sy=2, block_w=block_h=16, 
    // block_stride = 64 refers to the size of a 8*8 block, since we flatten blocks into a one-dimensional vector. 
    // here N = block_nw * block_nh is the total number of macroblocks (16*16), 
    // y_blocks will have N * sx * sy blocks (8*8), u_blocks/v_blocks will have N blocks (8*8), 
    // thus, given a macroblock with shape of 16*16, it should output four sequential 8*8 blocks (from left to right, from top to bottom) for Y channel,
    // one 8*8 block for U channel, one 8*8 block for V channel.
    // here we store blocks in y_blocks, u_blocks, v_blocks respectively as they will be processed indivisually.
    // 
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
