package com.theeasiestway.libyuv

import android.media.Image
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

    fun fill(image: Image) {

        for (i in 0 until 3) {
            val plane = image.planes[i]
            when(i) {
                0 -> {
                    y = plane.buffer
                    yStride = plane.rowStride
                }
                1 -> {
                    u = plane.buffer
                    uStride = plane.rowStride
                }
                2 -> {
                    v = plane.buffer
                    vStride = plane.rowStride
                }
            }
        }

        width = image.width
        height = image.height

        image.close()
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