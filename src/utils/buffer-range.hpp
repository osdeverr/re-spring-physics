#pragma once

#include <climits>

struct BufferRange {
    static BufferRange global;
    static BufferRange empty;

    int m_from, m_to;

    BufferRange(int from, int to): m_from(from), m_to(to) {}
    BufferRange(): m_from(0), m_to(0) {}

    bool is_empty() const { return m_to <= m_from; }

    void extend(int from, int to) {
        if(m_from > from) m_from = from;
        if(m_to < to) m_to = to;
    }

    void trim(int from, int to) {
        if(m_from < from) m_from = from;
        if(m_from > to) m_from = to;
        if(m_to > to) m_to = to;
        if(m_to < from) m_to = from;
    }

    int size() { return m_to - m_from; }
    void clear() { m_from = INT_MAX; m_to = INT_MIN; }
};