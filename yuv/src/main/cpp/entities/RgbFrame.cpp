//
// Created by Loboda Alexey on 30.07.2020.
//

#include "RgbFrame.h"

RgbFrame::RgbFrame(int width, int height, int stride, int dataSize, int type) {
    this->width = width;
    this->height = height;
    this->stride = stride;
    this->dataSize = dataSize;
    this->type = type;
    data = (uint8_t*) malloc(sizeof(uint8_t) * dataSize);
}

RgbFrame::~RgbFrame() { /* nothing to do yet */ }
