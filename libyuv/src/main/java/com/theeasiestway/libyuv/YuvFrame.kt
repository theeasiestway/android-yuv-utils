package com.theeasiestway.libyuv

import android.media.Image
import android.util.Log

//
// Created by Loboda Alexey on 22.06.2020.
//

class YuvFrame {

    lateinit var y: ByteArray; private set
    lateinit var u: ByteArray; private set
    lateinit var v: ByteArray; private set

    var yStride: Int = 0; private set
    var uStride: Int = 0; private set
    var vStride: Int = 0; private set

    var width: Int = 0; private set
    var height: Int = 0; private set

    fun fill(image: Image) {

        for (i in 0 until 3) {
            val plane = image.planes[i]
            val bytes = ByteArray(plane.buffer.remaining())
            plane.buffer.get(bytes)
            when(i) {
                0 -> {
                    y = bytes
                    yStride = plane.rowStride
                }
                1 -> {
                    u = bytes
                    uStride = plane.rowStride
                }
                2 -> {
                    v = bytes
                    vStride = plane.rowStride
                }
            }
        }

        width = image.width
        height = image.height

        Log.d("weewfwef", "width: $width; height: $height; y: ${y.size}; u: ${u.size}; v: ${v.size}")
        Log.d("weewfwef", "yStride: ${yStride}; uStride: ${uStride}; vStride: ${vStride}")

        image.close()
    }

    fun free() {
        y = ByteArray(1)
        u = ByteArray(1)
        v = ByteArray(1)

        yStride = 0
        uStride = 0
        vStride = 0

        width = 0
        height = 0
    }
}