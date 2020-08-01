//
// Created by Loboda Alexey on 31.07.2020.
//

#include "EntitiesFactory.h"

jobject EntitiesFactory::instanceYuv(YuvFrame &frame, JNIEnv &env) {
    jclass clazz = env.FindClass("com/theeasiestway/yuv/entities/YuvFrame");
    jmethodID ctor = env.GetMethodID(clazz, "<init>", "(J)V");
    jfieldID width = env.GetFieldID(clazz, "width", "I");
    jfieldID height = env.GetFieldID(clazz, "height", "I");

    jobject instance = env.NewObject(clazz, ctor, frame.getPointer(frame));
    env.SetIntField(instance, width, frame.width);
    env.SetIntField(instance, height, frame.height);
    return instance;
}

jobject EntitiesFactory::instanceArgb(RgbFrame &frame, JNIEnv &env) {
    jclass clazz = env.FindClass("com/theeasiestway/yuv/entities/ArgbFrame");
    jmethodID ctor = env.GetMethodID(clazz, "<init>", "(J)V");
    jfieldID width = env.GetFieldID(clazz, "width", "I");
    jfieldID height = env.GetFieldID(clazz, "height", "I");

    jobject instance = env.NewObject(clazz, ctor, frame.getPointer(frame));
    env.SetIntField(instance, width, frame.width);
    env.SetIntField(instance, height, frame.height);
    return instance;
}

jobject EntitiesFactory::instanceRgb565(RgbFrame &frame, JNIEnv &env) {
    jclass clazz = env.FindClass("com/theeasiestway/yuv/entities/Rgb565Frame");
    jmethodID ctor = env.GetMethodID(clazz, "<init>", "(J)V");
    jfieldID width = env.GetFieldID(clazz, "width", "I");
    jfieldID height = env.GetFieldID(clazz, "height", "I");

    jobject instance = env.NewObject(clazz, ctor, frame.getPointer(frame));
    env.SetIntField(instance, width, frame.width);
    env.SetIntField(instance, height, frame.height);
    return instance;
}