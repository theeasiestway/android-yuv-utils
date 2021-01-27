package com.theeasiestway.libyuvwrapper

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.*
import android.media.Image
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.CameraSelector
import androidx.camera.view.PreviewView
import androidx.lifecycle.LifecycleOwner
import com.theeasiestway.yuv.SurfaceDrawer
import com.theeasiestway.codec_h264.camera.ControllerVideo
import com.theeasiestway.yuv.Constants
import com.theeasiestway.yuv.YuvUtils
import com.theeasiestway.yuv.entities.RgbFrame
import com.theeasiestway.yuv.entities.YuvFrame
import java.io.ByteArrayOutputStream

//
// Created by Loboda Alexey on 22.06.2020.
//

class MainActivity : AppCompatActivity(), LifecycleOwner {

    private val PERMISSION_CAMERA = Manifest.permission.CAMERA
    private val yuvUtils = YuvUtils()
    private lateinit var vCameraFacing: ImageView
    private lateinit var vCameraView: PreviewView
    private lateinit var vSurfaceView: SurfaceView
    private lateinit var vPlay: Button
    private lateinit var vStop: Button
    private lateinit var vWidth: SeekBar
    private lateinit var vWidthLabel: TextView
    private lateinit var vHeight: SeekBar
    private lateinit var vHeightLabel: TextView
    private lateinit var vRotate: Button
    private lateinit var vMirrorH: Button
    private lateinit var vMirrorV: Button

    private var facing = CameraSelector.LENS_FACING_FRONT
    private var widthCurrent = 0
    private var heightCurrent = 0
    private var width = 0
    private var height = 0
    private var rotate = 0
    private var mirrorH = false
    private var mirrorV = false
    private var started = false
    private var needToUpdateWH = true

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        vCameraFacing = findViewById(R.id.vCameraFacing)
        vCameraFacing.setImageResource(R.drawable.ic_camera_rear_24dp)
        vCameraFacing.setOnClickListener {
            facing = if (facing == CameraSelector.LENS_FACING_FRONT) {
                vCameraFacing.setImageResource(R.drawable.ic_camera_front_24dp)
                CameraSelector.LENS_FACING_BACK
            } else {
                vCameraFacing.setImageResource(R.drawable.ic_camera_rear_24dp)
                CameraSelector.LENS_FACING_FRONT
            }
            if (started) {
                stopCamera()
                requestPermissions()
            }
        }

        vCameraView = findViewById(R.id.vCameraPreview)
        vSurfaceView = findViewById(R.id.vSurfaceView)
        vSurfaceView.holder.addCallback(object: SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) = Unit
            override fun surfaceDestroyed(holder: SurfaceHolder) { SurfaceDrawer.releaseSurface() }
            override fun surfaceCreated(holder: SurfaceHolder) {
                val surface = holder.surface ?: return
                SurfaceDrawer.setSurface(surface, vSurfaceView.width, vSurfaceView.height)
            }


        })

        vPlay = findViewById(R.id.vPlay)
        vPlay.setOnClickListener { requestPermissions() }

        vStop = findViewById(R.id.vStop)
        vStop.setOnClickListener { stopCamera() }

        vWidthLabel = findViewById(R.id.vWidthLabel)
        vHeightLabel = findViewById(R.id.vHeightLabel)

        vWidth = findViewById(R.id.vWidth)
        vWidth.isEnabled = false
        vWidth.setOnSeekBarChangeListener(object: SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) { updateWidth(progress) }
            override fun onStartTrackingTouch(seekBar: SeekBar?) = Unit
            override fun onStopTrackingTouch(seekBar: SeekBar?) = Unit
        })

        vHeight = findViewById(R.id.vHeight)
        vHeight.isEnabled = false
        vHeight.setOnSeekBarChangeListener(object: SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) { updateHeight(progress) }
            override fun onStartTrackingTouch(seekBar: SeekBar?) = Unit
            override fun onStopTrackingTouch(seekBar: SeekBar?) = Unit
        })

        vRotate = findViewById(R.id.vRotate)
        vRotate.setOnClickListener { rotate = when(rotate) {
            0 -> Constants.ROTATE_90
            90 -> Constants.ROTATE_180
            180 -> Constants.ROTATE_270
            else -> Constants.ROTATE_0
        }}

        vMirrorH = findViewById(R.id.vMirrorH)
        vMirrorH.setOnClickListener { mirrorH = !mirrorH }

        vMirrorV = findViewById(R.id.vMirrorV)
        vMirrorV.setOnClickListener { mirrorV = !mirrorV }
    }

    private fun updateHeight(progress: Int = 0) {
        heightCurrent = height + progress * 100
        vHeightLabel.text = heightCurrent.toString()
    }

    private fun updateWidth(progress: Int = 0) {
        widthCurrent = width + progress * 100
        vWidthLabel.text = widthCurrent.toString()
    }

    private fun requestPermissions() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) requestPermissions(arrayOf(PERMISSION_CAMERA), 1)
        else startCamera()
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        if (grantResults.isNotEmpty() && requestCode == 1 && permissions[0] == PERMISSION_CAMERA && grantResults[0] == PackageManager.PERMISSION_GRANTED)
            startCamera()
        else Toast.makeText(this, "App doesn't have enough permissions to continue", Toast.LENGTH_LONG).show()
    }

    private fun startCamera() {
        vPlay.visibility = View.GONE
        vStop.visibility = View.VISIBLE
        ControllerVideo.subscribe(javaClass.name) { image -> processImage(image) }
        ControllerVideo.initCamera(this, facing, vCameraView)
        started = true
    }

    private fun stopCamera() {
        vStop.visibility = View.GONE
        vPlay.visibility = View.VISIBLE
        vWidth.isEnabled = false
        vHeight.isEnabled = false
        ControllerVideo.destroyCamera()
        started = false
        vWidth.progress = 0
        vHeight.progress = 0
        rotate = Constants.ROTATE_0
        mirrorH = false
        mirrorV = false
        needToUpdateWH = true
    }

    var maxTime = 0f

    private fun processImage(image: Image) {

        val startTime1 = System.currentTimeMillis()

        width = image.width
        height = image.height

        val i420 = yuvUtils.scale(widthCurrent, heightCurrent, Constants.FILTER_BOX)
            .rotate(rotate)
            .mirrorH(mirrorH)
            .mirrorV(mirrorV)
            .getI420(image)
         val rgb = yuvUtils.getRgb565(i420, 1)

        //i420.destroy()

        // Checked for memory leaks (getI420->getRgb565) and there are no them in:
        // 1. scale
        // 2. getI420
        // 3. getRgb565
        // 4. rotate
        // 5. mirrorH
        // 6. mirrorV - was 350-400 mb memory usage
        // 7. all together

        // before optimisation get frame time was: 108.0 ms.

        if (maxTime < (System.currentTimeMillis() - startTime1).toFloat()) {
            maxTime = (System.currentTimeMillis() - startTime1).toFloat()
            Log.d("erfrf", "get frame time: $maxTime ms.")
        }

        image.close()

        if(needToUpdateWH) {
            runOnUiThread {
                updateWidth()
                updateHeight()
                needToUpdateWH = false
                vWidth.isEnabled = true
                vHeight.isEnabled = true
            }
        }

        if (widthCurrent <= 0 || heightCurrent <= 0) {
            i420.destroy()
            rgb.destroy()
            return
        }

        SurfaceDrawer.drawFrame(rgb)
        i420.destroy()
        rgb.destroy()
    }
}