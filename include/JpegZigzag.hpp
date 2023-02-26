#pragma once

class JpegZigzag {
public:
    JpegZigzag()=default;
    ~JpegZigzag()=default;

    void zigzag(int* const data8x8);

public:
    static const int ZIGZAG_INDEX[64]; 
};
