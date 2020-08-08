//
// Created by Loboda Alexey on 30.07.2020.
//

#include <libyuv.h>
#include "LibyuvWrapper.h"
#include "factories/FramesFactory.h"
#include "utils/logger.h"
#include "utils/TimeCounter.h"

YuvFrame* LibyuvWrapper::to420(uint8_t* y, int yStride, uint8_t* u, int uStride, uint8_t* v, int vStride, int uvPixelStride, int width, int height) {
    TimeCounter::setTime();

    YuvFrame *yuvFrame = instanceYuv(width, height);

    TimeCounter::printTime("[to420()] instanceYuv()     ", true);

    libyuv::Android420ToI420(y, yStride,
                             u, uStride,
                             v, vStride,
                             uvPixelStride,
                             yuvFrame->y, yuvFrame->yStride,
                             yuvFrame->u, yuvFrame->uStride,
                             yuvFrame->v, yuvFrame->vStride,
                             width, height);

    TimeCounter::printTime("[to420()] Android420ToI420()");

    return yuvFrame;
}

void LibyuvWrapper::scale(YuvFrame& in, int scaleWidth, int scaleHeight, int scaleFilter) {
    if (scaleWidth > 0 && scaleHeight > 0) {

        TimeCounter::setTime();

        YuvFrame* temp = instanceYuv(scaleWidth, scaleHeight);

        TimeCounter::printTime("[scale] instanceYuv()", true);

        libyuv::I420Scale(in.y, in.yStride,
                          in.u, in.uStride,
                          in.v, in.vStride,
                          in.width, in.height,
                          temp->y, temp->yStride,
                          temp->u, temp->uStride,
                          temp->v, temp->vStride,
                          scaleWidth, scaleHeight, libyuv::FilterMode(scaleFilter));

        TimeCounter::printTime("[scale] I420Scale()  ", true);

        in.update(*temp);

        TimeCounter::printTime("[scale] update()     ", true);

        delete temp;

        TimeCounter::printTime("[scale] delete temp  ");
    }
}

void LibyuvWrapper::rotate(YuvFrame &in, int rotationMode) {
    if (rotationMode >= 0) {

        TimeCounter::setTime();

        YuvFrame *temp = instanceYuv(in.width, in.height, rotationMode);

        TimeCounter::printTime("[rotate] instanceYuv()", true);

        libyuv::I420Rotate(in.y, in.yStride,
                           in.u, in.uStride,
                           in.v, in.vStride,
                           temp->y, temp->yStride,
                           temp->u, temp->uStride,
                           temp->v, temp->vStride,
                           in.width, in.height, libyuv::RotationMode(rotationMode));

        TimeCounter::printTime("[rotate] I420Rotate() ", true);

        in.update(*temp);

        TimeCounter::printTime("[rotate] update       ", true);

        delete temp;

        TimeCounter::printTime("[rotate] delete temp  ", true);
    }
}

void LibyuvWrapper::mirrorH(YuvFrame &in) {

    TimeCounter::setTime();

    YuvFrame *temp = instanceYuv(in.width, in.height);

    TimeCounter::printTime("[mirrorH] instanceYuv()", true);

    libyuv::I420Mirror(in.y, in.yStride,
                       in.u, in.uStride,
                       in.v, in.vStride,
                       temp->y, temp->yStride,
                       temp->u, temp->uStride,
                       temp->v, temp->vStride,
                       in.width, in.height);

    TimeCounter::printTime("[mirrorH] I420Mirror() ", true);

    in.update(*temp);

    TimeCounter::printTime("[mirrorH] update       ", true);

    delete temp;

    TimeCounter::printTime("[mirrorH] delete temp  ", true);
}

void LibyuvWrapper::mirrorV(YuvFrame &in) {

    TimeCounter::setTime();

    YuvFrame *temp = instanceYuv(in.width, in.height, 0);

    TimeCounter::printTime("[mirrorV] instanceYuv()", true);

    libyuv::I420Rotate(in.y, in.yStride,
                       in.u, in.uStride,
                       in.v, in.vStride,
                       temp->y, temp->yStride,
                       temp->u, temp->uStride,
                       temp->v, temp->vStride,
                       in.width, -in.height, libyuv::RotationMode(0));

    TimeCounter::printTime("[mirrorV] I420Rotate() ", true);

    in.update(*temp);

    TimeCounter::printTime("[mirrorV] update       ", true);

    delete temp;

    TimeCounter::printTime("[mirrorV] delete temp  ", true);
}

RgbFrame* LibyuvWrapper::toArgbFrame(YuvFrame &in) {

    TimeCounter::setTime();

    RgbFrame *temp = instanceArgb(in.width, in.height);

    TimeCounter::printTime("[toArgbFrame] instanceArgb()", true);

    libyuv::I420ToARGB(in.y, in.yStride,
                       in.v, in.vStride, // exactly this order "YVU" and not "YUV", otherwise the colors are inverted
                       in.u, in.uStride,
                       temp->data, temp->stride,
                       in.width, in.height);

    TimeCounter::printTime("[toArgbFrame] I420ToARGB()  ", true);

    delete &in;

    TimeCounter::printTime("[toArgbFrame] delete &in    ", true);

    return temp;
}

RgbFrame* LibyuvWrapper::toRgb565Frame(YuvFrame &in) {

    TimeCounter::setTime();

    RgbFrame *temp = instanceRgb565(in.width, in.height);

    TimeCounter::printTime("[toRgb565Frame] instanceRgb565()", true);

    libyuv::I420ToRGB565(in.y, in.yStride,
                         in.u, in.uStride,
                         in.v, in.vStride,
                         temp->data, temp->stride,
                         in.width, in.height);

    TimeCounter::printTime("[toRgb565Frame] I420ToRGB565()  ", true);

    delete &in;

    TimeCounter::printTime("[toRgb565Frame] delete &in      ", true);

    return temp;
}
