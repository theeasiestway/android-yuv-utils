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
Java_com_theeasiestway_libyuv_YuvUtils_scale(JNIEnv *env, jobject thiz, jbyteArray y,
                                             jbyteArray u, jbyteArray v, jint yStride, jint uStride,
                                             jint vStride, jint srcWidth, jint srcHeight,
                                             jint dstWidth, jint dstHeight, jint filterMode) {

    int yOutSize = sizeof(uint8_t) * (dstWidth * dstHeight);
    int uvOutSize = sizeof(uint8_t) * (dstWidth * dstHeight / 4);

    uint8_t *yOut = (uint8_t *) malloc((size_t) yOutSize);
    uint8_t *uOut = (uint8_t *) malloc((size_t) uvOutSize);
    uint8_t *vOut = (uint8_t *) malloc((size_t) uvOutSize);

    uint8_t *yNative = (uint8_t *) env->GetByteArrayElements(y, 0);
    uint8_t *uNative = (uint8_t *) env->GetByteArrayElements(u, 0);
    uint8_t *vNative = (uint8_t *) env->GetByteArrayElements(v, 0);

    libyuv::I420Scale(yNative, yStride,
            uNative, uStride,
            vNative, vStride,
            srcWidth, srcHeight,
            yOut, 1024,
            uOut, 512,
            vOut, 512,
            dstWidth, dstHeight,
            libyuv::FilterMode(filterMode));

    LOGD("wefwefwfe", "yStride: %d; uStride: %d; vStride: %d", yStride, uStride, vStride);


    int yS = env->GetArrayLength(y);
    int uS = env->GetArrayLength(u);
    int vS = env->GetArrayLength(v);

   /* jbyteArray result = env->NewByteArray(yS + uS + vS);
    env->SetByteArrayRegion(result, 0, yS, (jbyte *) yNative);
    env->SetByteArrayRegion(result, yS, uS, (jbyte *) uNative);
    env->SetByteArrayRegion(result, uS, vS, (jbyte *) vNative); */

    env->ReleaseByteArrayElements(y, (jbyte *) yNative, 0);
    env->ReleaseByteArrayElements(u, (jbyte *) uNative, 0);
    env->ReleaseByteArrayElements(v, (jbyte *) vNative, 0);

    jbyteArray result = env->NewByteArray(yOutSize + uvOutSize * 2);
    env->SetByteArrayRegion(result, 0, yOutSize, (jbyte *) yOut);
    env->SetByteArrayRegion(result, yOutSize, uvOutSize, (jbyte *) uOut);
    env->SetByteArrayRegion(result, yOutSize + uvOutSize, uvOutSize, (jbyte *) vOut);

    free(yOut);
    free(uOut);
    free(vOut);

    return result;
}