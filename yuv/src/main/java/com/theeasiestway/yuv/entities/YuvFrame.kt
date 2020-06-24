package com.theeasiestway.yuv.entities

import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 22.06.2020.
//

class YuvFrame {

    lateinit var y: ByteBuffer; private set
    lateinit var u: ByteBuffer; private set
    lateinit var v: ByteBuffer; private set

    var yStride: Int = 0; private set
    var uStride: Int = 0; private set
    var vStride: Int = 0; private set

    var width: Int = 0; private set
    var height: Int = 0; private set

    fun fill(y: ByteBuffer, u: ByteBuffer, v: ByteBuffer, yStride: Int, uStride: Int, vStride: Int, width: Int, height: Int) {
        this.y = y
        this.u = u
        this.v = v
        this.yStride = yStride
        this.uStride = uStride
        this.vStride = vStride
        this.width = width
        this.height = height
    }

    fun asArray(): ByteArray {
        return ByteBuffer.allocate(y.capacity() + u.capacity() + v.capacity()).put(y).put(u).put(v).array()
    }

    fun free() {
        y = ByteBuffer.allocate(1)
        u = ByteBuffer.allocate(1)
        v = ByteBuffer.allocate(1)
        yStride = 0
        uStride = 0
        vStride = 0
        width = 0
        height = 0
    }
}