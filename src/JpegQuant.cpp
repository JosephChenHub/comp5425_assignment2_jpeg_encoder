// ref. :  https://github.com/libjpeg-turbo/libjpeg-turbo/blob/main/jcparam.c

#include "JpegQuant.hpp"

/* These are the sample quantization tables given in Annex K (Clause K.1) of
 * Recommendation ITU-T T.81 (1992) | ISO/IEC 10918-1:1994.
 * The spec says that the values given produce "good" quality, and
 * when divided by 2, "very good" quality.
 */
const int STD_QUANT_TAB_LUMIN[64] = {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109, 103, 77,
        24, 35, 55, 64, 81, 104, 113, 92,
        49, 64, 78, 87, 103, 121, 120, 101,
        72, 92, 95, 98, 112, 100, 103, 99,
};
const int STD_QUANT_TAB_CHROM[64] = {
        17, 18, 24, 47, 99, 99, 99, 99,
        18, 21, 26, 66, 99, 99, 99, 99,
        24, 26, 56, 99, 99, 99, 99, 99,
        47, 66, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
};

JpegQuant::JpegQuant(const int quality, const bool force_baseline) {
   this->quality = quality;
   this->qtable_lumin = scaledQuality(this->quality, true, force_baseline);
   this->qtable_chrom = scaledQuality(this->quality, false, force_baseline);
}

JpegQuant::~JpegQuant() {

}

void JpegQuant::setQuality(int quality, const bool force_baseline) {
    if (quality == this->quality && force_baseline == this->force_baseline) return;
    this->qtable_lumin = scaledQuality(quality, true, force_baseline); 
    this->qtable_chrom = scaledQuality(quality, false, force_baseline);
}

std::vector<int> JpegQuant::scaledQuality(int quality, const bool luminance, const bool force_baseline) {
    std::vector<int> new_qtable(64);
    // quality range [1, 100]
    if (quality <= 0) quality = 1;
    if (quality > 100) quality = 100;

    /* The basic table is used as-is (scaling 100) for a quality of 50.
    * Qualities 50..100 are converted to scaling percentage 200 - 2*Q;
    * note that at Q=100 the scaling is 0, which will cause jpeg_add_quant_table
    * to make all the table entries 1 (hence, minimum quantization loss).
    * Qualities 1..50 are converted to scaling percentage 5000/Q.
    */    
    int scale_factor;
    if (quality < 50) {
       scale_factor = 5000 / quality;
    } else {
       scale_factor = 200 - 2 * quality;
    } 
    for(int i = 0; i < 64; ++i) {
        long temp;
        if (luminance) {
           temp = ((long)STD_QUANT_TAB_LUMIN[i] * scale_factor + 50L) / 100L;
        } else {
           temp = ((long)STD_QUANT_TAB_CHROM[i] * scale_factor + 50L) / 100L;
        } 
        if (temp <= 0) temp = 1L;
        if (temp > 32767L) temp = 32767L; // max quantizer needed for 12 bits
        if (force_baseline && temp > 255L) temp = 255L; // limit to 255 if requested      
        new_qtable[i] = temp;
    }
    return new_qtable;
}

void JpegQuant::quantEncode8x8(int *data8x8, const bool luminance) {
    for (int i = 0; i < 64; i++) {
        int temp = luminance ? this->qtable_lumin[i] : this->qtable_chrom[i];
        data8x8[i] /= temp;
    }
}
