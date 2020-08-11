package com.theeasiestway.yuv.entities

import android.graphics.Bitmap
import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 29.07.2020.
//

abstract class RgbFrame: Frame() {

    lateinit var data: ByteBuffer; private set
    var dataStride: Int = 0; private set

    fun getBitmap() = getBitmap(nativePointer, getType())

    private external fun getBitmap(pointer: Long, classType: Int): Bitmap?
}