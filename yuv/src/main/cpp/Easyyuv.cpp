#include <jni.h>
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
Java_com_theeasiestway_yuv_YuvUtils_transformNative__Ljava_nio_ByteBuffer_2Ljava_nio_ByteBuffer_2Ljava_nio_ByteBuffer_2IIIIIIIIIIZZI(JNIEnv *env, jobject thiz,
                                                    jobject y,
                                                    jobject u,
                                                    jobject v,
                                                    jint yStride,
                                                    jint uStride,
                                                    jint vStride,
                                                    jint uvPixelStride,
                                                    jint width,
                                                    jint height,
                                                    jint scaleWidth,
                                                    jint scaleHeight,
                                                    jint scaleFilter,
                                                    jint rotationMode,
                                                    jboolean mirrorH,
                                                    jboolean mirrorV,
                                                    jint returnType) {

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

    if (rotationMode >= 0) LibyuvWrapper::rotate(*yuvFrame, rotationMode);

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
        RgbFrame *rgbFrame = LibyuvWrapper::toRgb565Frame(*yuvFrame);
        return EntitiesFactory::instanceRgb565(*rgbFrame, *env);
    }

    return EntitiesFactory::instanceYuv(*yuvFrame, *env);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_YuvUtils_transformNative__Ljava_nio_ByteBuffer_2IIIIII(JNIEnv *env,
                                                                                       jobject thiz,
                                                                                       jobject data,
                                                                                       jint dataSize,
                                                                                       jint dataStride,
                                                                                       jint width,
                                                                                       jint height,
                                                                                       jint classType,
                                                                                       jint returnType) {
    RgbFrame *rgbFrame = new RgbFrame(width, height, dataStride, dataSize, classType);
    rgbFrame->data = (uint8_t*) env->GetDirectBufferAddress(data);

    //
    // to ARGB
    //

    if (rgbFrame->type != returnType && returnType == LibyuvWrapper::ARGB) {
        LibyuvWrapper::toArgbFrame(*rgbFrame);
        return EntitiesFactory::instanceArgb(*rgbFrame, *env);
    }

    //
    // to RGB565
    //

    if (rgbFrame->type != returnType && returnType == LibyuvWrapper::RGB565) {
        LibyuvWrapper::toRgb565Frame(*rgbFrame);
        return EntitiesFactory::instanceRgb565(*rgbFrame, *env);
    }

    YuvFrame *yuvFrame = LibyuvWrapper::to420(*rgbFrame);
    delete(rgbFrame);
    rgbFrame = nullptr;
    return EntitiesFactory::instanceYuv(*yuvFrame, *env);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_yuv_entities_Frame_destroy(JNIEnv *env, jobject thiz, jlong pointer, jint classType) {
    switch (classType) {
        case LibyuvWrapper::ARGB:
        case LibyuvWrapper::RGB565: {
            RgbFrame *rgbFrame = EntitiesFactory::fromPointer<RgbFrame>(pointer);
            delete rgbFrame;
            rgbFrame = nullptr;
            break;
        }
        case LibyuvWrapper::YUV:
        default: {
            YuvFrame *yuvFrame = EntitiesFactory::fromPointer<YuvFrame>(pointer);
            delete yuvFrame;
            yuvFrame = nullptr;
            break;
        }
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_Frame_getBytes(JNIEnv *env, jobject thiz, jlong pointer) {
    Frame *frame = EntitiesFactory::fromPointer<Frame>(pointer);
    return env->NewDirectByteBuffer(frame->data, frame->dataSize);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_RgbFrame_getBitmap(JNIEnv *env, jobject thiz, jlong pointer, jint classType) {
    switch (classType) {
        case LibyuvWrapper::ARGB:
        case LibyuvWrapper::RGB565: {
            RgbFrame *rgbFrame = EntitiesFactory::fromPointer<RgbFrame>(pointer);
            if (classType == LibyuvWrapper::RGB565) return EntitiesFactory::instanceBitmapRgb565(*rgbFrame, *env);
            else return EntitiesFactory::instanceBitmapArgb(*rgbFrame, *env);
        }
        default:
            return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_YuvFrame_getY(JNIEnv *env, jobject thiz, jlong pointer) {
    YuvFrame *frame = EntitiesFactory::fromPointer<YuvFrame>(pointer);
    return env->NewDirectByteBuffer(frame->y, frame->ySize);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_YuvFrame_getU(JNIEnv *env, jobject thiz, jlong pointer) {
    YuvFrame *frame = EntitiesFactory::fromPointer<YuvFrame>(pointer);
    return env->NewDirectByteBuffer(frame->u, frame->uSize);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_theeasiestway_yuv_entities_YuvFrame_getV(JNIEnv *env, jobject thiz, jlong pointer) {
    YuvFrame *frame = EntitiesFactory::fromPointer<YuvFrame>(pointer);
    return env->NewDirectByteBuffer(frame->v, frame->vSize);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_yuv_YuvUtils_00024Companion_nativeInit(JNIEnv *env, jobject thiz) {
    EntitiesFactory::init(*env);
}