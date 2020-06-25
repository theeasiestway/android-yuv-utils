package com.theeasiestway.yuv

//
// Created by Loboda Alexey on 22.06.2020.
//

object Constants {

    //
    // Scale
    //

    /** If filtering is FILTER_NONE, a simple nearest-neighbor algorithm is
     * used. This produces basic (blocky) quality at the fastest speed.
     * If filtering is FILTER_BILINEAR, interpolation is used to produce a better
     * quality image, at the expense of speed.
     * If filtering is FILTER_BOX, averaging is used to produce ever better
     * quality image, at further expense of speed. */

    const val FILTER_NONE = 0       // Point sample; Fastest.
    const val FILTER_LINEAR = 1     // Filter horizontally only.
    const val FILTER_BILINEAR = 2   // Faster than box, but lower quality scaling down.
    const val FILTER_BOX = 3        // Highest quality.

    //
    // Rotate
    //

    const val ROTATE_0 = 0          // No rotation.
    const val ROTATE_90 = 90        // Rotate 90 degrees clockwise.
    const val ROTATE_180 = 180      // Rotate 180 degrees.
    const val ROTATE_270 = 270      // Rotate 270 degrees clockwise.

}