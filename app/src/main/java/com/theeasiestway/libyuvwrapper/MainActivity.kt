package com.theeasiestway.libyuvwrapper

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.*
import android.media.Image
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.CameraSelector
import androidx.camera.view.PreviewView
import androidx.lifecycle.LifecycleOwner
import com.theeasiestway.codec_h264.camera.ControllerVideo
import com.theeasiestway.yuv.Constants
import com.theeasiestway.yuv.YuvUtils
import java.io.ByteArrayOutputStream
import java.nio.ByteBuffer


//
// Created by Loboda Alexey on 22.06.2020.
//

class MainActivity : AppCompatActivity(), LifecycleOwner {

    private val PERMISSION_CAMERA = Manifest.permission.CAMERA
    private val PERMISSION_WRITE = Manifest.permission.WRITE_EXTERNAL_STORAGE
    private val yuvUtils = YuvUtils()
    private lateinit var vCameraFacing: ImageView
    private lateinit var vCameraView: PreviewView
    private lateinit var vImageView: ImageView
    private lateinit var vPlay: Button
    private lateinit var vStop: Button
    private lateinit var vWidth: SeekBar
    private lateinit var vWidthLabel: TextView
    private lateinit var vHeight: SeekBar
    private lateinit var vHeightLabel: TextView
    private lateinit var vRotate: Button
    private lateinit var vMirror: Button

    private var facing = CameraSelector.LENS_FACING_FRONT
    private var width = 0
    private var height = 0
    private var rotate = 0
    private var mirror = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        vCameraFacing = findViewById(R.id.vCameraFacing)
        vCameraFacing.setImageResource(R.drawable.ic_camera_front_24dp)
        vCameraFacing.setOnClickListener {
            facing = if (facing == CameraSelector.LENS_FACING_FRONT) {
                vCameraFacing.setImageResource(R.drawable.ic_camera_rear_24dp)
                CameraSelector.LENS_FACING_BACK
            } else {
                vCameraFacing.setImageResource(R.drawable.ic_camera_front_24dp)
                CameraSelector.LENS_FACING_FRONT
            }
        }

        vCameraView = findViewById(R.id.vCameraPreview)
        vImageView = findViewById(R.id.vImageView)

        vPlay = findViewById(R.id.vPlay)
        vPlay.setOnClickListener { requestPermissions() }

        vStop = findViewById(R.id.vStop)
        vStop.setOnClickListener {
            vStop.visibility = View.GONE
            vPlay.visibility = View.VISIBLE
            ControllerVideo.destroyCamera()
        }

        vWidthLabel = findViewById(R.id.vWidthLabel)
        vHeightLabel = findViewById(R.id.vHeightLabel)

        vWidth = findViewById(R.id.vWidth)
        vWidth.setOnSeekBarChangeListener(object: SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                width = 640 + progress * 100
                vWidthLabel.text = width.toString()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) = Unit
            override fun onStopTrackingTouch(seekBar: SeekBar?) = Unit
        })
        vWidth.progress = 0

        vHeight = findViewById(R.id.vHeight)
        vHeight.setOnSeekBarChangeListener(object: SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                height = 480 + progress * 100
                vHeightLabel.text = height.toString()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) = Unit
            override fun onStopTrackingTouch(seekBar: SeekBar?) = Unit
        })
        vHeight.progress = 0

        vRotate = findViewById(R.id.vRotate)
        vRotate.setOnClickListener { rotate = when(rotate) {
            0 -> 90
            90 -> 180
            180 -> 270
            else -> 0
        }}

        vMirror = findViewById(R.id.vMirror)
        vMirror.setOnClickListener { mirror = !mirror }
    }

    private fun requestPermissions() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) requestPermissions(arrayOf(PERMISSION_CAMERA, PERMISSION_WRITE), 1)
        else startCamera()
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        if (requestCode == 1 &&
            permissions[0] == PERMISSION_CAMERA && grantResults[0] == PackageManager.PERMISSION_GRANTED &&
            permissions[1] == PERMISSION_WRITE && grantResults[1] == PackageManager.PERMISSION_GRANTED)
            startCamera()
        else Toast.makeText(this, "App doesn't have enough permissions to continue", Toast.LENGTH_LONG).show()
    }

    private fun startCamera() {
        vPlay.visibility = View.GONE
        vStop.visibility = View.VISIBLE
        ControllerVideo.subscribe(javaClass.name) { image -> processImage(image) }
        ControllerVideo.initCamera(this, facing, vCameraView)
    }

    private fun processImage(image: Image) {

        //    val result = yuvUtils.yuv420ToArgb(image)

        //    val result = yuvUtils.mirror(image)

        val result = yuvUtils.scale(image, width, height, Constants.FILTER_BOX)

        //    val result = yuvUtils.rotate(image, Constants.ROTATE_90)

        val yuvImage = YuvImage(result.asArray(), ImageFormat.NV21, result.width, result.height, null)
        val out = ByteArrayOutputStream()
        yuvImage.compressToJpeg(Rect(0, 0, result.width, result.height), 50, out)
        val imageBytes: ByteArray = out.toByteArray()
        val bm = BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.size) // for displaying yuv

        /* val bm = Bitmap.createBitmap(result.width, result.height, Bitmap.Config.ARGB_8888)
        bm.copyPixelsFromBuffer(ByteBuffer.wrap(result.asArray())) */ // for displaying argb

        vImageView.post { vImageView.setImageBitmap(bm) }
    }
}