package com.theeasiestway.yuv.entities

import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 29.07.2020.
//

abstract class Frame {
    protected var nativePointer = -1L
    var width: Int = 0; protected set
    var height: Int = 0; protected set

    fun destroy() { destroy(nativePointer) }
    fun getBytes() = getBytes(nativePointer)

    private external fun destroy(pointer: Long)
    private external fun getBytes(pointer: Long): ByteBuffer
}