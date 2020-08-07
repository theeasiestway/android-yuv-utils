package com.theeasiestway.yuv.entities

import com.theeasiestway.yuv.Constants

//
// Created by Loboda Alexey on 27.07.2020.
//

class TransformParams {

    var returnType = -1

    var scaleWidth = -1
    var scaleHeight = -1
    var scaleFilter = Constants.FILTER_NONE

    var rotationMode = -1

    var mirrorH = false
    var mirrorV = false

    fun clear() {
        returnType = -1

        scaleWidth = -1
        scaleHeight = -1
        scaleFilter = Constants.FILTER_NONE

        rotationMode = -1

        mirrorH = false
        mirrorV = false
    }
}