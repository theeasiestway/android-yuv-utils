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

    /*YuvFrame *yuvFrame = LibyuvWrapper::to420((uint8_t *) env->GetDirectBufferAddress(y), yStride,
                                              (uint8_t *) env->GetDirectBufferAddress(u), uStride,
                                              (uint8_t *) env->GetDirectBufferAddress(v), vStride,
                                              uvPixelStride, width, height);*/

    uint8_t *yBuf = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uBuf = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vBuf = (uint8_t *) env->GetDirectBufferAddress(v);

    int ySize = 307200;
    int uSize = 76800;
    int vSize = 76800;

    YuvFrame *yuvFrame = instanceYuv(width, height);
    yuvFrame->yStride = yStride;
    yuvFrame->uStride = uStride;
    yuvFrame->vStride = vStride;

    std::vector<uint8_t>().swap(yuvFrame->y);
    std::vector<uint8_t>().swap(yuvFrame->u);
    std::vector<uint8_t>().swap(yuvFrame->v);

    std::copy(&yBuf[0], &yBuf[ySize], back_inserter(yuvFrame->y));
    std::copy(&uBuf[0], &uBuf[uSize], back_inserter(yuvFrame->u));
    std::copy(&vBuf[0], &vBuf[vSize], back_inserter(yuvFrame->v));

    return EntitiesFactory::instanceYuv(*yuvFrame, *env);

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
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_Frame_getBytes(JNIEnv *env, jobject thiz, jlong pointer) {
    Frame *frame = (Frame*) pointer;
   // LOGD("asdgdsgsd OUT OF CLASS", "[0]: %d; [100]: %d; [200]: %d; [500]: %d", bytes[0], bytes[100], bytes[200], bytes[500]);
    return env->NewDirectByteBuffer()
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_YuvFrame_getY(JNIEnv *env, jobject thiz, jlong pointer) {
    YuvFrame *frame = (YuvFrame*) pointer;
    return env->NewDirectByteBuffer(frame->y.data(), frame->y.size());
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_YuvFrame_getU(JNIEnv *env, jobject thiz, jlong pointer) {
    YuvFrame *frame = (YuvFrame*) pointer;
    return env->NewDirectByteBuffer(frame->u.data(), frame->u.size());
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_YuvFrame_getV(JNIEnv *env, jobject thiz, jlong pointer) {
    YuvFrame *frame = (YuvFrame*) pointer;
    return env->NewDirectByteBuffer(frame->v.data(), frame->v.size());
}