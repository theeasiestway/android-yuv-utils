//
// Created by Loboda Alexey on 30.07.2020.
//

#include "RgbFrame.h"

RgbFrame::RgbFrame(int width, int height, int stride, int dataSize) {
    this->width = width;
    this->height = height;
    this->stride = stride;
    std::vector<uint8_t>().swap(data);
    data.resize(dataSize);
}

RgbFrame* RgbFrame::fromPointer(long ptr) {
    return (RgbFrame*) ptr;
}

long RgbFrame::getPointer(RgbFrame &rgbFrame) {
    return (long) &rgbFrame;
}

RgbFrame::~RgbFrame() { /* nothing to do yet */ }
