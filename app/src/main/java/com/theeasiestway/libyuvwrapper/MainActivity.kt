package com.theeasiestway.libyuvwrapper

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.media.Image
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.CameraSelector
import androidx.lifecycle.LifecycleOwner
import com.theeasiestway.codec_h264.camera.ControllerVideo
import com.theeasiestway.libyuvwrapper.databinding.ActivityMainBinding
import com.theeasiestway.yuv.Constants
import com.theeasiestway.yuv.SurfaceDrawer
import com.theeasiestway.yuv.YuvUtils
import com.theeasiestway.yuv.entities.Rgb565Frame
import com.theeasiestway.yuv.entities.RgbFrame

//
// Created by Loboda Alexey on 22.06.2020.
//

class MainActivity : AppCompatActivity(), LifecycleOwner {

    private val PERMISSION_CAMERA = Manifest.permission.CAMERA
    private val yuvUtils = YuvUtils()

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
    private var outputToImageView = false

    private var _binding: ActivityMainBinding? = null
    private val binding get() = _binding!!

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        _binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        with(binding) {
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

            vSurfaceView.holder.addCallback(object : SurfaceHolder.Callback {
                override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) = Unit
                override fun surfaceDestroyed(holder: SurfaceHolder) {
                    SurfaceDrawer.releaseSurface()
                }

                override fun surfaceCreated(holder: SurfaceHolder) {
                    SurfaceDrawer.setSurface(holder.surface, vSurfaceView.width, vSurfaceView.height)
                }
            })

            vCheckBoxOutput.setOnCheckedChangeListener { _, isChecked ->
                outputToImageView = isChecked
                vImageView.visibility = if (outputToImageView) View.VISIBLE else View.GONE
                vSurfaceView.visibility = if (!outputToImageView) View.VISIBLE else View.GONE
            }

            vPlay.setOnClickListener { requestPermissions() }
            vStop.setOnClickListener { stopCamera() }

            vWidth.isEnabled = false
            vWidth.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
                override fun onStartTrackingTouch(seekBar: SeekBar?) = Unit
                override fun onStopTrackingTouch(seekBar: SeekBar?) = Unit
                override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                    updateWidth(progress + 1)
                }
            })

            vHeight.isEnabled = false
            vHeight.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
                override fun onStartTrackingTouch(seekBar: SeekBar?) = Unit
                override fun onStopTrackingTouch(seekBar: SeekBar?) = Unit
                override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                    updateHeight(progress + 1)
                }
            })
            vWidth.progress = 5
            vHeight.progress = 5

            vRotate.setOnClickListener {
                rotate = when (rotate) {
                    0 -> Constants.ROTATE_90
                    90 -> Constants.ROTATE_180
                    180 -> Constants.ROTATE_270
                    else -> Constants.ROTATE_0
                }
            }

            vMirrorH.setOnClickListener { mirrorH = !mirrorH }
            vMirrorV.setOnClickListener { mirrorV = !mirrorV }
        }
    }

    override fun onPause() {
        super.onPause()
        stopCamera()
    }

    private fun updateHeight(progress: Int = 5) {
        heightCurrent = progress * height / 5
        binding.vHeightLabel.text = heightCurrent.toString()
    }

    private fun updateWidth(progress: Int = 5) {
        widthCurrent = progress * width / 5
        binding.vWidthLabel.text = widthCurrent.toString()
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
        with(binding) {
            vCheckBoxOutput.isEnabled = false
            vPlay.visibility = View.GONE
            vStop.visibility = View.VISIBLE
            ControllerVideo.subscribe(javaClass.name) { image -> processImage(image) }
            ControllerVideo.initCamera(this@MainActivity, facing, vCameraPreview)
            started = true
        }
    }

    private fun stopCamera() {
        with(binding) {
            vCheckBoxOutput.isEnabled = true
            vStop.visibility = View.GONE
            vPlay.visibility = View.VISIBLE
            vWidth.isEnabled = false
            vHeight.isEnabled = false
            vWidth.progress = 5
            vHeight.progress = 5
        }

        ControllerVideo.destroyCamera()
        started = false
        widthCurrent = 0
        heightCurrent = 0
        rotate = Constants.ROTATE_0
        mirrorH = false
        mirrorV = false
        needToUpdateWH = true
    }

    private fun processImage(image: Image) {

        width = image.width
        height = image.height

        val i420 = yuvUtils.scale(widthCurrent, heightCurrent, Constants.FILTER_BOX)
            .rotate(rotate)
            .mirrorH(mirrorH)
            .mirrorV(mirrorV)
            .getI420(image)
         val rgb = yuvUtils.getArgb(i420, 1)

        image.close()

        if(needToUpdateWH) {
            runOnUiThread {
                updateWidth()
                updateHeight()
                needToUpdateWH = false
                binding.vWidth.isEnabled = true
                binding.vHeight.isEnabled = true
            }
        }

        if (widthCurrent <= 0 || heightCurrent <= 0) {
            i420.destroy()
            rgb.destroy()
            return
        }

        renderFrame(rgb)
        i420.destroy()
    }

    private fun renderFrame(frame: RgbFrame) {
        if (!outputToImageView) {
            SurfaceDrawer.drawFrame(frame)
            frame.destroy()
        } else {
            val bitmap = frame.getBitmap()
            runOnUiThread {
                binding.vImageView.setImageBitmap(bitmap)
                frame.destroy()
            }
        }
    }
}