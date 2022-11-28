#pragma once

template<typename T>
class DoubleEndedLinkedList {

    T* m_start = nullptr;
    T* m_end = nullptr;
    int m_size = 0;

public:
    DoubleEndedLinkedList() {}

    void push_back(T* element) {
        if(m_start == nullptr) {
            m_start = element;
            m_end = element;
            element->m_prev = nullptr;
            element->m_next = nullptr;
        } else {
            m_end->m_next = element;
            element->m_prev = m_end;
            m_end = element;
        }

        m_size++;
    }

    void push_front(T* element) {
        if(m_start == nullptr) {
            m_start = element;
            m_end = element;
            element->m_prev = nullptr;
            element->m_next = nullptr;
        } else {
            m_start->m_prev = element;
            element->m_next = m_start;
            m_start = element;
        }

        m_size++;
    }

    void erase(T* element) {
        m_size--;

        if(element->m_next) element->m_next->m_prev = element->m_prev;
        if(element->m_prev) element->m_prev->m_next = element->m_next;

        if(element == m_start) m_start = element->m_next;
        if(element == m_end) m_end = element->m_prev;

        element->m_prev = nullptr;
        element->m_next = nullptr;
    }

    int size() { return m_size; }

    bool empty() { return m_start == nullptr; }

    T* begin() { return m_start; }
    T* end() { return m_end; }
};