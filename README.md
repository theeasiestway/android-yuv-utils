# android-yuv-utils
Compiled Google's [libyuv](https://chromium.googlesource.com/libyuv/libyuv/) library and wrapper for it for easy and fast scale, rotate, mirror and converting frames from android Camera2 or CameraX.

## Supported features:
1. Scale horizontally and/or vertically.
2. Rotating by 90, 180 or 270 degrees.
3. Mirror horizontally or vertically.
4. Convert android YUV_420_8888 frame to ARGB.

## Supported ABIs:
armeabi-v7a, arm64-v8a, x86, x86_64

## How to use

#### Init library:
```kotlin
val yuvUtils = YuvUtils()                                    // getting an instance of the library
```

#### Prepare a frame from Camera2 or CameraX for scaling, rotating etc.:
```kotlin
val image = ...                                              // getting an image from Camera2 or CameraX api
var yuvFrame = yuvUtils.convertToI420(image)                 // this step isn't mandatory but it may help you in case if the colors of the output frame (after scale, rotate etc.) are distorted  
```

#### Scale:
```kotlin
/* the first way */
val image = ...                                             // getting an image from Camera2 or CameraX api
val scaledWidth = image.width * 2                           // getting current width and height
val scaledHeight = image.height * 2                         // of a frame and for example double them
yuvFrame = yuvUtils.scale(image, scaledWidth, scaledHeight, Constants.FILTER_BOX)

/* the second way here yuvFrame was taken from the yuvUtils.convertToI420 method as showed above */ 
val scaledWidth = yuvFrame.width * 2                        // getting current width and height
val scaledHeight = yuvFrame.height * 2                      // of a frame and for example double them
yuvFrame = yuvUtils.scale(yuvFrame, scaledWidth, scaledHeight, Constants.FILTER_BOX)
```

#### Rotate:
```kotlin
val rotate = Constants.ROTATE_270                           // setting the rotation angle
yuvFrame = yuvUtils.rotate(image, rotate)
/* or */
yuvFrame = yuvUtils.rotate(yuvFrame, rotate)
```

#### Mirror:
```kotlin
/* for mirror horizontally */
yuvFrame = yuvUtils.mirrorH(image)
/* or */
yuvFrame =  yuvUtils.mirrorH(yuvFrame)

/* for mirror vertically */
yuvFrame = yuvUtils.mirrorV(image)
/* or */
yuvFrame = yuvUtils.mirrorV(yuvFrame)
```

#### Convert to ARGB:
```kotlin
val argbFrame = yuvUtils.yuv420ToArgb(image)
/* or */
val argbFrame = yuvUtils.yuv420ToArgb(yuvFrame)
```

## Project structure
#### The project consists of two modules:
- **app** - here you can find a sample app that demonsrates scaling, rotating, mirroring and converting procedures by capturing frames from device's camera.
- **yuv** - here you can find a C++ class that interacts with [libyuv 1759](https://chromium.googlesource.com/libyuv/libyuv/+/c5e45dcae58f5cb3eb893f8000c1de88a8fe3c4e) and a JNI wrapper for interacting with it from Java/Kotlin layer.

#### Compiled library:
- **yuv.aar** - it's a compiled library of **yuv** module that mentioned above, it placed in a root directory of the project, you can easily add it to your project using gradle dependencies. First you should place **yuv.aar** in the libs folder of your project and then add to your build.gradle:
````groovy
dependencies {
    api fileTree(dir: 'libs', include: '*.jar')       // this line is necessary in order to gradle took yuv.aar from "libs" dir
    api files('libs/yuv.aar')                         // dependency for yuv.aar library
    ...                                               // other dependencies
}
````
