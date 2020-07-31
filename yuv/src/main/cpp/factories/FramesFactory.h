//
// Created by Loboda Alexey on 29.07.2020.
//

#ifndef LIBYUVWRAPPER_FRAMESFACTORY_H
#define LIBYUVWRAPPER_FRAMESFACTORY_H

#include "../entities/YuvFrame.h"
#include "../entities/RgbFrame.h"

YuvFrame* instanceYuv(int width, int height);
YuvFrame* instanceYuv(int width, int height, int rotation);

RgbFrame* instanceArgb(int width, int height);
RgbFrame* instanceRgb565(int width, int height);

#endif //LIBYUVWRAPPER_FRAMESFACTORY_H
