package com.theeasiestway.yuv

import android.media.Image
import android.util.Log
import com.theeasiestway.yuv.entities.*
import java.nio.ByteBuffer

//
// Created by Loboda Alexey on 22.06.2020.
//

class YuvUtils {

    companion object {

        val TAG = "YuvUtils"
        val transformParams = TransformParams()

        init {
            try { System.loadLibrary("easyyuv") }
            catch (e: Exception) { Log.e(TAG, "Couldn't load easyyuv library: $e") }
        }
    }

    //
    // Scale
    //

    fun scale(dstWidth: Int, dstHeight: Int, filerMode: Int): YuvUtils {
        require(filerMode in 0..3) { "filterMode: $filerMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        transformParams.scaleWidth = dstWidth
        transformParams.scaleHeight = dstHeight
        transformParams.scaleFilter = filerMode
        return this
    }

    //
    // Rotate
    //

    fun rotate(rotationMode: Int): YuvUtils {
        require(rotationMode == 0 || rotationMode == 90 || rotationMode == 180 || rotationMode == 270) { "rotationMode: $rotationMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        transformParams.rotationMode = rotationMode
        return this
    }

    //
    // Mirror
    //

    fun mirrorH(b: Boolean): YuvUtils {
        transformParams.mirrorH = b
        return this
    }

    fun mirrorV(b: Boolean): YuvUtils {
        transformParams.mirrorV = b
        return this
    }

    //
    // Convert Android YUV to libyuv YUV
    //

    fun getYUV(from: Image): YuvFrame {
        transformParams.returnType = Constants.YUV
        return transform(from) as YuvFrame
    }

    fun getI420(from: Image): YuvFrame {
        transformParams.returnType = Constants.I420
        return transform(from) as YuvFrame
    }

    fun getI420(from: YuvFrame, uvPixelStride: Int): YuvFrame {
        transformParams.returnType = Constants.I420
        return transform(from, uvPixelStride) as YuvFrame
    }

    //
    // Convert to ARGB
    //

    fun getArgb(from: Image): ArgbFrame {
        transformParams.returnType = Constants.ARGB
        return transform(from) as ArgbFrame
    }

    fun getArgb(from: YuvFrame, uvPixelStride: Int): ArgbFrame {
        transformParams.returnType = Constants.ARGB
        return transform(from, uvPixelStride) as ArgbFrame
    }

    //
    // Convert RGB565
    //

    fun getRgb565(from: Image): Rgb565Frame {
        transformParams.returnType = Constants.RGB565
        return transform(from) as Rgb565Frame
    }

    fun getRgb565(from: YuvFrame, uvPixelStride: Int): Rgb565Frame {
        transformParams.returnType = Constants.RGB565
        return transform(from, uvPixelStride) as Rgb565Frame
    }

    //
    // Support
    //

    private fun transform(image: Image): Frame {
        return transformNative(image.planes[0].buffer,
            image.planes[1].buffer,
            image.planes[2].buffer,
            image.planes[0].rowStride,
            image.planes[1].rowStride,
            image.planes[2].rowStride,
            image.planes[2].pixelStride,
            image.width,
            image.height,
            transformParams.scaleWidth,
            transformParams.scaleHeight,
            transformParams.scaleFilter,
            transformParams.rotationMode,
            transformParams.mirrorH,
            transformParams.mirrorV,
            transformParams.returnType)
    }

    private fun transform(yuvFrame: YuvFrame, uvPixelStride: Int): Frame {
        return transformNative(yuvFrame.y,
            yuvFrame.u,
            yuvFrame.v,
            yuvFrame.yStride,
            yuvFrame.uStride,
            yuvFrame.vStride,
            uvPixelStride,
            yuvFrame.width,
            yuvFrame.height,
            transformParams.scaleWidth,
            transformParams.scaleHeight,
            transformParams.scaleFilter,
            transformParams.rotationMode,
            transformParams.mirrorH,
            transformParams.mirrorV,
            transformParams.returnType)
    }

    private external fun transformNative(y: ByteBuffer,
                                         u: ByteBuffer,
                                         v: ByteBuffer,
                                         yStride: Int,
                                         uStride: Int,
                                         vStride: Int,
                                         uvPixelStride: Int,
                                         width: Int,
                                         height: Int,
                                         scaleWidth: Int,
                                         scaleHeight: Int,
                                         scaleFilter: Int,
                                         rotationMode: Int,
                                         mirrorH: Boolean,
                                         mirrorV: Boolean,
                                         returnType: Int): Frame
}