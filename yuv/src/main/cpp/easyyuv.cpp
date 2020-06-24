#include <jni.h>
#include <string>
#include <vector>
#include <libyuv/scale.h>
#include "libyuv.h"
#include "utils/logger.h"

//
// Created by Loboda Alexey on 22.06.2020.
//


//
// Scale
//

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_yuv_YuvUtils_scale(JNIEnv *env, jobject thiz, jobject y,
                                          jobject u, jobject v, jint yStride, jint uStride,
                                          jint vStride, jint srcWidth, jint srcHeight,
                                          jint dstWidth, jint dstHeight, jint filterMode) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    int yOutSize = sizeof(uint8_t) * (dstWidth * dstHeight);
    int uvOutSize = sizeof(uint8_t) * (dstWidth * dstHeight / 4);
    int extra = dstWidth % 2 == 0 ? 0 : 1;

    uint8_t *yOut = (uint8_t *) malloc((size_t) yOutSize);
    uint8_t *uOut = (uint8_t *) malloc((size_t) uvOutSize);
    uint8_t *vOut = (uint8_t *) malloc((size_t) uvOutSize);

    libyuv::I420Scale(yNative, yStride,
                      uNative, uStride,
                      vNative, vStride,
                      srcWidth, srcHeight,
                      yOut, dstWidth,
                      uOut, dstWidth / 2 + extra,
                      vOut, dstWidth / 2 + extra,
                      dstWidth, dstHeight,
                      libyuv::FilterMode(filterMode));

    jbyteArray result = env->NewByteArray(yOutSize + uvOutSize * 2);
    env->SetByteArrayRegion(result, 0, yOutSize, (jbyte *) yOut);
    env->SetByteArrayRegion(result, yOutSize, uvOutSize, (jbyte *) uOut);
    env->SetByteArrayRegion(result, yOutSize + uvOutSize, uvOutSize, (jbyte *) vOut);

    free(yOut);
    free(uOut);
    free(vOut);

    return result;
}

//
// Rotate
//

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_yuv_YuvUtils_rotate(JNIEnv *env, jobject thiz, jobject y,
                                           jobject u, jobject v, jint yStride, jint uStride,
                                           jint vStride, jint width, jint height, jint rotationMode) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    int yOutSize = sizeof(uint8_t) * (width * height);
    int uvOutSize = sizeof(uint8_t) * (width * height / 4);
    int extra = width % 2 == 0 ? 0 : 1;

    uint8_t *yOut = (uint8_t *) malloc((size_t) yOutSize);
    uint8_t *uOut = (uint8_t *) malloc((size_t) uvOutSize);
    uint8_t *vOut = (uint8_t *) malloc((size_t) uvOutSize);

    int yOutStride = (rotationMode == 90 || rotationMode == 270) ? height : width;
    int uvOutStride = yOutStride / 2 + extra;

    libyuv::I420Rotate(yNative, yStride,
                       uNative, uStride,
                       vNative, vStride,
                       yOut, yOutStride,
                       uOut, uvOutStride,
                       vOut, uvOutStride,
                       width, height,
                       libyuv::RotationMode(rotationMode));

    jbyteArray result = env->NewByteArray(yOutSize + uvOutSize * 2);
    env->SetByteArrayRegion(result, 0, yOutSize, (jbyte *) yOut);
    env->SetByteArrayRegion(result, yOutSize, uvOutSize, (jbyte *) uOut);
    env->SetByteArrayRegion(result, yOutSize + uvOutSize, uvOutSize, (jbyte *) vOut);

    free(yOut);
    free(uOut);
    free(vOut);

    return result;
}

//
// Mirror
//

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_yuv_YuvUtils_mirror(JNIEnv *env, jobject thiz, jobject y,
                                           jobject u, jobject v, jint yStride, jint uStride,
                                           jint vStride, jint width, jint height) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    int yOutSize = sizeof(uint8_t) * (width * height);
    int uvOutSize = sizeof(uint8_t) * (width * height / 4);
    int extra = width % 2 == 0 ? 0 : 1;

    uint8_t *yOut = (uint8_t *) malloc((size_t) yOutSize);
    uint8_t *uOut = (uint8_t *) malloc((size_t) uvOutSize);
    uint8_t *vOut = (uint8_t *) malloc((size_t) uvOutSize);

    libyuv::I420Mirror(yNative, yStride,
                       uNative, uStride,
                       vNative, vStride,
                       yOut, width,
                       uOut, width / 2 + extra,
                       vOut, width / 2 + extra,
                       width, height);

    jbyteArray result = env->NewByteArray(yOutSize + uvOutSize * 2);
    env->SetByteArrayRegion(result, 0, yOutSize, (jbyte *) yOut);
    env->SetByteArrayRegion(result, yOutSize, uvOutSize, (jbyte *) uOut);
    env->SetByteArrayRegion(result, yOutSize + uvOutSize, uvOutSize, (jbyte *) vOut);

    free(yOut);
    free(uOut);
    free(vOut);

    return result;
}

//
// YUV to ARGB
//

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_yuv_YuvUtils_yuv420ToArgb(JNIEnv *env, jobject thiz, jobject y, jobject u,
                                                 jobject v, jint yStride, jint uStride,
                                                 jint vStride, jint width, jint height) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    int argbSize = sizeof(uint8_t) * (width * height) * 32;
    int extra = width % 2 == 0 ? 0 : 1;

    uint8_t *argb = (uint8_t *) malloc((size_t) argbSize);

    libyuv::I420ToARGB(yNative, yStride,
                       vNative, vStride, // exactly this order "YVU" and not "YUV", otherwise the colors are inverted
                       uNative, uStride,
                       argb, width * 4 + extra,
                       width, height);

    jbyteArray result = env->NewByteArray(argbSize);
    env->SetByteArrayRegion(result, 0, argbSize, (jbyte *) argb);

    free(argb);

    return result;
}

