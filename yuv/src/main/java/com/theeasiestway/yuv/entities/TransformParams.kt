package com.theeasiestway.yuv.entities

import com.theeasiestway.yuv.Constants

//
// Created by Loboda Alexey on 27.07.2020.
//

class TransformParams {

    val return_YUV = 0
    val return_I420 = 1
    val return_ARGB = 2
    val return_RGB565 = 3
    var returnType = -1

    var scaleWidth = -1
    var scaleHeight = -1
    var scaleFilter = Constants.FILTER_NONE

    var rotationMode = Constants.ROTATE_0

    var mirrorH = false
    var mirrorV = false

    fun clear() {
        returnType = -1

        scaleWidth = -1
        scaleHeight = -1
        scaleFilter = Constants.FILTER_NONE

        rotationMode = Constants.ROTATE_0

        mirrorH = false
        mirrorV = false
    }
}