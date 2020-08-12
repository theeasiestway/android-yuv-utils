//
// Created by Loboda Alexey on 30.07.2020.
//

#include <libyuv.h>
#include "LibyuvWrapper.h"
#include "factories/FramesFactory.h"

YuvFrame* LibyuvWrapper::to420(uint8_t* y, int yStride, uint8_t* u, int uStride, uint8_t* v, int vStride, int uvPixelStride, int width, int height) {
    YuvFrame *yuvFrame = FramesFactory::instanceYuv(width, height);
    libyuv::Android420ToI420(y, yStride,
                             u, uStride,
                             v, vStride,
                             uvPixelStride,
                             yuvFrame->y, yuvFrame->yStride,
                             yuvFrame->u, yuvFrame->uStride,
                             yuvFrame->v, yuvFrame->vStride,
                             width, height);
    yuvFrame->fillData();
    return yuvFrame;
}

YuvFrame* LibyuvWrapper::to420(RgbFrame &in) {
    YuvFrame *yuvFrame = FramesFactory::instanceYuv(in.width, in.height);
    if (in.type == ARGB) {
        libyuv::ARGBToI420(in.data, in.dataStride,
                           yuvFrame->y, yuvFrame->yStride,
                           yuvFrame->v, yuvFrame->vStride,
                           yuvFrame->u, yuvFrame->uStride,
                           in.width, in.height);
    }

    if (in.type == RGB565) {
        libyuv::RGB565ToI420(in.data, in.dataStride,
                             yuvFrame->y, yuvFrame->yStride,
                             yuvFrame->u, yuvFrame->uStride,
                             yuvFrame->v, yuvFrame->vStride,
                             in.width, in.height);
    }
    yuvFrame->fillData();
    return yuvFrame;
}

void LibyuvWrapper::scale(YuvFrame& in, int scaleWidth, int scaleHeight, int scaleFilter) {
    if (scaleWidth > 0 && scaleHeight > 0) {
        YuvFrame* temp = FramesFactory::instanceYuv(scaleWidth, scaleHeight);
        libyuv::I420Scale(in.y, in.yStride,
                          in.u, in.uStride,
                          in.v, in.vStride,
                          in.width, in.height,
                          temp->y, temp->yStride,
                          temp->u, temp->uStride,
                          temp->v, temp->vStride,
                          scaleWidth, scaleHeight, libyuv::FilterMode(scaleFilter));
        in.update(*temp);
        delete temp;
    }
}

void LibyuvWrapper::rotate(YuvFrame &in, int rotationMode) {
    if (rotationMode >= 0) {
        YuvFrame *temp = FramesFactory::instanceYuv(in.width, in.height, rotationMode);
        libyuv::I420Rotate(in.y, in.yStride,
                           in.u, in.uStride,
                           in.v, in.vStride,
                           temp->y, temp->yStride,
                           temp->u, temp->uStride,
                           temp->v, temp->vStride,
                           in.width, in.height, libyuv::RotationMode(rotationMode));
        in.update(*temp);
        delete temp;
    }
}

void LibyuvWrapper::mirrorH(YuvFrame &in) {
    YuvFrame *temp = FramesFactory::instanceYuv(in.width, in.height);
    libyuv::I420Mirror(in.y, in.yStride,
                       in.u, in.uStride,
                       in.v, in.vStride,
                       temp->y, temp->yStride,
                       temp->u, temp->uStride,
                       temp->v, temp->vStride,
                       in.width, in.height);
    in.update(*temp);
    delete temp;
}

void LibyuvWrapper::mirrorV(YuvFrame &in) {
    YuvFrame *temp = FramesFactory::instanceYuv(in.width, in.height);
    libyuv::I420Rotate(in.y, in.yStride,
                       in.u, in.uStride,
                       in.v, in.vStride,
                       temp->y, temp->yStride,
                       temp->u, temp->uStride,
                       temp->v, temp->vStride,
                       in.width, -in.height, libyuv::RotationMode(0));
    in.update(*temp);
    delete temp;
}

RgbFrame* LibyuvWrapper::toArgbFrame(YuvFrame &in) {
    RgbFrame *temp = FramesFactory::instanceArgb(in.width, in.height);
    libyuv::I420ToARGB(in.y, in.yStride,
                       in.v, in.vStride, // exactly this order "YVU" and not "YUV", otherwise the colors are inverted
                       in.u, in.uStride,
                       temp->data, temp->dataStride,
                       in.width, in.height);
    delete &in;
    return temp;
}

RgbFrame* LibyuvWrapper::toRgb565Frame(YuvFrame &in) {
    RgbFrame *temp = FramesFactory::instanceRgb565(in.width, in.height);
    libyuv::I420ToRGB565(in.y, in.yStride,
                         in.u, in.uStride,
                         in.v, in.vStride,
                         temp->data, temp->dataStride,
                         in.width, in.height);
    delete &in;
    return temp;
}

void LibyuvWrapper::toArgbFrame(RgbFrame &in) {
    if (in.type == RGB565) {
        RgbFrame *temp = FramesFactory::instanceArgb(in.width, in.height);
        libyuv::RGB565ToARGB(in.data, in.dataStride,
                             temp->data, temp->dataStride,
                             in.width, in.height);
        in.update(temp);
        delete temp;
    }
}

void LibyuvWrapper::toRgb565Frame(RgbFrame &in) {
    if (in.type == ARGB) {
        RgbFrame *temp = FramesFactory::instanceRgb565(in.width, in.height);
        libyuv::ARGBToRGB565(in.data, in.dataStride,
                             temp->data, temp->dataStride,
                             in.width, in.height);
        in.update(temp);
        delete temp;
    }
}
