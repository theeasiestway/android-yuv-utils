//
// Created by Loboda Alexey on 29.07.2020.
//

#include "FramesFactory.h"
#include "../entities/YuvFrame.h"
#include "../entities/RgbFrame.h"
#include "../LibyuvWrapper.h"

YuvFrame* FramesFactory::instanceYuv(int width, int height) {
    int ySize = width * height;
    int uvSize = ySize / 4;
    int extra = width % 2 == 0 ? 0 : 1;
    return new YuvFrame(width, height, width, width / 2 + extra, width / 2 + extra, ySize, uvSize, uvSize);
}

YuvFrame* FramesFactory::instanceYuv(int width, int height, int rotation) {
    int outWidth = (rotation == 90 || rotation == 270) ? height : width;
    int outHeight = (rotation == 90 || rotation == 270) ? width : height;
    int ySize = outWidth * outHeight;
    int uvSize = outWidth * outHeight / 4;
    int extra = outWidth % 2 == 0 ? 0 : 1;
    return new YuvFrame(outWidth, outHeight, outWidth, outWidth / 2 + extra, outWidth / 2 + extra, ySize, uvSize, uvSize);
}

RgbFrame* FramesFactory::instanceArgb(int width, int height) {
    int dataSize = (width * height) * 4; // 4 bytes: (8 + 8 + 8 + 8)bits / 8 bits
    int extra = width % 2 == 0 ? 0 : 1;
    int stride = width * 4 + extra;
    return new RgbFrame(width, height, dataSize, stride, LibyuvWrapper::ARGB);
}

RgbFrame* FramesFactory::instanceRgb565(int width, int height) {
    int dataSize = (width * height) * 2; // 2 bytes: (5 + 6 + 5)bits / 8 bits
    int extra = width % 2 == 0 ? 0 : 1;
    int stride = width * 2 + extra;
    return new RgbFrame(width, height, dataSize, stride, LibyuvWrapper::RGB565);
}