package com.theeasiestway.yuv.entities

import android.graphics.Bitmap

//
// Created by Loboda Alexey on 29.07.2020.
//

abstract class RgbFrame: Frame() {
    var dataSize: Int = 0
    var dataStride: Int = 0

    fun getBitmap() = getBitmap(nativePointer, getType())

    private external fun getBitmap(pointer: Long, classType: Int): Bitmap?
}