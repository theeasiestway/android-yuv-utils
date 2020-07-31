//
// Created by Loboda Alexey on 29.07.2020.
//

#include <vector>
#include "FramesFactory.h"
#include "../entities/YuvFrame.h"
#include "../entities/RgbFrame.h"

YuvFrame* instanceYuv(int width, int height) {
    int ySize = width * height;
    int uvSize = ySize / 4;
    int extra = width % 2 == 0 ? 0 : 1;
    return new YuvFrame(width, height, width, width / 2 + extra, width / 2 + extra, ySize, uvSize, uvSize);
}

YuvFrame* instanceYuv(int width, int height, int rotation) {
    int outWidth = (rotation == 90 || rotation == 270) ? height : width;
    int outHeight = (rotation == 90 || rotation == 270) ? width : height;
    int ySize = outWidth * outHeight;
    int uvSize = outWidth * outHeight / 4;
    int extra = width % 2 == 0 ? 0 : 1;
    return new YuvFrame(width, height, width, width / 2 + extra, width / 2 + extra, ySize, uvSize, uvSize);
}

RgbFrame* instanceArgb(int width, int height) {
    int dataSize = (width * height) * 32;
    int extra = width % 2 == 0 ? 0 : 1;
    int stride = width * 4 + extra;
    return new RgbFrame(width, height, stride, dataSize);
}

RgbFrame* instanceRgb565(int width, int height) {
    int dataSize = (width * height) * 16;
    int extra = width % 2 == 0 ? 0 : 1;
    int stride = width * 4 + extra;
    return new RgbFrame(width, height, stride, dataSize);
}