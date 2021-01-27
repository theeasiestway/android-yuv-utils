//
// Created by Loboda Alexey on 04.09.2020.
//

#include "SurfaceDrawer.h"
#include "../LibyuvWrapper.h"

void SurfaceDrawer::setSurface(JNIEnv &env, jobject surface, jint width, jint height) {
    isBuffersGeometrySet = false;
    window = ANativeWindow_fromSurface(&env, surface);
}

void SurfaceDrawer::renderFrame(const RgbFrame &frame) {
    if (!window) {
        LOGE(TAG, "[renderFrame] window is null");
        return;
    }
    if (frame.type == LibyuvWrapper::ARGB)
        renderArgbFrame(frame);
    else if (frame.type == LibyuvWrapper::RGB565)
        renderRgb565Frame(frame);
    else
        LOGE(TAG, "[renderFrame] incorrect frame type: %d", frame.type);
}

void SurfaceDrawer::renderArgbFrame(const RgbFrame &frame) {
    if (!isBuffersGeometrySet) {
        int ret = ANativeWindow_setBuffersGeometry(window, 0, 0, WINDOW_FORMAT_RGBA_8888);
        if(ret != 0) {
            LOGE(TAG, "[renderArgbFrame] ANativeWindow_setBuffersGeometry error %d", ret);
            SurfaceDrawer::releaseSurface();
            return;
        } else isBuffersGeometrySet = true;
    }

    ANativeWindow_Buffer buffer;
    int32_t ret = ANativeWindow_lock(window, &buffer, nullptr);

    if(ret != 0) {
        LOGE(TAG, "[renderArgbFrame] ANativeWindow_lock error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }

    for(int i = 0; i < buffer.height; ++i)
    {
        memcpy((uint8_t*) buffer.bits + i * (buffer.stride * PIXEL_ARGB), frame.data + i * frame.dataStride, buffer.stride * PIXEL_ARGB);
    }

    ret = ANativeWindow_unlockAndPost(window);

    if(ret != 0) {
        LOGE(TAG, "[renderFrame] ANativeWindow_unlockAndPost error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }
}

void SurfaceDrawer::renderRgb565Frame(const RgbFrame &frame) {
    if (!isBuffersGeometrySet) {
        int ret = ANativeWindow_setBuffersGeometry(window, 0, 0, WINDOW_FORMAT_RGB_565);
        if(ret != 0) {
            LOGE(TAG, "[renderRgb565Frame] ANativeWindow_setBuffersGeometry error %d", ret);
            SurfaceDrawer::releaseSurface();
            return;
        } else isBuffersGeometrySet = true;
    }

    ANativeWindow_Buffer buffer;
    int32_t ret = ANativeWindow_lock(window, &buffer, nullptr);

    if(ret != 0) {
        LOGE(TAG, "[renderRgb565Frame] ANativeWindow_lock error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }

    for(int i = 0; i < buffer.height; ++i)
    {
        memcpy((uint8_t*) buffer.bits + i * (buffer.stride * PIXEL_RGB), frame.data + i * frame.dataStride, buffer.stride * PIXEL_RGB);
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
}