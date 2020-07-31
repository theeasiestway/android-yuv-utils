//
// Created by Loboda Alexey on 31.07.2020.
//

#include "EntitiesFactory.h"

jobject EntitiesFactory::instanceArgb(RgbFrame &frame, JNIEnv &env) {
    jclass clazz = env.FindClass("com/theeasiestway/yuv/entities/ArgbFrame");
    jmethodID ctor = env.GetMethodID(clazz, "<init>", "()V");
    jfieldID pointer = env.GetFieldID(clazz, "nativePointer", "J");

    jobject instance = env.NewObject(clazz, ctor);
    env.SetLongField(instance, pointer, frame.getPointer(frame));

    return instance
}

jobject* EntitiesFactory::instanceRgb565(RgbFrame &frame) {

}