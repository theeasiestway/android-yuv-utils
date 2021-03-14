//
// Created by Loboda Alexey on 04.09.2020.
//

#include "SurfaceDrawer.h"
#include "../LibyuvWrapper.h"

void SurfaceDrawer::setSurface(JNIEnv &env, jobject surface, jint width, jint height) {
    isBuffersGeometrySet = false;
    buffer = new ANativeWindow_Buffer();
    window = ANativeWindow_fromSurface(&env, surface);
    bufferWidth = width;
    bufferHeight = height;
}

int SurfaceDrawer::setBuffersGeometryIfNeed(int frameWidth, int frameHeight, int frameType) {
    bool needToSet = frameWidth < bufferWidth || frameHeight < bufferHeight;
    if (!isBuffersGeometrySet || needToSet) {
        int width = frameWidth < bufferWidth ? frameWidth : bufferWidth;
        int height = frameHeight < bufferHeight ? frameHeight : bufferHeight;
        auto format = frameType == LibyuvWrapper::ARGB ? WINDOW_FORMAT_RGBA_8888 : WINDOW_FORMAT_RGB_565;
        int ret = ANativeWindow_setBuffersGeometry(window, width, height, format);
        if(ret != 0) {
            LOGE(TAG, "[setBuffersGeometryIfNeed] ANativeWindow_setBuffersGeometry error %d; "
                      "frameType: %d; "
                      "frameWidth: %d; "
                      "frameHeight: %d; "
                      "bufferWidth: %d; "
                      "bufferHeight: %d",
                      ret, frameType, frameWidth, frameHeight, bufferWidth, bufferHeight);
            SurfaceDrawer::releaseSurface();
            return ret;
        } else isBuffersGeometrySet = true;
    }
    return 0;
}

void SurfaceDrawer::renderFrame(const RgbFrame &frame) {
    if (!window) {
        LOGE(TAG, "[renderFrame] window is null");
        return;
    }

    switch (frame.type) {
        case LibyuvWrapper::ARGB:
            renderArgbFrame(frame);
            break;

        case LibyuvWrapper::RGB565:
            renderRgb565Frame(frame);
            break;

        default:
            LOGE(TAG, "[renderFrame] incorrect frame type: %d", frame.type);
    }
}

void SurfaceDrawer::renderArgbFrame(const RgbFrame &frame) {
    if(setBuffersGeometryIfNeed(frame.width, frame.height, frame.type) != 0) return;

    int32_t ret = ANativeWindow_lock(window, buffer, nullptr);
    if(ret != 0) {
        LOGE(TAG, "[renderArgbFrame] ANativeWindow_lock error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }

    for(int i = 0; i < buffer->height; ++i) {
        memcpy((uint8_t*) buffer->bits + i * (buffer->stride * BYTES_ARGB), frame.data + i * frame.dataStride, buffer->stride * BYTES_ARGB);
    }

    ret = ANativeWindow_unlockAndPost(window);

    if(ret != 0) {
        LOGE(TAG, "[renderFrame] ANativeWindow_unlockAndPost error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }
}

void SurfaceDrawer::renderRgb565Frame(const RgbFrame &frame) {
    if(setBuffersGeometryIfNeed(frame.width, frame.height, frame.type) != 0) return;

    int32_t ret = ANativeWindow_lock(window, buffer, nullptr);
    if(ret != 0) {
        LOGE(TAG, "[renderRgb565Frame] ANativeWindow_lock error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }

    for(int i = 0; i < buffer->height; ++i) {
        memcpy((uint8_t*) buffer->bits + i * (buffer->stride * BYTES_RGB), frame.data + i * frame.dataStride, buffer->stride * BYTES_RGB);
    }

    ret = ANativeWindow_unlockAndPost(window);
    if(ret != 0) {
        LOGE(TAG, "[renderRgb565Frame] ANativeWindow_unlockAndPost error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }
}

void SurfaceDrawer::releaseSurface() {
    if (window) {
        ANativeWindow_release(window);
        window = nullptr;
    }
    isBuffersGeometrySet = false;
    delete buffer;
    bufferWidth = 0;
    bufferHeight = 0;
}