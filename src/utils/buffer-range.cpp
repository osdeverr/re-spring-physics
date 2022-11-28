//
// Created by Артем on 03.02.2022.
//

#include "buffer-range.hpp"

BufferRange BufferRange::global { INT_MIN, INT_MAX };
BufferRange BufferRange::empty { INT_MAX, INT_MIN };