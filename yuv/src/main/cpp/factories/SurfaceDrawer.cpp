//
// Created by Loboda Alexey on 04.09.2020.
//

#include "SurfaceDrawer.h"
#include "../LibyuvWrapper.h"

void SurfaceDrawer::setSurface(JNIEnv &env, jobject surface, jint width, jint height) {
    ::width = width;
    ::height = height;
    window = ANativeWindow_fromSurface(&env, surface);
    needToUpdateBuffers = true;
}

void SurfaceDrawer::renderFrame(const RgbFrame &frame) {
    if (!window) return;

    if (needToUpdateBuffers) {
        // it's necessary to try to set width and height from frame
        ANativeWindow_setBuffersGeometry(window, width, height, frame.type == LibyuvWrapper::ARGB ? WINDOW_FORMAT_RGBA_8888 : WINDOW_FORMAT_RGB_565);
        needToUpdateBuffers = false;
    }

    int32_t ret = ANativeWindow_lock(window, &buffer, nullptr);

    if(ret != 0) {
        LOGE(TAG, "[renderFrame] ANativeWindow_lock error %d", ret);
        return;
    }

    int srcLineSize = frame.width * 2;  // 4 for RGBA
    int dstLineSize = buffer.stride * 2;

    uint8_t *dstBuffer = (uint8_t*) buffer.bits;

    for (int i = 0; i < height; ++i) {
        memcpy(dstBuffer + i * dstLineSize, frame.data + i * srcLineSize, srcLineSize);
    }

    ret = ANativeWindow_unlockAndPost(window);

    if(ret != 0) {
        LOGE(TAG, "[renderFrame] ANativeWindow_unlockAndPost error %d", ret);
        return;
    }
}

void SurfaceDrawer::releaseSurface() {
    if (window) {
        ANativeWindow_release(window);
        window = nullptr;
    }
    needToUpdateBuffers = true;
}