//
// // Created by Loboda Alexey on 30.07.2020.
//

#ifndef LIBYUVWRAPPER_RGBFRAME_H
#define LIBYUVWRAPPER_RGBFRAME_H

#include <vector>

class RgbFrame {

public:

    RgbFrame(int width, int height, int stride, int dataSize);

    int width;
    int height;
    int stride;
    std::vector<uint8_t> data;

    RgbFrame* fromPointer(long ptr);

    long getPointer(RgbFrame& rgbFrame);

    ~RgbFrame();
};


#endif //LIBYUVWRAPPER_RGBFRAME_H
