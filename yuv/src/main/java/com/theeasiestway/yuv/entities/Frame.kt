package com.theeasiestway.yuv.entities

import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 29.07.2020.
//

abstract class Frame {
    protected var nativePointer = -1
    var width: Int = 0; protected set
    var height: Int = 0; protected set

    fun destroy() { destroy(nativePointer) }
    fun getBytes() = getBytes(nativePointer)

    private external fun destroy(pointer: Int)
    private external fun getBytes(pointer: Int): ByteBuffer
}