package com.theeasiestway.yuv

import android.annotation.SuppressLint
import android.media.Image
import android.util.Log
import com.theeasiestway.yuv.entities.*
import java.lang.ref.WeakReference
import java.nio.ByteBuffer
import kotlin.reflect.KClass

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

    fun rotate(rotationMode: Int): YuvUtils {
        require(rotationMode == 0 || rotationMode == 90 || rotationMode == 180 || rotationMode == 270) { "rotationMode: $rotationMode is not unsupported. Check supported values in com.theeasiestway.libyuv.Constant.kt" }
        transformParams.rotationMode = rotationMode
        return this
    }

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

    fun mirrorH(): YuvUtils {
        transformParams.mirrorH = true
        return this
    }

    fun mirrorH(image: Image): YuvFrame {
        val outFrame = FramesFactory.instanceYuv(image.width, image.height)
        mirrorH(image.planes[0].buffer,
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

    fun mirrorH(yuvFrame: YuvFrame): YuvFrame {
        val outFrame = FramesFactory.instanceYuv(yuvFrame.width, yuvFrame.height)
        mirrorH(yuvFrame.y,
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

    fun mirrorV(): YuvUtils {
        transformParams.mirrorV = true
        return this
    }

    fun mirrorV(image: Image): YuvFrame {
        val outFrame = FramesFactory.instanceYuv(image.width, image.height)
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
            -image.height,
            Constants.ROTATE_0)
        return outFrame
    }

    fun mirrorV(yuvFrame: YuvFrame): YuvFrame {
        val outFrame = FramesFactory.instanceYuv(yuvFrame.width, yuvFrame.height)
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
            -yuvFrame.height,
            Constants.ROTATE_0)
        return outFrame
    }

    private external fun mirrorH(y: ByteBuffer,
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

    //
    // Convert Android YUV to libyuv YUV
    //

    fun getYUV(from: Image): YuvFrame {
        transformParams.returnType = transformParams.return_YUV
        return transform(from)
    }

    fun getI420(from: Image): YuvFrame {
        transformParams.returnType = transformParams.return_I420
        return transform(from)
    }

    fun getI420(from: YuvFrame): YuvFrame {
        transformParams.returnType = transformParams.return_I420
        return transform(from)
    }

    fun getArgb(from: Image): ArgbFrame {
        transformParams.returnType = transformParams.return_ARGB
        return transform(from)
    }

    fun getArgb(from: YuvFrame): ArgbFrame {
        transformParams.returnType = transformParams.return_ARGB
        return transform(from)
    }

    fun getRgb565(from: Image): Rgb565Frame {
        transformParams.returnType = transformParams.return_RGB565
        return transform(from)
    }

    fun getRgb565(from: YuvFrame): Rgb565Frame {
        transformParams.returnType = transformParams.return_RGB565
        return transform(from)
    }

    fun convertToI420(image: Image): YuvFrame {
        val outFrame = FramesFactory.instanceYuv(image.width, image.height)
        convertToI420(image.planes[0].buffer,
            image.planes[1].buffer,
            image.planes[2].buffer,
            image.planes[0].rowStride,
            image.planes[1].rowStride,
            image.planes[2].rowStride,
            image.planes[2].pixelStride,
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

    fun convertToI420(yuvFrame: YuvFrame, uvPixelStride: Int): YuvFrame {
        val outFrame = FramesFactory.instanceYuv(yuvFrame.width, yuvFrame.height)
        convertToI420(yuvFrame.y,
            yuvFrame.u,
            yuvFrame.v,
            yuvFrame.yStride,
            yuvFrame.uStride,
            yuvFrame.vStride,
            uvPixelStride,
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

    private external fun convertToI420(y: ByteBuffer,
                                       u: ByteBuffer,
                                       v: ByteBuffer,
                                       yStride: Int,
                                       uStride: Int,
                                       vStride: Int,
                                       srcPixelStrideUv: Int,
                                       yOut: ByteBuffer,
                                       uOut: ByteBuffer,
                                       vOut: ByteBuffer,
                                       yOutStride: Int,
                                       uOutStride: Int,
                                       vOutStride: Int,
                                       width: Int,
                                       height: Int)

    private fun transform(image: Image): Frame {
        return transformNative(image.planes[0].buffer,
            image.planes[1].buffer,
            image.planes[2].buffer,
            image.planes[0].rowStride,
            image.planes[1].rowStride,
            image.planes[2].rowStride,
            transformParams.scaleWidth,
            transformParams.scaleHeight,
            transformParams.scaleFilter,
            transformParams.rotationMode,
            transformParams.mirrorH,
            transformParams.mirrorV,
            transformParams.returnType)
    }

    private fun transform(yuvFrame: YuvFrame): Frame {
        return transformNative(yuvFrame.y,
            yuvFrame.u,
            yuvFrame.v,
            yuvFrame.yStride,
            yuvFrame.uStride,
            yuvFrame.vStride,
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