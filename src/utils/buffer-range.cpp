//
// Created by Артем on 03.02.2022.
//

#include "buffer-range.hpp"

BufferRange BufferRange::global { INT_MIN, INT_MAX };
BufferRange BufferRange::empty { INT_MAX, INT_MIN };

void BufferRange::extend(int from, int to) {
    if(m_from > from) m_from = from;
    if(m_to < to) m_to = to;
}

void BufferRange::trim(int from, int to) {
    if(m_from < from) m_from = from;
    if(m_from > to) m_from = to;
    if(m_to > to) m_to = to;
    if(m_to < from) m_to = from;
}
