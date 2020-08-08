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
    uint8_t* y = nullptr;
    int ySize;
    uint8_t* u = nullptr;
    int uSize;
    uint8_t* v = nullptr;
    int vSize;

    YuvFrame(int width, int height, int yStride, int uStride, int vStride, int ySize, int uSize, int vSize);

    void fillData();

    void update(YuvFrame& other);

    ~YuvFrame();
};


#endif //LIBYUVWRAPPER_YUVFRAME_H
