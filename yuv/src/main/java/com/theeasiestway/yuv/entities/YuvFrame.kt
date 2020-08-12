package com.theeasiestway.yuv.entities

import com.theeasiestway.yuv.Constants
import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 29.07.2020.
//

open class YuvFrame: Frame() {

    var yStride: Int = 0
    var uStride: Int = 0
    var vStride: Int = 0

    override fun getType(): Int = Constants.YUV

    open fun getBytesY() = getY(nativePointer)
    open fun getBytesU() = getU(nativePointer)
    open fun getBytesV() = getV(nativePointer)

    private external fun getY(pointer: Long): ByteBuffer
    private external fun getU(pointer: Long): ByteBuffer
    private external fun getV(pointer: Long): ByteBuffer
}