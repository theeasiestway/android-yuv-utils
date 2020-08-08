//
// Created by Loboda Alexey on 02.08.2020.
//

#ifndef LIBYUVWRAPPER_FRAME_H
#define LIBYUVWRAPPER_FRAME_H

//
// Created by Loboda Alexey on 02.08.2020.
//

class Frame {

public:
    int width;
    int height;
    uint8_t* data = nullptr;
    int dataSize;

    long getPointer(Frame &frame) {
        return (long) &frame;
    }

    ~Frame() {
        if (data) free(data);
        data = nullptr;
    }
};


#endif //LIBYUVWRAPPER_FRAME_H
