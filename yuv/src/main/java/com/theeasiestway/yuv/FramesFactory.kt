package com.theeasiestway.yuv

import com.theeasiestway.yuv.entities.ArgbFrame
import com.theeasiestway.yuv.entities.YuvFrame
import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 25.06.2020.
//

object FramesFactory {

    fun instanceYuv(dstWidth: Int, dstHeight: Int): YuvFrame {
        val yuvFrame = YuvFrame()
        val ySize = dstWidth * dstHeight
        val uvSize = dstWidth * dstHeight / 4
        val y = ByteBuffer.allocateDirect(ySize)
        val u = ByteBuffer.allocateDirect(uvSize)
        val v = ByteBuffer.allocateDirect(uvSize)
        val extra = if (dstWidth % 2 == 0) 0 else 1
        yuvFrame.fill(y, u, v, dstWidth, dstWidth / 2 + extra, dstWidth / 2 + extra, dstWidth, dstHeight)
        return yuvFrame
    }

    fun instanceYuv(width: Int, height: Int, rotationMode: Int): YuvFrame {
        val newFrame = YuvFrame()
        val outWidth = if (rotationMode == 90 || rotationMode == 270) height else width
        val outHeight = if (rotationMode == 90 || rotationMode == 270) width else height
        val ySize = outWidth * outHeight
        val uvSize = outWidth * outHeight / 4
        val y = ByteBuffer.allocateDirect(ySize)
        val u = ByteBuffer.allocateDirect(uvSize)
        val v = ByteBuffer.allocateDirect(uvSize)
        val extra = if (outWidth % 2 == 0) 0 else 1
        newFrame.fill(y, u, v, outWidth, outWidth / 2 + extra, outWidth / 2 + extra, outWidth, outHeight)
        return newFrame
    }

    fun instanceArgb(width: Int, height: Int): ArgbFrame {
        val newFrame = ArgbFrame()
        val data = ByteBuffer.allocateDirect((width * height) * 32)
        val extra = if (width % 2 == 0) 0 else 1
        val dataStride = width * 4 + extra
        newFrame.fill(data, dataStride, width, height)
        return newFrame
    }
}