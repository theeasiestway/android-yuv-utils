package com.theeasiestway.yuv.entities

import com.theeasiestway.yuv.Constants

//
// Created by Loboda Alexey on 29.07.2020.
//

class Rgb565Frame: RgbFrame() {
    override fun getType(): Int = Constants.RGB565
}