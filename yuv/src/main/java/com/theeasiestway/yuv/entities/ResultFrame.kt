package com.theeasiestway.yuv.entities

class ResultFrame {

    lateinit var data: ByteArray; private set
    var size = 0; private set
    var width = 0; private set
    var height = 0; private set

    fun fill(data: ByteArray, width: Int, height: Int) {
        this.data = data
        this.size = data.size
        this.width = width
        this.height = height
    }

    fun free() {
        data = ByteArray(1)
        size = 0
        width = 0
        height = 0
    }

    override fun equals(other: Any?): Boolean {
        if (other == null || other !is ResultFrame) return false
        return (data.contentEquals(other.data) &&
                size == other.size &&
                width == other.width &&
                height == other.height)
    }
}