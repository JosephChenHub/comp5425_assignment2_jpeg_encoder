#pragma once

#include <vector>

class JpegQuant {
public:
    JpegQuant(const int quality, const bool force_baseline);
    ~JpegQuant();

    void quantEncode8x8(int* data8x8, const bool luminance);
    void setQuality(int quality, const bool force_baseline);

public:
    int quality; // quality range [1, 100]
    int force_baseline; // if true, the maximum of the quantization table is limit to 255
    std::vector<int> qtable_lumin;
    std::vector<int> qtable_chrom; 

private:
    std::vector<int> scaledQuality(int quality, const bool luminance, const bool force_baseline);
};


