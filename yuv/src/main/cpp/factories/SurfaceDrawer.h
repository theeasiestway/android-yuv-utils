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
static const int BYTES_ARGB = 4;
static const int BYTES_RGB = 2;

static ANativeWindow *window = nullptr;
static ANativeWindow_Buffer *buffer = nullptr;
static bool isBuffersGeometrySet = false;
static int bufferWidth = 0;
static int bufferHeight = 0;

class SurfaceDrawer {

private:
    static int setBuffersGeometryIfNeed(int frameWidth, int frameHeight, int frameType);
    static void renderArgbFrame(const RgbFrame &frame);
    static void renderRgb565Frame(const RgbFrame &frame);

public:
    static void setSurface(JNIEnv &env, jobject surface, jint width, jint height);
    static void renderFrame(const RgbFrame &frame);
    static void releaseSurface();
};

#endif //LIBYUVWRAPPER_SURFACEDRAWER_H
