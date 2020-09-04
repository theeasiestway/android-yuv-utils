//
// // Created by Loboda Alexey on 30.07.2020.
//

#ifndef LIBYUVWRAPPER_RGBFRAME_H
#define LIBYUVWRAPPER_RGBFRAME_H

#include <vector>
#include <string>
#include "Frame.h"

class RgbFrame: public Frame {

public:
    RgbFrame(int width, int height, int dataSize, int stride, int type);
    RgbFrame(int width, int height, uint8_t &data, int dataSize, int stride, int type);

    int type;
    int dataStride;

    void update(RgbFrame &other);

    ~RgbFrame();
};

#endif //LIBYUVWRAPPER_RGBFRAME_H