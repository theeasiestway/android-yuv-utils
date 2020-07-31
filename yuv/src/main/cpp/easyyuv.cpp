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

//
// Scale
//

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_yuv_YuvUtils_scale__Ljava_nio_ByteBuffer_2Ljava_nio_ByteBuffer_2Ljava_nio_ByteBuffer_2IIILjava_nio_ByteBuffer_2Ljava_nio_ByteBuffer_2Ljava_nio_ByteBuffer_2IIIIIIII(
        JNIEnv *env, jobject thiz, jobject y, jobject u, jobject v,
        jint yStride, jint uStride, jint vStride,
        jobject yOut, jobject uOut, jobject vOut,
        jint yOutStride, jint uOutStride, jint vOutStride,
        jint srcWidth, jint srcHeight, jint dstWidth, jint dstHeight, jint filterMode) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);


    uint8_t *yOutNative = (uint8_t *) env->GetDirectBufferAddress(yOut);
    uint8_t *uOutNative = (uint8_t *) env->GetDirectBufferAddress(uOut);
    uint8_t *vOutNative = (uint8_t *) env->GetDirectBufferAddress(vOut);

    libyuv::I420Scale(yNative, yStride,
                      uNative, uStride,
                      vNative, vStride,
                      srcWidth, srcHeight,
                      yOutNative, yOutStride,
                      uOutNative, uOutStride,
                      vOutNative, vOutStride,
                      dstWidth, dstHeight,
                      libyuv::FilterMode(filterMode));
}

//
// Rotate
//

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_yuv_YuvUtils_rotate(JNIEnv *env, jobject thiz, jobject y, jobject u,
                                           jobject v, jint yStride, jint uStride, jint vStride,
                                           jobject yOut, jobject uOut, jobject vOut,
                                           jint yOutStride, jint uOutStride, jint vOutStride,
                                           jint width, jint height, jint rotationMode) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    uint8_t *yOutNative = (uint8_t *) env->GetDirectBufferAddress(yOut);
    uint8_t *uOutNative = (uint8_t *) env->GetDirectBufferAddress(uOut);
    uint8_t *vOutNative = (uint8_t *) env->GetDirectBufferAddress(vOut);

    libyuv::I420Rotate(yNative, yStride,
                       uNative, uStride,
                       vNative, vStride,
                       yOutNative, yOutStride,
                       uOutNative, uOutStride,
                       vOutNative, vOutStride,
                       width, height,
                       libyuv::RotationMode(rotationMode));
}

//
// Mirror
//

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_yuv_YuvUtils_mirrorH(JNIEnv *env, jobject thiz, jobject y, jobject u,
                                            jobject v, jint yStride, jint uStride, jint vStride,
                                            jobject yOut, jobject uOut, jobject vOut,
                                            jint yOutStride, jint uOutStride, jint vOutStride,
                                            jint width, jint height) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    uint8_t *yOutNative = (uint8_t *) env->GetDirectBufferAddress(yOut);
    uint8_t *uOutNative = (uint8_t *) env->GetDirectBufferAddress(uOut);
    uint8_t *vOutNative = (uint8_t *) env->GetDirectBufferAddress(vOut);

    libyuv::I420Mirror(yNative, yStride,
                       uNative, uStride,
                       vNative, vStride,
                       yOutNative, yOutStride,
                       uOutNative, uOutStride,
                       vOutNative, vOutStride,
                       width, height);
}

//
// YUV to ARGB
//

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_yuv_YuvUtils_yuv420ToArgb(JNIEnv *env, jobject thiz, jobject y, jobject u,
                                                 jobject v, jint yStride, jint uStride, jint vStride,
                                                 jobject out, jint outStride,
                                                 jint width, jint height) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    uint8_t *outNative = (uint8_t *) env->GetDirectBufferAddress(out);

    libyuv::I420ToARGB(yNative, yStride,
                       vNative, vStride, // exactly this order "YVU" and not "YUV", otherwise the colors are inverted
                       uNative, uStride,
                       outNative, outStride,
                       width, height);
}

//
// YUV to RGB565
//

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_yuv_YuvUtils_yuv420ToRgb565(JNIEnv *env, jobject thiz, jobject y, jobject u,
                                                   jobject v, jint yStride, jint uStride, jint vStride,
                                                   jobject out, jint outStride,
                                                   jint width, jint height) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    uint8_t *outNative = (uint8_t *) env->GetDirectBufferAddress(out);

    libyuv::I420ToRGB565(yNative, yStride,
                         vNative, vStride, // exactly this order "YVU" and not "YUV", otherwise the colors are inverted
                         uNative, uStride,
                         outNative, outStride,
                         width, height);


}

//
// Convert to I420
//

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_yuv_YuvUtils_convertToI420(JNIEnv *env, jobject thiz, jobject y, jobject u,
                                                  jobject v, jint yStride, jint uStride, jint vStride, jint srcPixelStrideUv,
                                                  jobject yOut, jobject uOut, jobject vOut,
                                                  jint yOutStride, jint uOutStride, jint vOutStride,
                                                  jint width, jint height) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    uint8_t *yOutNative = (uint8_t *) env->GetDirectBufferAddress(yOut);
    uint8_t *uOutNative = (uint8_t *) env->GetDirectBufferAddress(uOut);
    uint8_t *vOutNative = (uint8_t *) env->GetDirectBufferAddress(vOut);

    libyuv::Android420ToI420(yNative, yStride,
                             uNative, uStride,
                             vNative, vStride,
                             srcPixelStrideUv,
                             yOutNative, yOutStride,
                             uOutNative, uOutStride,
                             vOutNative, vOutStride,
                             width, height);
}


















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

    if (returnType == LibyuvWrapper::RETURN_ARGB) {
        RgbFrame *rgbFrame = LibyuvWrapper::toArgbFrame(*yuvFrame);
        return *EntitiesFactory::instanceArgb(*rgbFrame, *env);
    }
}