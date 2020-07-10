package com.theeasiestway.yuv.entities

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

    fun fill(image: Image) {
        for (i in 0 until 3) {
            when(i) {
                0 -> {
                    y = image.planes[i].buffer
                    yStride = image.planes[i].rowStride
                }
                1 -> {
                    u = image.planes[i].buffer
                    uStride = image.planes[i].rowStride
                }
                2 -> {
                    v = image.planes[i].buffer
                    vStride = image.planes[i].rowStride
                }
            }
        }
        width = image.width
        height = image.height
    }

    /** experimental method */
    fun fill(width: Int, height: Int, data: ByteArray) {
        this.width = width
        this.height = height

        val yArr = ByteArray(width * height)
        val uArr = ByteArray(width * height / 4)
        val vArr = ByteArray(width * height / 4)

        System.arraycopy(data, 0, yArr, 0, yArr.size)
        System.arraycopy(data, yArr.size, uArr, 0, uArr.size)
        System.arraycopy(data, yArr.size + uArr.size, vArr, 0, uArr.size)

        y = ByteBuffer.allocateDirect(yArr.size).put(yArr)
        u = ByteBuffer.allocateDirect(uArr.size).put(uArr)
        v = ByteBuffer.allocateDirect(vArr.size).put(vArr)

        y.position(0)
        u.position(0)
        v.position(0)
    }

    fun asArray(): ByteArray {
        var array: ByteArray

        val yPos = y.position()
        val uPos = u.position()
        val vPos = v.position()

        try {
            array = ByteBuffer.allocate(y.capacity() + u.capacity() + v.capacity()).put(y).put(u).put(v).array()
            y.position(yPos)
            u.position(uPos)
            v.position(vPos)
        } catch (e: Exception) {

            array = ByteArray(size())

            y.get(array, 0, y.remaining())
            y.position(yPos)

            u.get(array, y.remaining(), u.remaining())
            u.position(uPos)

            v.get(array, y.remaining() + u.remaining(), v.remaining())
            v.position(vPos)
        }
        return array
    }

    fun size() = y.remaining() + u.remaining() + v.remaining()

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