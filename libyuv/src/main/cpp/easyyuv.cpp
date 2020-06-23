#include <jni.h>
#include <string>
#include <vector>
#include <libyuv/scale.h>
#include "libyuv.h"
#include "utils/logger.h"

//
// Created by Loboda Alexey on 22.06.2020.
//

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_libyuv_YuvUtils_scale(JNIEnv *env, jobject thiz, jobject y,
                                             jobject u, jobject v, jint yStride, jint uStride,
                                             jint vStride, jint srcWidth, jint srcHeight,
                                             jint dstWidth, jint dstHeight, jint filterMode) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    int yOutSize = sizeof(uint8_t) * (dstWidth * dstHeight);
    int uvOutSize = sizeof(uint8_t) * (dstWidth * dstHeight / 4);
    int exta = dstWidth % 2 == 0 ? 0 : 1;

    uint8_t *yOut = (uint8_t *) malloc((size_t) yOutSize);
    uint8_t *uOut = (uint8_t *) malloc((size_t) uvOutSize);
    uint8_t *vOut = (uint8_t *) malloc((size_t) uvOutSize);

    libyuv::I420Scale(yNative, yStride,
            uNative, uStride,
            vNative, vStride,
            srcWidth, srcHeight,
            yOut, dstWidth,
            uOut, dstWidth / 2 + exta,
            vOut, dstWidth / 2 + exta,
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

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_libyuv_YuvUtils_rotate(JNIEnv *env, jobject thiz, jobject y,
                                             jobject u, jobject v, jint yStride, jint uStride,
                                             jint vStride, jint width, jint height, jint rotationMode) {

    uint8_t *yNative = (uint8_t *) env->GetDirectBufferAddress(y);
    uint8_t *uNative = (uint8_t *) env->GetDirectBufferAddress(u);
    uint8_t *vNative = (uint8_t *) env->GetDirectBufferAddress(v);

    int yOutSize = sizeof(uint8_t) * (width * height);
    int uvOutSize = sizeof(uint8_t) * (width * height / 4);
    int exta = width % 2 == 0 ? 0 : 1;

    uint8_t *yOut = (uint8_t *) malloc((size_t) yOutSize);
    uint8_t *uOut = (uint8_t *) malloc((size_t) uvOutSize);
    uint8_t *vOut = (uint8_t *) malloc((size_t) uvOutSize);

    int yOutStride = (rotationMode == 90 || rotationMode == 270) ? height : width;
    int uvOutStride = yOutStride / 2 + exta;

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