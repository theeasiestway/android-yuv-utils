package com.theeasiestway.yuv

import android.media.Image
import android.util.Log
import com.theeasiestway.yuv.entities.ResultFrame
import com.theeasiestway.yuv.entities.YuvFrame
import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 22.06.2020.
//

class YuvUtils {

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

    private val rawScaleFrame = YuvFrame()
    private val resultScaleFrame = ResultFrame()

    fun scale(image: Image, dstWidth: Int, dstHeight: Int, filerMode: Int): ResultFrame {
        require(filerMode in 0..3) { "filterMode: $filerMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        rawScaleFrame.fill(image)
        resultScaleFrame.fill(scale(rawScaleFrame.y,
            rawScaleFrame.u,
            rawScaleFrame.v,
            rawScaleFrame.yStride,
            rawScaleFrame.uStride,
            rawScaleFrame.vStride,
            rawScaleFrame.width,
            rawScaleFrame.height,
            dstWidth,
            dstHeight,
            filerMode), dstWidth, dstHeight)
        return resultScaleFrame
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

    private val rawRotateFrame = YuvFrame()
    private val resultRotateFrame = ResultFrame()

    fun rotate(image: Image, rotationMode: Int): ResultFrame {
        require(rotationMode == 0 || rotationMode == 90 || rotationMode == 180 || rotationMode == 270) { "rotationMode: $rotationMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        rawRotateFrame.fill(image)

        val width = if (rotationMode == 90 || rotationMode == 270) rawRotateFrame.height else rawRotateFrame.width
        val height = if (rotationMode == 90 || rotationMode == 270) rawRotateFrame.width else rawRotateFrame.height

        resultRotateFrame.fill(rotate(rawRotateFrame.y,
            rawRotateFrame.u,
            rawRotateFrame.v,
            rawRotateFrame.yStride,
            rawRotateFrame.uStride,
            rawRotateFrame.vStride,
            rawRotateFrame.width,
            rawRotateFrame.height,
            rotationMode), width, height)
        return resultRotateFrame
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

    //
    // Mirror
    //

    private val rawMirrorFrame = YuvFrame()
    private val resultMirrorFrame = ResultFrame()

    fun mirror(image: Image): ResultFrame {
        rawMirrorFrame.fill(image)
        resultMirrorFrame.fill(mirror(rawMirrorFrame.y,
            rawMirrorFrame.u,
            rawMirrorFrame.v,
            rawMirrorFrame.yStride,
            rawMirrorFrame.uStride,
            rawMirrorFrame.vStride,
            rawMirrorFrame.width,
            rawMirrorFrame.height), rawMirrorFrame.width, rawMirrorFrame.height)
        return resultMirrorFrame
    }

    private external fun mirror(y: ByteBuffer,
                                u: ByteBuffer,
                                v: ByteBuffer,
                                yStride: Int,
                                uStride: Int,
                                vStride: Int,
                                width: Int,
                                height: Int): ByteArray

    //
    // YUV to ARGB
    //

    private val rawYuvToArgbFrame = YuvFrame()
    private val resultYuvToArgbFrame = ResultFrame()

    fun yuv420ToArgb(image: Image): ResultFrame {
        rawYuvToArgbFrame.fill(image)
        resultYuvToArgbFrame.fill(yuv420ToArgb(rawYuvToArgbFrame.y,
            rawYuvToArgbFrame.u,
            rawYuvToArgbFrame.v,
            rawYuvToArgbFrame.yStride,
            rawYuvToArgbFrame.uStride,
            rawYuvToArgbFrame.vStride,
            rawYuvToArgbFrame.width,
            rawYuvToArgbFrame.height), rawYuvToArgbFrame.width, rawYuvToArgbFrame.height)
        return resultYuvToArgbFrame
    }

    private external fun yuv420ToArgb(y: ByteBuffer,
                                      u: ByteBuffer,
                                      v: ByteBuffer,
                                      yStride: Int,
                                      uStride: Int,
                                      vStride: Int,
                                      width: Int,
                                      height: Int): ByteArray

    fun release() {
        rawScaleFrame.free()
        resultScaleFrame.free()
        rawRotateFrame.free()
        resultRotateFrame.free()
    }
}