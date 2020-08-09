//
// Created by Loboda Alexey on 31.07.2020.
//

#include "EntitiesFactory.h"
#include "../LibyuvWrapper.h"

jclass classYuv;
jmethodID ctorYuv;
jfieldID pointerYuv;
jfieldID widthYuv;
jfieldID heightYuv;

jclass classArgb;
jmethodID ctorArgb;
jfieldID pointerArgb;
jfieldID widthArgb;
jfieldID heightArgb;

jclass classRgb565;
jmethodID ctorRgb565;
jfieldID pointerRgb565;
jfieldID widthRgb565;
jfieldID heightRgb565;

void EntitiesFactory::init(JNIEnv &env) {
    classYuv = (jclass) env.NewGlobalRef(env.FindClass("com/theeasiestway/yuv/entities/YuvFrame"));
    ctorYuv = env.GetMethodID(classYuv, "<init>", "()V");
    pointerYuv = env.GetFieldID(classYuv, "nativePointer", "J");
    widthYuv = env.GetFieldID(classYuv, "width", "I");
    heightYuv = env.GetFieldID(classYuv, "height", "I");

    classArgb = (jclass) env.NewGlobalRef(env.FindClass("com/theeasiestway/yuv/entities/ArgbFrame"));
    ctorArgb = env.GetMethodID(classYuv, "<init>", "()V");
    pointerArgb = env.GetFieldID(classYuv, "nativePointer", "J");
    widthArgb = env.GetFieldID(classYuv, "width", "I");
    heightArgb = env.GetFieldID(classYuv, "height", "I");

    classRgb565 = (jclass) env.NewGlobalRef(env.FindClass("com/theeasiestway/yuv/entities/Rgb565Frame"));
    ctorRgb565 = env.GetMethodID(classYuv, "<init>", "()V");
    pointerRgb565 = env.GetFieldID(classYuv, "nativePointer", "J");
    widthRgb565 = env.GetFieldID(classYuv, "width", "I");
    heightRgb565 = env.GetFieldID(classYuv, "height", "I");
}

jobject EntitiesFactory::instanceYuv(YuvFrame &frame, JNIEnv &env) {
    jobject instance = env.NewObject(classYuv, ctorYuv);
    env.SetLongField(instance, pointerYuv, frame.getPointer(frame));
    env.SetIntField(instance, widthYuv, frame.width);
    env.SetIntField(instance, heightYuv, frame.height);
    return instance;
}

jobject EntitiesFactory::instanceArgb(RgbFrame &frame, JNIEnv &env) {
    jobject instance = env.NewObject(classArgb, ctorArgb);
    env.SetLongField(instance, pointerArgb, frame.getPointer(frame));
    env.SetIntField(instance, widthArgb, frame.width);
    env.SetIntField(instance, heightArgb, frame.height);
    return instance;
}

jobject EntitiesFactory::instanceRgb565(RgbFrame &frame, JNIEnv &env) {
    jobject instance = env.NewObject(classRgb565, ctorRgb565);
    env.SetLongField(instance, pointerRgb565, frame.getPointer(frame));
    env.SetIntField(instance, widthRgb565, frame.width);
    env.SetIntField(instance, heightRgb565, frame.height);
    return instance;
}