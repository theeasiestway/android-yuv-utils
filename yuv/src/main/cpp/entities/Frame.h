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

    template <class T>
    long getPointer(T t) { return 0; }

    virtual std::vector<uint8_t> getBytes() = 0;
};


#endif //LIBYUVWRAPPER_FRAME_H
