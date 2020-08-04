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

    RgbFrame(int width, int height, int stride, int dataSize);

    int width;
    int height;
    int stride;

    static RgbFrame* fromPointer(long ptr);

    long getPointer(RgbFrame &frame);

    ~RgbFrame();
};


#endif //LIBYUVWRAPPER_RGBFRAME_H
