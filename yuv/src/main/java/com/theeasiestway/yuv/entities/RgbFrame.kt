package com.theeasiestway.yuv.entities

//
// Created by Loboda Alexey on 29.07.2020.
//

abstract class RgbFrame: Frame() {
    var dataSize: Int = 0
    var dataStride: Int = 0
}