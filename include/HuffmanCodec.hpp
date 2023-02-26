#pragma once

#include <cstdint>

#include "JpegColor.hpp"

typedef struct {
    unsigned runlen: 4;
    unsigned codesize: 4;
    unsigned codedata: 16;
} RLEITEM;

typedef struct {
    int symbol; 
    int freq;   
    int group;  
    int depth;  
    int code;   
} HUFCODEITEM;

class HuffmanCodec {
public:
    HuffmanCodec();
    ~HuffmanCodec();

    long encode(const int* yBlocks, const int* uBlocks, const int* vBlocks,
                const int w, const int h, YUVFormat format);

    char* getResult();
private:
    void initCodeList(bool dc, bool luminance);

    void encodeBlock(const int *const block, int &dc, bool luminance);

    void categoryEncode(int &code, int &size);

    bool huffmanEncode(HUFCODEITEM *codeList, int size);

public:
    static const uint8_t STD_HUFTAB_LUMIN_AC[];
    static const uint8_t STD_HUFTAB_LUMIN_DC[];
    static const uint8_t STD_HUFTAB_CHROM_AC[];
    static const uint8_t STD_HUFTAB_CHROM_DC[];

private:
    char *mBuffer;
    void *mBitStream;
    HUFCODEITEM mCodeListDCLumin[256]; 
    HUFCODEITEM mCodeListDCChrom[256]; 
    HUFCODEITEM mCodeListACLumin[256]; 
    HUFCODEITEM mCodeListACChrom[256]; 

    const uint8_t MAX_HUFFMAN_CODE_LEN = 16;
};
