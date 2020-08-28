package com.theeasiestway.libyuvwrapper

import android.graphics.Bitmap
import android.graphics.Paint
import android.view.Surface
import com.theeasiestway.yuv.entities.ArgbFrame
import com.theeasiestway.yuv.entities.RgbFrame
import java.lang.ref.WeakReference

object SurfaceDrawer {

    private var surfaceRef: WeakReference<Surface>? = null
    private val paint = Paint()

    fun setSurface(surface: Surface?) {
        surfaceRef = if (surface == null) null else WeakReference(surface)
    }

    fun draw(rgbFrame: RgbFrame) {
        val surface = surfaceRef?.get() ?: return
        if (!surface.isValid) return
        val canvas = surface.lockCanvas(null)
        val bytes = rgbFrame.getBytes()
        val bitmap = Bitmap.createBitmap(rgbFrame.width, rgbFrame.height, if (rgbFrame is ArgbFrame) Bitmap.Config.ARGB_8888 else Bitmap.Config.RGB_565)
        bitmap.copyPixelsFromBuffer(bytes)
        canvas.drawBitmap(bitmap, 0f, 0f, paint)
        surface.unlockCanvasAndPost(canvas)
        rgbFrame.destroy()
    }

    fun draw(bitmap: Bitmap) {
        val surface = surfaceRef?.get() ?: return
        if (!surface.isValid) return
        val canvas = surface.lockCanvas(null)
        canvas.drawBitmap(bitmap, 0f, 0f, paint)
        surface.unlockCanvasAndPost(canvas)
    }
}