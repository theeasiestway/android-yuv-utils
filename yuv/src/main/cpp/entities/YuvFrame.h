//
// Created by Loboda Alexey on 30.07.2020.
//

#ifndef LIBYUVWRAPPER_YUVFRAME_H
#define LIBYUVWRAPPER_YUVFRAME_H

#include <vector>
#include "Frame.h"

class YuvFrame: public Frame {

public:

    int yStride;
    int uStride;
    int vStride;
    std::vector<uint8_t> y;
    std::vector<uint8_t> u;
    std::vector<uint8_t> v;

    YuvFrame(int width, int height, int yStride, int uStride, int vStride, int ySize, int uSize, int vSize);

    void update(YuvFrame& other);

    long getPointer(YuvFrame &frame);

    static YuvFrame* fromPointer(long ptr);

    std::vector<uint8_t> getBytes() override;

    ~YuvFrame();
};


#endif //LIBYUVWRAPPER_YUVFRAME_H
