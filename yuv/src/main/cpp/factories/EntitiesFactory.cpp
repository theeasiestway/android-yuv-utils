//
// Created by Loboda Alexey on 31.07.2020.
//

#include "EntitiesFactory.h"
#include "../LibyuvWrapper.h"
#include "../utils/logger.h"
#include <android/bitmap.h>

//
// YUV
//

jclass classYuv;
jmethodID ctorYuv;
jfieldID pointerYuv;
jfieldID widthYuv;
jfieldID heightYuv;

//
// ARGB
//

jclass classArgb;
jmethodID ctorArgb;
jfieldID pointerArgb;
jfieldID widthArgb;
jfieldID heightArgb;

//
// RGB565
//

jclass classRgb565;
jmethodID ctorRgb565;
jfieldID pointerRgb565;
jfieldID widthRgb565;
jfieldID heightRgb565;

//
// Bitmap
//

jclass classBitmap;
jclass classBitmapConfig;
jmethodID ctorBitmap;
jmethodID ctorBitmapConfig;
jstring bitmapConfigArgb;
jstring bitmapConfigRgb565;

const char *TAG = "EntitiesFactory";

void EntitiesFactory::init(JNIEnv &env) {

    //
    // YUV
    //

    classYuv = (jclass) env.NewGlobalRef(env.FindClass("com/theeasiestway/yuv/entities/YuvFrame"));
    ctorYuv = env.GetMethodID(classYuv, "<init>", "()V");
    pointerYuv = env.GetFieldID(classYuv, "nativePointer", "J");
    widthYuv = env.GetFieldID(classYuv, "width", "I");
    heightYuv = env.GetFieldID(classYuv, "height", "I");

    //
    // ARGB
    //

    classArgb = (jclass) env.NewGlobalRef(env.FindClass("com/theeasiestway/yuv/entities/ArgbFrame"));
    ctorArgb = env.GetMethodID(classYuv, "<init>", "()V");
    pointerArgb = env.GetFieldID(classYuv, "nativePointer", "J");
    widthArgb = env.GetFieldID(classYuv, "width", "I");
    heightArgb = env.GetFieldID(classYuv, "height", "I");

    //
    // RGB565
    //

    classRgb565 = (jclass) env.NewGlobalRef(env.FindClass("com/theeasiestway/yuv/entities/Rgb565Frame"));
    ctorRgb565 = env.GetMethodID(classYuv, "<init>", "()V");
    pointerRgb565 = env.GetFieldID(classYuv, "nativePointer", "J");
    widthRgb565 = env.GetFieldID(classYuv, "width", "I");
    heightRgb565 = env.GetFieldID(classYuv, "height", "I");

    //
    // Bitmap
    //

    classBitmap = (jclass) env.NewGlobalRef(env.FindClass("android/graphics/Bitmap"));
    ctorBitmap = env.GetStaticMethodID(classBitmap, "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    bitmapConfigArgb = (jstring) env.NewGlobalRef(env.NewStringUTF("ARGB_8888"));
    bitmapConfigRgb565 = (jstring) env.NewGlobalRef(env.NewStringUTF("RGB_565"));
    classBitmapConfig = (jclass) env.NewGlobalRef(env.FindClass("android/graphics/Bitmap$Config"));
    ctorBitmapConfig = env.GetStaticMethodID(classBitmapConfig, "valueOf", "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");
}

jobject EntitiesFactory::instanceYuv(YuvFrame &frame, JNIEnv &env) {
    jobject instance = env.NewObject(classYuv, ctorYuv);
    env.SetLongField(instance, pointerYuv, frame.getPointer(frame));
    env.SetIntField(instance, widthYuv, frame.width);
    env.SetIntField(instance, heightYuv, frame.height);
    return instance;
}

jobject EntitiesFactory::instanceArgb(RgbFrame &frame, JNIEnv &env) {
    if (frame.type != LibyuvWrapper::ARGB) return nullptr;
    jobject instance = env.NewObject(classArgb, ctorArgb);
    env.SetLongField(instance, pointerArgb, frame.getPointer(frame));
    env.SetIntField(instance, widthArgb, frame.width);
    env.SetIntField(instance, heightArgb, frame.height);
    return instance;
}

jobject EntitiesFactory::instanceRgb565(RgbFrame &frame, JNIEnv &env) {
    if (frame.type != LibyuvWrapper::RGB565) return nullptr;
    jobject instance = env.NewObject(classRgb565, ctorRgb565);
    env.SetLongField(instance, pointerRgb565, frame.getPointer(frame));
    env.SetIntField(instance, widthRgb565, frame.width);
    env.SetIntField(instance, heightRgb565, frame.height);
    return instance;
}

//
// Bitmap
//

jobject EntitiesFactory::instanceBitmapArgb(RgbFrame &frame, JNIEnv &env) {
    if (frame.type != LibyuvWrapper::ARGB || frame.dataSize <= 0) return nullptr;
    return instanceBitmap(frame, env, bitmapConfigArgb, 4);
}

jobject EntitiesFactory::instanceBitmapRgb565(RgbFrame &frame, JNIEnv &env) {
    if (frame.type != LibyuvWrapper::RGB565 || frame.dataSize <= 0) return nullptr;
    return instanceBitmap(frame, env, bitmapConfigRgb565, 2);
}

jobject EntitiesFactory::instanceBitmap(RgbFrame &frame, JNIEnv &env, jstring bitmapConfig, short extra) {
    jobject config = env.CallStaticObjectMethod(classBitmapConfig, ctorBitmapConfig, bitmapConfig);
    jobject bitmap = env.CallStaticObjectMethod(classBitmap, ctorBitmap, frame.width, frame.height, config);
    void *pixels;
    int ret = 0;
    if ((ret = AndroidBitmap_lockPixels(&env, bitmap, &pixels)) < 0) {
        LOGE(TAG, "[instanceBitmapArgb] lockPixels error: %d", ret);
        return nullptr;
    }

    memcpy(pixels, frame.data, sizeof(uint8_t) * (frame.width * frame.height) * extra);
    AndroidBitmap_unlockPixels(&env, bitmap);
    return bitmap;
}
