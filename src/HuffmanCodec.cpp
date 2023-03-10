#include <cstdlib>
#include "HuffmanCodec.hpp"
#include "../3rdparty/bitstr.h"
#include <stdexcept>

const uint8_t HuffmanCodec::STD_HUFTAB_LUMIN_AC[] = {
        0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d,
        0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
        0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
        0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
        0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
        0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
        0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
        0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
        0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
        0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
        0xf9, 0xfa,
};
const uint8_t HuffmanCodec::STD_HUFTAB_LUMIN_DC[] = {
        0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
};
const uint8_t HuffmanCodec::STD_HUFTAB_CHROM_AC[] = {
        0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,
        0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
        0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
        0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
        0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
        0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
        0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
        0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
        0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
        0xf9, 0xfa
};
const uint8_t HuffmanCodec::STD_HUFTAB_CHROM_DC[] = {
        0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
};

HuffmanCodec::HuffmanCodec() : mBitStream(nullptr) {
    initCodeList(true, true);
    initCodeList(true, false);
    initCodeList(false, true);
    initCodeList(false, false);
}

void HuffmanCodec::initCodeList(bool dc, bool luminance) {
    int i, j, k;
    int symbol;
    int code;
    uint8_t hufsize[256];
    int hufcode[256];
    int tabsize;

    k = 0;
    code = 0x00;
    const uint8_t *hufTable;
    HUFCODEITEM *codeList;
    if (dc && luminance) {
        hufTable = STD_HUFTAB_LUMIN_DC;
        codeList = mCodeListDCLumin;
    } else if (dc && !luminance) {
        hufTable = STD_HUFTAB_CHROM_DC;
        codeList = mCodeListDCChrom;
    } else if (!dc && luminance) {
        hufTable = STD_HUFTAB_LUMIN_AC;
        codeList = mCodeListACLumin;
    } else {
        hufTable = STD_HUFTAB_CHROM_AC;
        codeList = mCodeListACChrom;
    }
    for (i = 0; i < MAX_HUFFMAN_CODE_LEN; i++) {
        for (j = 0; j < hufTable[i]; j++) {
            hufsize[k] = i + 1;
            hufcode[k] = code;
            code++;
            k++;
        }
        code <<= 1;
    }
    tabsize = k;
    for (i = 0; i < tabsize; i++) {
        symbol = hufTable[MAX_HUFFMAN_CODE_LEN + i];
        codeList[symbol].depth = hufsize[i];
        codeList[symbol].code = hufcode[i];
    }
}

HuffmanCodec::~HuffmanCodec() {
    if (mBitStream) {
        bitstr_flush(mBitStream, 1);
        bitstr_close(mBitStream);
    }
    if (mBuffer) {
        free(mBuffer);
    }
}

bool HuffmanCodec::huffmanEncode(HUFCODEITEM *codeList, int size) {
    unsigned code;
    int len;
    if (!mBitStream) {
        return false;
    }

    code = codeList[size].code;
    len = codeList[size].depth;
    if (EOF == bitstr_put_bits(mBitStream, code, len)) {
        return false;
    }

    return true;
}

void HuffmanCodec::categoryEncode(int &code, int &size) {
    unsigned absc = abs(code);
    unsigned mask = (1 << 15);
    int i = 15;
    if (absc == 0) {
        size = 0;
        return;
    }
    while (i && !(absc & mask)) {
        mask >>= 1;
        i--;
    }
    size = i + 1;
    if (code < 0) {
        code = (1 << size) - absc - 1;
    }
}

void HuffmanCodec::encodeBlock(const int *const block, int &dc, bool luminance) {
    void *bs = mBitStream;
    int diff, code, size;
    RLEITEM rlelist[63];
    int i, j, n, eob;

    // DC ????????????????????????????????????DPCM???
    diff = block[0] - dc;
    dc = block[0];
    code = diff;
    // DC ???????????????????????????
    // category encode for dc
    // ??????category?????????????????????????????????????????????????????????
    // https://sce.umkc.edu/faculty-sites/lizhu/teaching/2018.fall.video-com/notes/lec04.pdf ???16???
    categoryEncode(code, size);
    // ????????? DC
    // huffman encode for dc
    huffmanEncode(luminance ? mCodeListDCLumin : mCodeListDCChrom, size);
    bitstr_put_bits(bs, code, size);

    // AC ??????????????????????????????RLE???
    // AC ???????????????????????????
    // rle encode for ac
    for (i = 1, j = 0, n = 0, eob = 0; i < 64 && j < 63; i++) {
        if (block[i] == 0 && n < 15) {
            n++;
        } else {
            code = block[i];
            size = 0;
            // AC ???????????????????????????
            categoryEncode(code, size);
            rlelist[j].runlen = n;
            rlelist[j].codesize = size;
            rlelist[j].codedata = code;
            n = 0;
            j++;
            if (size != 0) {
                eob = j;
            }
        }
    }
    // ?????? eob
    if (block[63] == 0) {
        rlelist[eob].runlen = 0;
        rlelist[eob].codesize = 0;
        rlelist[eob].codedata = 0;
        j = eob + 1;
    }
    // ????????? AC
    // huffman encode for ac
    for (i = 0; i < j; i++) {
        huffmanEncode(luminance ? mCodeListACLumin : mCodeListACChrom,
                      (rlelist[i].runlen << 4) | (rlelist[i].codesize << 0));
        bitstr_put_bits(bs, rlelist[i].codedata, rlelist[i].codesize);
    }
}

inline int div_up(int a, int b) { return (a + b - 1) / b; }

long HuffmanCodec::encode(const int* yBlocks, const int* uBlocks, const int* vBlocks,
                          const int w, const int h, YUVFormat format) {
    if (mBitStream == nullptr) {
        mBuffer = static_cast<char *>(malloc(w * h * 2));
        mBitStream = bitstr_open(BITSTR_MEM, mBuffer, reinterpret_cast<char *>(w * h * 2));
    }
    int dcCache[3] = {0, 0, 0}; // cache for DPCM 

    if (format == YUVFormat::YUV444) {
        for(size_t i = 0; i < div_up(w, 8) * div_up(h, 8); ++i) {
            encodeBlock(yBlocks + i * 64, dcCache[0], true);
            encodeBlock(uBlocks + i * 64, dcCache[1], false);
            encodeBlock(vBlocks + i * 64, dcCache[2], false);
        }
    } else if(format == YUVFormat::YUV420) {
        for(size_t i = 0; i < div_up(w, 16) * div_up(h, 16); ++i) {
            encodeBlock(yBlocks + i * 256,       dcCache[0], true);
            encodeBlock(yBlocks + i * 256 + 64 , dcCache[0], true);
            encodeBlock(yBlocks + i * 256 + 128, dcCache[0], true);
            encodeBlock(yBlocks + i * 256 + 192, dcCache[0], true);

            encodeBlock(uBlocks + i * 64, dcCache[1], false);
            encodeBlock(vBlocks + i * 64, dcCache[2], false);
        }
    } else if(format == YUVFormat::YUV422) {
        for(size_t i = 0; i < div_up(w, 16) * div_up(h, 8); ++i) {
            encodeBlock(yBlocks + i * 128,       dcCache[0], true);
            encodeBlock(yBlocks + i * 128+64,    dcCache[0], true);
            
            encodeBlock(uBlocks + i * 64, dcCache[1], false);
            encodeBlock(vBlocks + i * 64, dcCache[2], false);
        } 
    } else {
        throw std::runtime_error("unsupported YUV format!");
    }
    return bitstr_tell(mBitStream);
}

char* HuffmanCodec::getResult() {
    return mBuffer;
}
