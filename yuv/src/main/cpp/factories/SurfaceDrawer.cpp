//
// Created by Loboda Alexey on 04.09.2020.
//

#include "SurfaceDrawer.h"
#include "../LibyuvWrapper.h"

void SurfaceDrawer::setSurface(JNIEnv &env, jobject surface, jint width, jint height) {
    ::width = width;
    ::height = height;
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
    if (width != frame.width || height != frame.height) {
        LOGE(TAG, "[renderArgbFrame] frame.width %d; frame.height: %d", frame.width, frame.height);
        width = frame.width;
        height = frame.height;
        // it's necessary to try to set width and height from frame
        int ret = ANativeWindow_setBuffersGeometry(window, 0, 0, WINDOW_FORMAT_RGBA_8888);
        if(ret != 0) {
            LOGE(TAG, "[renderArgbFrame] ANativeWindow_setBuffersGeometry error %d", ret);
            SurfaceDrawer::releaseSurface();
            return;
        }
    }

    ANativeWindow_Buffer buffer;
    int32_t ret = ANativeWindow_lock(window, &buffer, nullptr);

    if(ret != 0) {
        LOGE(TAG, "[renderArgbFrame] ANativeWindow_lock error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }

    int rgb_pixel = 4;

    /*if( buffer.stride <= buffer.width)
    {
        LOGE("wefefewfwf", "[1]");
        memcpy((uint8_t*) buffer.bits, frame.data, width * rgb_pixel * height);
    }
    else
    {
        LOGE("wefefewfwf", "[2]");
        LOGE("wefefewfwf", "frame width: %d; frame height: %d; frame stride: %d; frame dataSize: %d; buffer width: %d; buffer height: %d; buffer stride: %d", frame.width, frame.height, frame.dataStride, frame.dataSize, buffer.width, buffer.height, buffer.stride);
        int count = 0;
        for(int i = 0; i < buffer.height; ++i)
        {
            LOGE("wefefewfwf", "count: %d", count);
            memcpy((uint8_t*) buffer.bits + buffer.stride * i * rgb_pixel, frame.data + width * i * rgb_pixel, width * rgb_pixel);
            count += width * rgb_pixel; // doesn't crash 2636800
        }
    }*/

    for(int i = 0; i < buffer.height; ++i)
    {
        memcpy((uint8_t*) buffer.bits + i * (buffer.stride * rgb_pixel), frame.data + i * frame.dataStride, buffer.stride * rgb_pixel);
    }

    ret = ANativeWindow_unlockAndPost(window);

    if(ret != 0) {
        LOGE(TAG, "[renderFrame] ANativeWindow_unlockAndPost error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }
}

void SurfaceDrawer::renderRgb565Frame(const RgbFrame &frame) {
    if (width != frame.width || height != frame.height) {
        LOGE(TAG, "[renderRgb565Frame] frame.width %d; frame.height: %d", frame.width, frame.height);
        width = frame.width;
        height = frame.height;
        // it's necessary to try to set width and height from frame
        int ret = ANativeWindow_setBuffersGeometry(window, 0, 0, WINDOW_FORMAT_RGB_565);
        if(ret != 0) {
            LOGE(TAG, "[renderRgb565Frame] ANativeWindow_setBuffersGeometry error %d", ret);
            SurfaceDrawer::releaseSurface();
            return;
        }
    }

    ANativeWindow_Buffer buffer;
    int32_t ret = ANativeWindow_lock(window, &buffer, nullptr);

    if(ret != 0) {
        LOGE(TAG, "[renderRgb565Frame] ANativeWindow_lock error %d", ret);
        SurfaceDrawer::releaseSurface();
        return;
    }

    int rgbPixel = 2;

    for(int i = 0; i < buffer.height; ++i)
    {
        memcpy((uint8_t*) buffer.bits + i * (buffer.stride * rgbPixel), frame.data + i * frame.dataStride, buffer.stride * rgbPixel);
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
}