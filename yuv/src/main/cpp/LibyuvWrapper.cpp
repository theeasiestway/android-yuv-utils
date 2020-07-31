//
// Created by Loboda Alexey on 30.07.2020.
//

#include <libyuv.h>
#include "LibyuvWrapper.h"
#include "factories/FramesFactory.h"

YuvFrame* LibyuvWrapper::to420(uint8_t* y, int yStride, uint8_t* u, int uStride, uint8_t* v, int vStride, int uvPixelStride, int width, int height) {
    YuvFrame *yuvFrame = instanceYuv(width, height);

    libyuv::Android420ToI420(y, yStride,
                             u, uStride,
                             v, vStride,
                             uvPixelStride,
                             yuvFrame->y.data(), yuvFrame->yStride,
                             yuvFrame->u.data(), yuvFrame->uStride,
                             yuvFrame->v.data(), yuvFrame->vStride,
                             width, height);

    return yuvFrame;
}

void LibyuvWrapper::scale(YuvFrame& in, int scaleWidth, int scaleHeight, int scaleFilter) {
    if (scaleWidth > 0 && scaleHeight > 0) {
        YuvFrame* temp = instanceYuv(scaleWidth, scaleHeight);

        libyuv::I420Scale(in.y.data(), in.yStride,
                          in.u.data(), in.uStride,
                          in.v.data(), in.vStride,
                          in.width, in.height,
                          temp->y.data(), temp->yStride,
                          temp->u.data(), temp->uStride,
                          temp->v.data(), temp->vStride,
                          scaleWidth, scaleHeight, libyuv::FilterMode(scaleFilter));

        in.update(*temp);
        delete temp;
    }
}

void LibyuvWrapper::rotate(YuvFrame &in, int rotationMode) {
    if (rotationMode > 0) {
        YuvFrame *temp = instanceYuv(in.width, in.height, rotationMode);
        libyuv::I420Rotate(in.y.data(), in.yStride,
                           in.u.data(), in.uStride,
                           in.v.data(), in.vStride,
                           temp->y.data(), temp->yStride,
                           temp->u.data(), temp->uStride,
                           temp->v.data(), temp->vStride,
                           in.width, in.height, libyuv::RotationMode(rotationMode));

        in.update(*temp);
        delete temp;
    }
}

void LibyuvWrapper::mirrorH(YuvFrame &in) {
    YuvFrame *temp = instanceYuv(in.width, in.height);
    libyuv::I420Mirror(in.y.data(), in.yStride,
                       in.u.data(), in.uStride,
                       in.v.data(), in.vStride,
                       temp->y.data(), temp->yStride,
                       temp->u.data(), temp->uStride,
                       temp->v.data(), temp->vStride,
                       in.width, in.height);

    in.update(*temp);
    delete temp;
}

void LibyuvWrapper::mirrorV(YuvFrame &in) {
    YuvFrame *temp = instanceYuv(in.width, in.height, 0);
    libyuv::I420Rotate(in.y.data(), in.yStride,
                       in.u.data(), in.uStride,
                       in.v.data(), in.vStride,
                       temp->y.data(), temp->yStride,
                       temp->u.data(), temp->uStride,
                       temp->v.data(), temp->vStride,
                       in.width, -in.height, libyuv::RotationMode(0));

    in.update(*temp);
    delete temp;
}

RgbFrame* LibyuvWrapper::toArgbFrame(YuvFrame &in) {

    RgbFrame *temp = instanceArgb(in.width, in.height);

    libyuv::I420ToARGB(in.y.data(), in.yStride,
                       in.u.data(), in.vStride, // exactly this order "YVU" and not "YUV", otherwise the colors are inverted
                       in.v.data(), in.uStride,
                       temp->data.data(), temp->stride,
                       in.width, in.height);
    delete &in;
    return temp;
}

RgbFrame* LibyuvWrapper::toRgb565Frame(YuvFrame &in) {
    RgbFrame *temp = instanceRgb565(in.width, in.height);

    libyuv::I420ToRGB565(in.y.data(), in.yStride,
                         in.u.data(), in.vStride, // exactly this order "YVU" and not "YUV", otherwise the colors are inverted
                         in.v.data(), in.uStride,
                         temp->data.data(), temp->stride,
                         in.width, in.height);

    delete &in;
    return temp;
}
