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
    std::vector<uint8_t>().swap(y);
    std::vector<uint8_t>().swap(u);
    std::vector<uint8_t>().swap(v);
    y.reserve(ySize);
    u.reserve(uSize);
    v.reserve(vSize);
}

void YuvFrame::update(YuvFrame& other) {
    width = other.width;
    height = other.height;
    yStride = other.yStride;
    uStride = other.uStride;
    vStride = other.vStride;
    std::vector<uint8_t>().swap(y);
    std::vector<uint8_t>().swap(u);
    std::vector<uint8_t>().swap(v);
    y = std::move(other.y);
    u = std::move(other.u);
    v = std::move(other.v);
}

YuvFrame* YuvFrame::fromPointer(long ptr) {
    return (YuvFrame *) ptr;
}

long YuvFrame::getPointer(YuvFrame &yuvFrame) {
    return (long) &yuvFrame;
}

YuvFrame::~YuvFrame() {
    std::vector<uint8_t>().swap(y);
    std::vector<uint8_t>().swap(u);
    std::vector<uint8_t>().swap(v);
}