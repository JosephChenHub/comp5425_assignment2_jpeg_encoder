#include "JpegIO.hpp"
#include "JpegZigzag.hpp"

extern "C" {
#include "../3rdparty/bitstr.h"
}
#include <stdexcept>

bool JpegIO::writeToFile(const char* dst_file, 
                         const char* buffer, 
                         long dataLength,
                         const int* quant_tab[2], 
                         const uint8_t* huf_ac_tab[2],
                         const uint8_t* huf_dc_tab[2],  
                         const int w, const int h, 
                         YUVFormat format) {
    FILE *fp = fopen(dst_file, "wb");

    // SOI
    fputc(0xff, fp);
    fputc(0xd8, fp);
     

    // DQT
    for (int i = 0; i < 2; i++) {
        int len = 2 + 1 + 64;
        fputc(0xff, fp);
        fputc(0xdb, fp);
        fputc(len >> 8, fp);
        fputc(len >> 0, fp);
        fputc(i, fp);
        for (int j = 0; j < 64; j++) {
            fputc(quant_tab[i][JpegZigzag::ZIGZAG_INDEX[j]], fp);
        }
    }

    // SOF0
    int SOF0Len = 2 + 1 + 2 + 2 + 1 + 3 * 3;
    fputc(0xff, fp);
    fputc(0xc0, fp);
    fputc(SOF0Len >> 8, fp);
    fputc(SOF0Len >> 0, fp);
    fputc(8, fp); // precision 8bit
    fputc(h >> 8, fp); // height
    fputc(h >> 0, fp); // height
    fputc(w >> 8, fp); // width
    fputc(w >> 0, fp); // width
    fputc(3, fp); 

    // Y, U, V 
    unsigned char chrom[] = {0x01, 0x11, 0x00, 
                             0x02, 0x11, 0x01,
                             0x03, 0x11, 0x01
                            };
    if (format == YUVFormat::YUV444) {
        chrom[1] = 0x11; chrom[4] = 0x11; chrom[7] = 0x11;
    } else if(format == YUVFormat::YUV420) {
        chrom[1] = 0x22; chrom[4] = 0x11; chrom[7] = 0x11;
    } else if(format == YUVFormat::YUV422) {
        chrom[1] = 0x21; chrom[4] = 0x11; chrom[7] = 0x11;
    } else {
        throw std::runtime_error("unsupported yuv format!");
    }
    for(int i = 0; i < 9; ++i) fputc(chrom[i], fp);

    // DHT AC
    for (int i = 0; i < 2; i++) {
        fputc(0xff, fp);
        fputc(0xc4, fp);
        int len = 2 + 1 + 16;
        for (int j = 0; j < 16; j++) {
            len += huf_ac_tab[i][j];
        }
        fputc(len >> 8, fp);
        fputc(len >> 0, fp);
        fputc(i + 0x10, fp);
        fwrite(huf_ac_tab[i], len - 3, 1, fp);
    }
    // DHT DC
    for (int i = 0; i < 2; i++) {
        fputc(0xff, fp);
        fputc(0xc4, fp);
        int len = 2 + 1 + 16;
        for (int j = 0; j < 16; j++) {
            len += huf_dc_tab[i][j];
        }
        fputc(len >> 8, fp);
        fputc(len >> 0, fp);
        fputc(i + 0x00, fp);
        fwrite(huf_dc_tab[i], len - 3, 1, fp);
    }

    // SOS
    int SOSLen = 2 + 1 + 2 * 3 + 3;
    fputc(0xff, fp);
    fputc(0xda, fp);
    fputc(SOSLen >> 8, fp);
    fputc(SOSLen >> 0, fp);
    fputc(3, fp);

    fputc(0x01, fp); fputc(0x00, fp);
    fputc(0x02, fp); fputc(0x11, fp);
    fputc(0x03, fp); fputc(0x11, fp);

    fputc(0x00, fp);
    fputc(0x3F, fp);
    fputc(0x00, fp);

    // data
    fwrite(buffer, dataLength, 1, fp);

    // EOI
    fputc(0xff, fp);
    fputc(0xd9, fp);

    fflush(fp);
    fclose(fp);

    return true;
}
