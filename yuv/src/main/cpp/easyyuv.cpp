#include <jni.h>
#include <string>
#include <vector>
#include <libyuv/scale.h>
#include "libyuv.h"
#include "utils/logger.h"
#include "entities/rgbFrame.h"
#include "entities/yuvFrame.h"
#include "factories/FramesFactory.h"
#include "factories/EntitiesFactory.h"
#include "LibyuvWrapper.h"

//
// Created by Loboda Alexey on 22.06.2020.
//

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_YuvUtils_transformNative(JNIEnv *env, jobject thiz,
                                                    jobject y, jobject u, jobject v,
                                                    jint yStride, jint uStride, jint vStride, jint uvPixelStride,
                                                    jint width, jint height,
                                                    jint scaleWidth, jint scaleHeight, jint scaleFilter,
                                                    jint rotationMode, jboolean mirrorH,
                                                    jboolean mirrorV, jint returnType) {

    //
    // to I420
    //

    YuvFrame *yuvFrame = LibyuvWrapper::to420((uint8_t *) env->GetDirectBufferAddress(y), yStride,
                                              (uint8_t *) env->GetDirectBufferAddress(u), uStride,
                                              (uint8_t *) env->GetDirectBufferAddress(v), vStride,
                                              uvPixelStride, width, height);

    //
    // Scale
    //

    if (scaleWidth > 0 && scaleHeight > 0) LibyuvWrapper::scale(*yuvFrame, scaleWidth, scaleHeight, scaleFilter);

    //
    // Rotate
    //

    if (rotationMode > 0) LibyuvWrapper::rotate(*yuvFrame, rotationMode);

    //
    // MirrorH
    //

    if (mirrorH) LibyuvWrapper::mirrorH(*yuvFrame);

    //
    // MirrorV
    //

    if (mirrorV) LibyuvWrapper::mirrorV(*yuvFrame);

    //
    // to ARGB
    //

    if (returnType == LibyuvWrapper::ARGB) {
        RgbFrame *rgbFrame = LibyuvWrapper::toArgbFrame(*yuvFrame);
        return EntitiesFactory::instanceArgb(*rgbFrame, *env);
    }

    //
    // to RGB565
    //

    if (returnType == LibyuvWrapper::RGB565) {
        RgbFrame *rgbFrame = LibyuvWrapper::toArgbFrame(*yuvFrame);
        return EntitiesFactory::instanceRgb565(*rgbFrame, *env);
    }

    return EntitiesFactory::instanceYuv(*yuvFrame, *env);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_yuv_entities_Frame_destroy(JNIEnv *env, jobject thiz, jlong pointer, jint classType) {
    switch (classType) {
        case LibyuvWrapper::ARGB: {
            RgbFrame *argbFrame = RgbFrame::fromPointer((long) pointer);
            delete argbFrame;
            argbFrame = nullptr;
            break;
        }
        case LibyuvWrapper::RGB565: {
            RgbFrame *rgb565Frame = RgbFrame::fromPointer((long) pointer);
            delete rgb565Frame;
            rgb565Frame = nullptr;
            break;
        }
        case LibyuvWrapper::YUV: {
            default:
                YuvFrame *yuvFrame = YuvFrame::fromPointer((long) pointer);
            delete yuvFrame;
            yuvFrame = nullptr;
            break;
        }
    }
}extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_Frame_getBytes(JNIEnv *env, jobject thiz, jlong pointer) {
    Frame *frame = (Frame*) pointer;
    auto bytes = frame->getBytes();
    return env->NewDirectByteBuffer(bytes.data(), bytes.size());
}