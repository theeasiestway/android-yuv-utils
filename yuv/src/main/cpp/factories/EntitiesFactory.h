//
// Created by Loboda Alexey on 31.07.2020.
//

#ifndef LIBYUVWRAPPER_ENTITIESFACTORY_H
#define LIBYUVWRAPPER_ENTITIESFACTORY_H

#include <jni.h>
#include "../entities/YuvFrame.h"

class EntitiesFactory {

public:

    static jobject instanceArgb(YuvFrame &frame, );
    static jobject instanceRgb565(YuvFrame &frame);
};


#endif //LIBYUVWRAPPER_ENTITIESFACTORY_H
