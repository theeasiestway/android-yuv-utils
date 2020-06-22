package com.theeasiestway.libyuv

import android.media.Image
import android.util.Log

//
// Created by Loboda Alexey on 22.06.2020.
//

class YuvUtils {

    private val yuvFrame = YuvFrame()

    companion object {

        val TAG = "YuvUtils"

        init {
            try {
                System.loadLibrary("yuv")
                System.loadLibrary("easyyuv")
            }
            catch (e: Exception) { Log.e(TAG, "Couldn't load yuv library: $e") }
        }
    }

    fun scale(image: Image, dstWidth: Int, dstHeight: Int, filerMode: Int): ByteArray {
        yuvFrame.fill(image)
        return scale(yuvFrame.y,
            yuvFrame.u,
            yuvFrame.v,
            yuvFrame.yStride,
            yuvFrame.uStride,
            yuvFrame.vStride,
            yuvFrame.width,
            yuvFrame.height,
            dstWidth,
            dstHeight,
            filerMode)
    }

    private external fun scale(y: ByteArray,
                               u: ByteArray,
                               v: ByteArray,
                               yStride: Int,
                               uStride: Int,
                               vStride: Int,
                               srcWidth: Int,
                               srcHeight: Int,
                               dstWidth: Int,
                               dstHeight: Int,
                               filerMode: Int): ByteArray

    fun release() {
        yuvFrame.free()
    }
}