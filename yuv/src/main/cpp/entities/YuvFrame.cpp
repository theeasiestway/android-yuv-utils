//
// Created by Loboda Alexey on 30.07.2020.
//

#include "YuvFrame.h"

YuvFrame::YuvFrame(int width, int height, int yStride, int uStride, int vStride, int ySize, int uSize, int vSize) {
    this->width = width;
    this->height = height;
    this->yStride = yStride;
    this->uStride = uStride;
    this->vStride = vStride;
    this->ySize = ySize;
    this->uSize = uSize;
    this->vSize = vSize;
    dataSize = ySize + uSize + vSize;

    y = (uint8_t*) malloc(sizeof(uint8_t) * ySize);
    u = (uint8_t*) malloc(sizeof(uint8_t) * uSize);
    v = (uint8_t*) malloc(sizeof(uint8_t) * vSize);
}

void YuvFrame::update(YuvFrame& other) {
    width = other.width;
    height = other.height;

    yStride = other.yStride;
    uStride = other.uStride;
    vStride = other.vStride;

    ySize = other.ySize;
    uSize = other.uSize;
    vSize = other.vSize;
    dataSize = other.dataSize;

    release();

    y = (uint8_t*) malloc(sizeof(uint8_t) * ySize);
    u = (uint8_t*) malloc(sizeof(uint8_t) * uSize);
    v = (uint8_t*) malloc(sizeof(uint8_t) * vSize);

    memmove(y, other.y, sizeof(uint8_t) * ySize);
    memmove(u, other.u, sizeof(uint8_t) * uSize);
    memmove(v, other.v, sizeof(uint8_t) * vSize);

    fillData();
}

void YuvFrame::fillData() {
    data = (uint8_t*) malloc(sizeof(uint8_t) * dataSize);
    memmove(data, y, sizeof(uint8_t) * ySize);
    memmove(&data[ySize], &u[0], sizeof(uint8_t) * uSize);
    memmove(&data[ySize + uSize], &v[0], sizeof(uint8_t) * vSize);
}

YuvFrame::~YuvFrame() {
    release();
}

void YuvFrame::release() {
    Frame::release();
    if (y) free(y);
    if (u) free(u);
    if (v) free(v);
    y = nullptr;
    u = nullptr;
    v = nullptr;
}