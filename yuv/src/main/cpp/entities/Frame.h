//
// Created by Loboda Alexey on 02.08.2020.
//

#ifndef LIBYUVWRAPPER_FRAME_H
#define LIBYUVWRAPPER_FRAME_H

//
// Created by Loboda Alexey on 02.08.2020.
//

class Frame {

protected:
    virtual void release() {
        if (data) free(data);
        data = nullptr;
    }

public:
    int width;
    int height;
    uint8_t* data = nullptr;
    int dataSize;

    long getPointer() {
        return (long) this;
    }

    ~Frame() {
        dataSize = -1;
        width = -1;
        height = -1;
    }
};

#endif //LIBYUVWRAPPER_FRAME_H