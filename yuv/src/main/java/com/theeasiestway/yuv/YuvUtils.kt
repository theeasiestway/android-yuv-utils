package com.theeasiestway.yuv

import android.media.Image
import android.util.Log
import com.theeasiestway.yuv.entities.ArgbFrame
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

    fun scale(image: Image, dstWidth: Int, dstHeight: Int, filerMode: Int): YuvFrame {
        require(filerMode in 0..3) { "filterMode: $filerMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        val outFrame = FramesFactory.instanceYuv(dstWidth, dstHeight)
        scale(image.planes[0].buffer,
            image.planes[1].buffer,
            image.planes[2].buffer,
            image.planes[0].rowStride,
            image.planes[1].rowStride,
            image.planes[2].rowStride,
            outFrame.y,
            outFrame.u,
            outFrame.v,
            outFrame.yStride,
            outFrame.uStride,
            outFrame.vStride,
            image.width,
            image.height,
            dstWidth,
            dstHeight,
            filerMode)
        return outFrame
    }

    fun scale(yuvFrame: YuvFrame, dstWidth: Int, dstHeight: Int, filerMode: Int): YuvFrame {
        require(filerMode in 0..3) { "filterMode: $filerMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        val outFrame = FramesFactory.instanceYuv(dstWidth, dstHeight)
        scale(yuvFrame.y,
            yuvFrame.u,
            yuvFrame.v,
            yuvFrame.yStride,
            yuvFrame.uStride,
            yuvFrame.vStride,
            outFrame.y,
            outFrame.u,
            outFrame.v,
            outFrame.yStride,
            outFrame.uStride,
            outFrame.vStride,
            yuvFrame.width,
            yuvFrame.height,
            dstWidth,
            dstHeight,
            filerMode)
        return outFrame
    }

    private external fun scale(y: ByteBuffer,
                               u: ByteBuffer,
                               v: ByteBuffer,
                               yStride: Int,
                               uStride: Int,
                               vStride: Int,
                               yOut: ByteBuffer,
                               uOut: ByteBuffer,
                               vOut: ByteBuffer,
                               yOutStride: Int,
                               uOutStride: Int,
                               vOutStride: Int,
                               srcWidth: Int,
                               srcHeight: Int,
                               dstWidth: Int,
                               dstHeight: Int,
                               filterMode: Int)

    //
    // Rotate
    //

    fun rotate(image: Image, rotationMode: Int): YuvFrame {
        require(rotationMode == 0 || rotationMode == 90 || rotationMode == 180 || rotationMode == 270) { "rotationMode: $rotationMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        val outFrame = FramesFactory.instanceYuv(image.width, image.height, rotationMode)
        rotate(image.planes[0].buffer,
            image.planes[1].buffer,
            image.planes[2].buffer,
            image.planes[0].rowStride,
            image.planes[1].rowStride,
            image.planes[2].rowStride,
            outFrame.y,
            outFrame.u,
            outFrame.v,
            outFrame.yStride,
            outFrame.uStride,
            outFrame.vStride,
            image.width,
            image.height,
            rotationMode)
        return outFrame
    }

    fun rotate(yuvFrame: YuvFrame, rotationMode: Int): YuvFrame {
        require(rotationMode == 0 || rotationMode == 90 || rotationMode == 180 || rotationMode == 270) { "rotationMode: $rotationMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        val outFrame = FramesFactory.instanceYuv(yuvFrame.width, yuvFrame.height, rotationMode)
        rotate(yuvFrame.y,
            yuvFrame.u,
            yuvFrame.v,
            yuvFrame.yStride,
            yuvFrame.uStride,
            yuvFrame.vStride,
            outFrame.y,
            outFrame.u,
            outFrame.v,
            outFrame.yStride,
            outFrame.uStride,
            outFrame.vStride,
            yuvFrame.width,
            yuvFrame.height,
            rotationMode)
        return outFrame
    }

    private external fun rotate(y: ByteBuffer,
                                u: ByteBuffer,
                                v: ByteBuffer,
                                yStride: Int,
                                uStride: Int,
                                vStride: Int,
                                yOut: ByteBuffer,
                                uOut: ByteBuffer,
                                vOut: ByteBuffer,
                                yOutStride: Int,
                                uOutStride: Int,
                                vOutStride: Int,
                                width: Int,
                                height: Int,
                                rotationMode: Int)

    //
    // Mirror
    //

    fun mirror(image: Image): YuvFrame {
        val outFrame = FramesFactory.instanceYuv(image.width, image.height)
        mirror(image.planes[0].buffer,
            image.planes[1].buffer,
            image.planes[2].buffer,
            image.planes[0].rowStride,
            image.planes[1].rowStride,
            image.planes[2].rowStride,
            outFrame.y,
            outFrame.u,
            outFrame.v,
            outFrame.yStride,
            outFrame.uStride,
            outFrame.vStride,
            image.width,
            image.height)
        return outFrame
    }

    fun mirror(yuvFrame: YuvFrame): YuvFrame {
        val outFrame = FramesFactory.instanceYuv(yuvFrame.width, yuvFrame.height)
        mirror(yuvFrame.y,
            yuvFrame.u,
            yuvFrame.v,
            yuvFrame.yStride,
            yuvFrame.uStride,
            yuvFrame.vStride,
            outFrame.y,
            outFrame.u,
            outFrame.v,
            outFrame.yStride,
            outFrame.uStride,
            outFrame.vStride,
            yuvFrame.width,
            yuvFrame.height)
        return outFrame
    }

    private external fun mirror(y: ByteBuffer,
                                u: ByteBuffer,
                                v: ByteBuffer,
                                yStride: Int,
                                uStride: Int,
                                vStride: Int,
                                yOut: ByteBuffer,
                                uOut: ByteBuffer,
                                vOut: ByteBuffer,
                                yOutStride: Int,
                                uOutStride: Int,
                                vOutStride: Int,
                                width: Int,
                                height: Int)

    //
    // YUV to ARGB
    //

    fun yuv420ToArgb(image: Image): ArgbFrame {
        val outFrame = FramesFactory.instanceArgb(image.width, image.height)
        yuv420ToArgb(image.planes[0].buffer,
            image.planes[1].buffer,
            image.planes[2].buffer,
            image.planes[0].rowStride,
            image.planes[1].rowStride,
            image.planes[2].rowStride,
            outFrame.data,
            outFrame.dataStride,
            image.width,
            image.height)
        return outFrame
    }

    fun yuv420ToArgb(yuvFrame: YuvFrame): ArgbFrame {
        val outFrame = FramesFactory.instanceArgb(yuvFrame.width, yuvFrame.height)
        yuv420ToArgb(yuvFrame.y,
            yuvFrame.u,
            yuvFrame.v,
            yuvFrame.yStride,
            yuvFrame.uStride,
            yuvFrame.vStride,
            outFrame.data,
            outFrame.dataStride,
            yuvFrame.width,
            yuvFrame.height)
        return outFrame
    }

    private external fun yuv420ToArgb(y: ByteBuffer,
                                      u: ByteBuffer,
                                      v: ByteBuffer,
                                      yStride: Int,
                                      uStride: Int,
                                      vStride: Int,
                                      out: ByteBuffer,
                                      outStride: Int,
                                      width: Int,
                                      height: Int)
}