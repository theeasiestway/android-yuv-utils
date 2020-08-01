//
// Created by Loboda Alexey on 31.07.2020.
//

#ifndef LIBYUVWRAPPER_ENTITIESFACTORY_H
#define LIBYUVWRAPPER_ENTITIESFACTORY_H

#include <jni.h>
#include "../entities/YuvFrame.h"
#include "../entities/RgbFrame.h"

class EntitiesFactory {

public:
    static jobject instanceYuv(YuvFrame &frame, JNIEnv &env);
    static jobject instanceArgb(RgbFrame &frame, JNIEnv &env);
    static jobject instanceRgb565(RgbFrame &frame, JNIEnv &env);
};


#endif //LIBYUVWRAPPER_ENTITIESFACTORY_H
