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
    std::vector<uint8_t> data;

    long getPointer(Frame &frame) {
        return (long) &frame;
    }

    virtual std::pair<uint8_t*, int> getBytes() {
        return std::make_pair(data.data(), data.size());
    }

    ~Frame() {
        std::vector<uint8_t>().swap(data);
    }
};


#endif //LIBYUVWRAPPER_FRAME_H
