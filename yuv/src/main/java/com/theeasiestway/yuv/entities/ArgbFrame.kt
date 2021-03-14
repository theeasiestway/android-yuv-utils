package com.theeasiestway.yuv.entities

import android.graphics.Bitmap
import com.theeasiestway.yuv.Constants

//
// Created by Loboda Alexey on 29.07.2020.
//

class ArgbFrame: RgbFrame() {

    companion object {
        private var bitmap: Bitmap? = null
        private val config = Bitmap.Config.ARGB_8888
    }

    override fun getType(): Int = Constants.ARGB

    override fun getBitmapSingle(): Bitmap? {
        val bytes = getBytes()
        if (bytes.remaining() <= 0) return null

        if (bitmap == null || bitmap!!.width != width || bitmap!!.height != height) {
            bitmap = Bitmap.createBitmap(width, height, config).apply {
                copyPixelsFromBuffer(bytes)
            }
        } else {
            bitmap!!.copyPixelsFromBuffer(bytes)
        }
        return bitmap
    }

    override fun getBitmap(isMutable: Boolean): Bitmap? {
        return getBitmapSingle()?.copy(config, isMutable)
    }

    override fun releaseBitmapSingle() {
        bitmap?.recycle()
        bitmap = null
    }
}