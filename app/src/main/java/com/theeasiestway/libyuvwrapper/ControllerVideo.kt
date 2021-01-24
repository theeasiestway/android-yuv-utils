package com.theeasiestway.codec_h264.camera

import android.annotation.SuppressLint
import android.content.Context
import android.content.res.Configuration
import android.hardware.display.DisplayManager
import android.media.Image
import android.util.Log
import android.util.Size
import android.widget.Toast
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.core.content.ContextCompat
import androidx.lifecycle.LifecycleOwner
import com.google.common.util.concurrent.ListenableFuture
import java.lang.ref.WeakReference
import java.util.concurrent.Executors

//
// Created by Loboda Alexey on 22.06.2020.
//

object ControllerVideo {

    private lateinit var cameraProviderFuture: ListenableFuture<ProcessCameraProvider>
    private lateinit var cameraProvider: ProcessCameraProvider
    private lateinit var displayManager: DisplayManager

    private val executor = Executors.newSingleThreadExecutor()
    private var subscriptions = HashMap<String, (Image) -> Unit>()
    private var isLandscape = false
    private var rotationCurrent = 0
    private var displayId = -1
    private var released = false

    fun initCamera(context: Context, @CameraSelector.LensFacing lensFacing: Int, previewView: PreviewView) {
        val lifecycleOwner = WeakReference<LifecycleOwner>(context as LifecycleOwner)
        destroyCamera()
        cameraProviderFuture = ProcessCameraProvider.getInstance(context).apply {
            addListener(Runnable {
                cameraProvider = cameraProviderFuture.get()
                val preview = Preview.Builder().setTargetName("CameraPreview").build()
                preview.setSurfaceProvider(previewView.surfaceProvider)
                startCamera(lifecycleOwner, cameraProvider, preview, lensFacing, displayId = previewView.display.displayId, onError = {
                    Toast.makeText(context, "Unable to init camera", Toast.LENGTH_LONG).show()
                })
            }, ContextCompat.getMainExecutor(context))
        }
    }

    @SuppressLint("UnsafeExperimentalUsageError")
    private fun startCamera(lifecycleOwner: WeakReference<LifecycleOwner>, cameraProvider: ProcessCameraProvider, preview: Preview, facing: Int, displayId: Int, onError: () -> Unit) {
        try {
            cameraProvider.unbindAll()

            if (lifecycleOwner.get() == null) return

            displayManager = (lifecycleOwner.get() as Context).getSystemService(Context.DISPLAY_SERVICE) as DisplayManager
            displayManager.registerDisplayListener(rotationListener, null)

            this.displayId = displayId

            rotationCurrent = displayManager.getDisplay(displayId).rotation

            isLandscape = (lifecycleOwner.get() as Context).resources.configuration.orientation == Configuration.ORIENTATION_LANDSCAPE

            val cameraSelector = CameraSelector.Builder().requireLensFacing(facing).build()

            val imageAnalysis = ImageAnalysis.Builder()
                .setTargetResolution(Size(1920, 1080))
                .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                .build()
            imageAnalysis.setAnalyzer(executor, ImageAnalysis.Analyzer { imageProxy ->
                if (imageProxy.image == null || imageProxy.planes.size < 3) return@Analyzer

                for (i in subscriptions) i.value.invoke(imageProxy.image!!)

                imageProxy.close()
            })

            released = false

            cameraProvider.bindToLifecycle(lifecycleOwner.get()!!, cameraSelector, preview, imageAnalysis)
        } catch (e: Exception) {
            cameraProvider.unbindAll()
            released = true
            onError.invoke()
        }
    }

    object rotationListener : DisplayManager.DisplayListener {
        override fun onDisplayAdded(displayId: Int) = Unit
        override fun onDisplayRemoved(displayId: Int) = Unit
        override fun onDisplayChanged(displayId: Int) {
            if (ControllerVideo.displayId == displayId)
                rotationCurrent = displayManager.getDisplay(displayId).rotation
        }
    }

    fun subscribe(tag: String, onFrame: (Image) -> Unit) {
        subscriptions.remove(tag)
        subscriptions[tag] = onFrame
    }

    fun unsubscribe(tag: String) {
        subscriptions.remove(tag)
    }

    fun destroyCamera() {
        if (this::cameraProvider.isInitialized) {
            try { cameraProvider.unbindAll() }
            catch (e: Exception) { Log.e("ControllerVideo", "[destroyCamera] error: $e") }
        }
        if (this::displayManager.isInitialized) displayManager.unregisterDisplayListener(rotationListener)
        released = true
    }
}