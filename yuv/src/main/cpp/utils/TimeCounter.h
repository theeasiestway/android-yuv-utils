//
// Created by Loboda Alexey on 08.08.2020.
//

#ifndef LIBYUVWRAPPER_TIMECOUNTER_H
#define LIBYUVWRAPPER_TIMECOUNTER_H

#include <unistd.h>
#include <chrono>
#include <string>
#include "Logger.h"

static std::chrono::time_point<std::chrono::steady_clock> start, end;
static const char *TAG = "TimeCounter";
static const char *EMPTY = "";

class TimeCounter {

private:
    static void print(const char *tag, const char *methodName, bool setTime) {
        end = std::chrono::high_resolution_clock::now();
        auto duration = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / (double)1000000;
        LOGD(tag, "%s time: %f ms.", methodName, duration);
        if (setTime) TimeCounter::setTime();
    }

public:
    static void setTime() {
        start = std::chrono::high_resolution_clock::now();
    }

    static void printTime(bool setTime = false) {
        print(TAG, EMPTY, setTime);
    }

    static void printTime(const char *methodName, bool setTime = false) {
        print(TAG, methodName, setTime);
    }

    static void printTime(const char *tag, const char *methodName, bool setTime = false) {
        print(tag, methodName, setTime);
    }
};

#endif //LIBYUVWRAPPER_TIMECOUNTER_H
