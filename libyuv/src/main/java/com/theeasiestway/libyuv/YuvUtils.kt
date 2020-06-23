package com.theeasiestway.libyuv

import android.media.Image
import android.util.Log
import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 22.06.2020.
//

class YuvUtils {

    private val yuvFrame = YuvFrame()

    companion object {

        val TAG = "YuvUtils"

        init {
            try { System.loadLibrary("easyyuv") }
            catch (e: Exception) { Log.e(TAG, "Couldn't load easyyuv library: $e") }
        }
    }

    //
    // Scale
    //

    fun scale(image: Image, dstWidth: Int, dstHeight: Int, filerMode: Int): ByteArray {
        require(filerMode in 0..3) { "filterMode: $filerMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
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

    private external fun scale(y: ByteBuffer,
                               u: ByteBuffer,
                               v: ByteBuffer,
                               yStride: Int,
                               uStride: Int,
                               vStride: Int,
                               srcWidth: Int,
                               srcHeight: Int,
                               dstWidth: Int,
                               dstHeight: Int,
                               filerMode: Int): ByteArray

    //
    // Rotate
    //

    fun rotate(image: Image, rotationMode: Int): ByteArray {
        require(rotationMode == 0 || rotationMode == 90 || rotationMode == 180 || rotationMode == 270) { "rotationMode: $rotationMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        yuvFrame.fill(image)
        return rotate(yuvFrame.y,
            yuvFrame.u,
            yuvFrame.v,
            yuvFrame.yStride,
            yuvFrame.uStride,
            yuvFrame.vStride,
            yuvFrame.width,
            yuvFrame.height,
            rotationMode)
    }

    private external fun rotate(y: ByteBuffer,
                               u: ByteBuffer,
                               v: ByteBuffer,
                               yStride: Int,
                               uStride: Int,
                               vStride: Int,
                               width: Int,
                               height: Int,
                               rotationMode: Int): ByteArray

    fun release() {
        yuvFrame.free()
    }
}