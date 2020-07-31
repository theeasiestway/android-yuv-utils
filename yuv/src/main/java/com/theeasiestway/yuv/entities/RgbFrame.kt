package com.theeasiestway.yuv.entities

import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 29.07.2020.
//

abstract class RgbFrame: Frame() {

    lateinit var data: ByteBuffer; private set
    var dataStride: Int = 0; private set

    fun fill(data: ByteBuffer, dataStride: Int, width: Int, height: Int) {
        this.data = data
        this.dataStride = dataStride
        this.width = width
        this.height = height
    }

    fun asArray(): ByteArray {
        return ByteBuffer.allocate(data.capacity()).put(data).array()
    }

    fun free() {
        data = ByteBuffer.allocate(1)
        dataStride = 0
        width = 0
        height = 0
    }
}