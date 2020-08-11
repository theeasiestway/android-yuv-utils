//
// Created by Loboda Alexey on 31.07.2020.
//

#ifndef LIBYUVWRAPPER_ENTITIESFACTORY_H
#define LIBYUVWRAPPER_ENTITIESFACTORY_H

#include <jni.h>
#include "../entities/YuvFrame.h"
#include "../entities/RgbFrame.h"

class EntitiesFactory {

private:
    static jobject instanceBitmap(RgbFrame &frame, JNIEnv &env, jstring bitmapConfig, short extra);

public:
    static void init(JNIEnv &env);
    static jobject instanceYuv(YuvFrame &frame, JNIEnv &env);
    static jobject instanceArgb(RgbFrame &frame, JNIEnv &env);
    static jobject instanceRgb565(RgbFrame &frame, JNIEnv &env);
    static jobject instanceBitmapArgb(RgbFrame &frame, JNIEnv &env);
    static jobject instanceBitmapRgb565(RgbFrame &frame, JNIEnv &env);

    template<typename T>
    static T* fromPointer(jlong ptr) {
        return (T*) ptr;
    }
};

#endif //LIBYUVWRAPPER_ENTITIESFACTORY_H
