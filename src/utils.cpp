//
// Created by Артем on 27.01.2022.
//

#include "utils.hpp"

unsigned char to_brightness(double value) {
    if(value < 0) return 0;
    if(value > 1) return 255;
    return (unsigned char)((double)255 * value);
}

double from_brightness(unsigned char color) {
    return (double)color / 255;
}