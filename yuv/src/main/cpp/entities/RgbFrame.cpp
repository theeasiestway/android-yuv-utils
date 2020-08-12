//
// Created by Loboda Alexey on 30.07.2020.
//

#include "RgbFrame.h"

RgbFrame::RgbFrame(int width, int height, int stride, int dataSize, int type) {
    this->width = width;
    this->height = height;
    this->dataStride = stride;
    this->dataSize = dataSize;
    this->type = type;
    data = (uint8_t*) malloc(sizeof(uint8_t) * dataSize);
}

RgbFrame::RgbFrame(int width, int height, uint8_t *data, int dataSize, int stride, int type) {
    this->width = width;
    this->height = height;
    this->dataStride = stride;
    this->dataSize = dataSize;
    this->type = type;
    this->data = data;
}

void RgbFrame::update(RgbFrame *other) {
    this->width = other->width;
    this->height = other->height;
    this->dataStride = other->dataStride;
    this->dataSize = other->dataSize;
    this->type = other->type;
    Frame::release();
    data = (uint8_t*) malloc(sizeof(uint8_t) * dataSize);
    memmove(data, other->data, sizeof(uint8_t) * dataSize);
}