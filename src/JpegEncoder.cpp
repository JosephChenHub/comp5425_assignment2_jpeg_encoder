/// a more concise reference: https://www.jonolick.com/uploads/7/9/2/1/7921194/jo_jpeg.cpp

#include "JpegEncoder.hpp"
//#include "JpegDCT.hpp"
#include "JpegZigzag.hpp"
#include "HuffmanCodec.hpp"
#include "JpegIO.hpp"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <memory>


void JpegEncoder::encodeRGB(const Image<uint8_t> &rgb,
                            const int quality, 
                            YUVFormat format,
                            const bool force_baseline
                            ) {

    /// step 0 : RGB -> YUV 
    const int width = rgb.cols();
    const int height = rgb.rows();
    Image<uint8_t> yuv = JpegColor::rgbToYUV444(rgb);

    /// step 1 : subsampling chrominance if required
    int block_w = 8, block_h = 8;
    int sx = 1, sy = 1;
    if (format == YUVFormat::YUV444) {
        // do nothing 
    }
    else if (format == YUVFormat::YUV420) {
        block_w = 16; block_h = 16;
        sx = 2; sy = 2;
    } else if (format == YUVFormat::YUV422) {
        block_w = 16; block_h = 8;
        sx = 2; sy = 1;
    } else {
        throw std::runtime_error("not supported yuv format!");
    }

    // step 2 : divide blocks
    std::vector<uint8_t> y_blocks, u_blocks, v_blocks; 
    JpegColor::sampleToBlocks(yuv, y_blocks, u_blocks, v_blocks,
                              block_w, block_h, sx, sy);


    /// step 3: apply DCT for each 8x8 block
    std::vector<int> y_dct = blocksToFDCT(y_blocks, 64);
    std::vector<int> u_dct = blocksToFDCT(u_blocks, 64);
    std::vector<int> v_dct = blocksToFDCT(v_blocks, 64);

    // quantization
    std::shared_ptr<JpegQuant> quantizer = std::make_shared<JpegQuant>(quality, force_baseline);
    fdctToQuant(quantizer.get(), y_dct, 64, true);
    fdctToQuant(quantizer.get(), u_dct, 64, false);
    fdctToQuant(quantizer.get(), v_dct, 64, false);

    // zigzag order
    quantToZigzag(y_dct, 64);
    quantToZigzag(u_dct, 64);
    quantToZigzag(v_dct, 64);

    // entropy encoding
    std::shared_ptr<HuffmanCodec> huffmanCodec = std::make_shared<HuffmanCodec>();
    long dataLength = huffmanCodec->encode(y_dct.data(), u_dct.data(), v_dct.data(), width, height, format);
    std::cout << "JpegEncoder encode length:" << dataLength << std::endl; 
    if (dataLength <= 0) {
        return;
    }

    // write to disk
    const int* pqtab[2] = {quantizer->qtable_lumin.data(), quantizer->qtable_chrom.data()};
    const uint8_t* huf_ac_tab[2] = {HuffmanCodec::STD_HUFTAB_LUMIN_AC, HuffmanCodec::STD_HUFTAB_CHROM_AC };
    const uint8_t* huf_dc_tab[2] = {HuffmanCodec::STD_HUFTAB_LUMIN_DC, HuffmanCodec::STD_HUFTAB_CHROM_DC };


    JpegIO::writeToFile(this->mOutputPath.c_str(),
                        huffmanCodec->getResult(), dataLength, 
                        pqtab, huf_ac_tab, huf_dc_tab, 
                        width, height, format);
    float ratio = width * height * 3 / dataLength;
    std::cout<< "JPEG compression ratio:" << ratio << std::endl;
}






std::vector<int> JpegEncoder::blocksToFDCT(const std::vector<uint8_t> &blocks, const int block_stride) {
    const int block_numel = blocks.size() / block_stride;
    ASSERT(blocks.size() % block_stride == 0, " blocks.size() % block_stride != 0");
    std::vector<int> dct(blocks.size());

    for (int block_id = 0; block_id < block_numel; ++block_id) {
        /* TODO: apply DCT transform to each block, this operation is in-place*/
        throw std::runtime_error("TODO: blocksToFDCT was not implemented!");
    }
    return dct;
}

void JpegEncoder::fdctToQuant(JpegQuant* quantizer, 
                              std::vector<int> &dct, 
                              const int block_stride, 
                              const bool luminance  
                              ) {
    const int block_numel = dct.size() / block_stride;
 
    for (size_t block_id = 0; block_id < block_numel; ++block_id) {
        quantizer->quantEncode8x8(dct.data() + block_id * block_stride, luminance);
    }
}

void JpegEncoder::quantToZigzag(std::vector<int> &quant,
                                const int block_stride
                               ) {
    const int block_numel = quant.size() / block_stride;
    std::shared_ptr<JpegZigzag> zigzag = std::make_shared<JpegZigzag>();
    for (size_t block_id = 0; block_id < block_numel; ++block_id) {
        zigzag->zigzag(quant.data() + block_id * block_stride);
    }
}
