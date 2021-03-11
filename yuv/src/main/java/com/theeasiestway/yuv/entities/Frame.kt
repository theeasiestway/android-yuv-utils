package com.theeasiestway.yuv.entities

import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 29.07.2020.
//

abstract class Frame {
    private val defPointerValue = 0L
    internal var nativePointer = defPointerValue
    var width: Int = 0; protected set
    var height: Int = 0; protected set

    abstract fun getType(): Int

    fun destroy() {
        if (nativePointer != defPointerValue) destroy(nativePointer, getType())
    }
    open fun getBytes() = getBytes(nativePointer)
    open fun getBytesArray() = getBytesArray(nativePointer)

    private external fun destroy(pointer: Long, classType: Int)
    private external fun getBytes(pointer: Long): ByteBuffer
    private external fun getBytesArray(pointer: Long): ByteArray
}