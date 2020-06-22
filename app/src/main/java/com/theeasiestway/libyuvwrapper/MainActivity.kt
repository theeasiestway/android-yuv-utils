package com.theeasiestway.libyuvwrapper

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.*
import android.media.Image
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.ImageView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.CameraSelector
import androidx.camera.view.PreviewView
import androidx.lifecycle.LifecycleOwner
import com.theeasiestway.codec_h264.camera.ControllerVideo
import com.theeasiestway.libyuv.Constants
import com.theeasiestway.libyuv.YuvUtils
import java.io.ByteArrayOutputStream


//
// Created by Loboda Alexey on 22.06.2020.
//

class MainActivity : AppCompatActivity(), LifecycleOwner {

    private val PERMISSION_CAMERA = Manifest.permission.CAMERA
    private val PERMISSION_WRITE = Manifest.permission.WRITE_EXTERNAL_STORAGE
    private val yuvUtils = YuvUtils()
    private lateinit var vCameraView: PreviewView
    private lateinit var vImageView: ImageView
    private lateinit var vPlay: Button
    private lateinit var vStop: Button
    private var saved = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        vCameraView = findViewById(R.id.vCameraPreview)
        vImageView = findViewById(R.id.vImageView)
        vPlay = findViewById(R.id.vPlay)
        vPlay.setOnClickListener { requestPermissions() }
        vStop = findViewById(R.id.vStop)
        vStop.setOnClickListener {
            vStop.visibility = View.GONE
            vPlay.visibility = View.VISIBLE
            ControllerVideo.destroyCamera()
            saved = false
        }

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
        ControllerVideo.initCamera(this, CameraSelector.LENS_FACING_FRONT, vCameraView)
    }

    private fun processImage(image: Image) {

        Log.d("efwfe", "format: ${image.format}")

        val result = yuvUtils.scale(image, 1024, 768, Constants.FILTER_NONE)

        val yuvImage = YuvImage(result, ImageFormat.NV21, 1024, 768, null)

        val out = ByteArrayOutputStream()
        yuvImage.compressToJpeg(Rect(0, 0, 1024, 768), 50, out)
        val imageBytes: ByteArray = out.toByteArray()
        val bm = BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.size)
        vImageView.post { vImageView.setImageBitmap(bm) }
    }


    /* if (saved) return

     val rootFolder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)
     val file = File(rootFolder, "image.yuv")
     if (!file.exists()) file.createNewFile()

     val fos = FileOutputStream(file)
     fos.write(result)
     fos.flush()
     fos.close()

     saved = true

    Log.d("edwee", "result size: ${result.size}")
} */
}