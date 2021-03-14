package com.theeasiestway.yuv.entities

import android.graphics.Bitmap

//
// Created by Loboda Alexey on 29.07.2020.
//

abstract class RgbFrame: Frame() {
    var dataSize: Int = 0
    var dataStride: Int = 0

    abstract fun getBitmapSingle(): Bitmap?
    abstract fun getBitmap(isMutable: Boolean = true): Bitmap?
    abstract fun releaseBitmapSingle()
}