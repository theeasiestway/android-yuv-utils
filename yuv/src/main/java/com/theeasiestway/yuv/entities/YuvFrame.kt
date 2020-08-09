package com.theeasiestway.yuv.entities

import com.theeasiestway.yuv.Constants
import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 29.07.2020.
//

class YuvFrame: Frame() {

    lateinit var y: ByteBuffer; private set
    lateinit var u: ByteBuffer; private set
    lateinit var v: ByteBuffer; private set

    var yStride: Int = 0; private set
    var uStride: Int = 0; private set
    var vStride: Int = 0; private set

    override fun getType(): Int = Constants.YUV

    fun getBytesY() = getY(nativePointer)
    fun getBytesU() = getU(nativePointer)
    fun getBytesV() = getV(nativePointer)

    private external fun getY(pointer: Long): ByteBuffer
    private external fun getU(pointer: Long): ByteBuffer
    private external fun getV(pointer: Long): ByteBuffer
}