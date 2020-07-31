//
// Created by Loboda Alexey on 30.07.2020.
//

#ifndef LIBYUVWRAPPER_LIBYUVWRAPPER_H
#define LIBYUVWRAPPER_LIBYUVWRAPPER_H

#include "entities/YuvFrame.h"
#include "entities/RgbFrame.h"

class LibyuvWrapper {

public:

    static const int RETURN_YUV = 0;
    static const int RETURN_I420 = 1;
    static const int RETURN_ARGB = 2;
    static const int RETURN_RGB565 = 3;

    static YuvFrame& to420(uint8_t* y, int yStride, uint8_t* u, int uStride, uint8_t* v, int vStride, int uvPixelStride, int width, int height);

    static YuvFrame& scale(YuvFrame& in, int scaleWidth, int scaleHeight, int scaleFilter);

    static YuvFrame& rotate(YuvFrame& in, int rotationMode);

    static YuvFrame& mirrorH(YuvFrame& in);

    static YuvFrame& mirrorV(YuvFrame& in);

    static RgbFrame& toArgbFrame(YuvFrame& in);

    static RgbFrame& toRgb565Frame(YuvFrame& in);
};


#endif //LIBYUVWRAPPER_LIBYUVWRAPPER_H
