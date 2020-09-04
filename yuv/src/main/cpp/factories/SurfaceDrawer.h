//
// Created by Loboda Alexey on 04.09.2020.
//

#ifndef LIBYUVWRAPPER_SURFACEDRAWER_H
#define LIBYUVWRAPPER_SURFACEDRAWER_H

#include <jni.h>
#include <cstring>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "../utils/Logger.h"
#include "../entities/RgbFrame.h"

static const char *TAG = "SurfaceDrawer";

static bool needToUpdateBuffers = false;

static ANativeWindow *window = nullptr;
static int width, height;
static ANativeWindow_Buffer buffer;

class SurfaceDrawer {

public:
    static void setSurface(JNIEnv &env, jobject surface, jint width, jint height);
    static void renderFrame(const RgbFrame &frame);
    static void releaseSurface();
};

#endif //LIBYUVWRAPPER_SURFACEDRAWER_H
