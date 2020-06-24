package com.theeasiestway.yuv.entities

import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 25.06.2020.
//

class ArgbFrame {

    lateinit var data: ByteBuffer; private set
    var dataStride: Int = 0; private set

    var width: Int = 0; private set
    var height: Int = 0; private set

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