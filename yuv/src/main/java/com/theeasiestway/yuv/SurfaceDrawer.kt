package com.theeasiestway.yuv

import android.view.Surface
import com.theeasiestway.yuv.entities.RgbFrame

object SurfaceDrawer {

    fun setSurface(surface: Surface, width: Int, height: Int) {
        releaseSurface()
        setSurfaceNative(
            surface,
            width,
            height
        )
    }

    fun drawFrame(rgbFrame: RgbFrame) {
        drawFrameNative(rgbFrame.nativePointer)
    }

    private external fun setSurfaceNative(surface: Surface, width: Int, height: Int)

    private external fun drawFrameNative(pointer: Long)

    external fun releaseSurface()
}