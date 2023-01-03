#pragma once

#include <stack>

template<typename T>
class IndexPool {

    std::stack<T> unused {};
    T next = 0;

public:
    IndexPool() {}

    T get_next() {
        if(unused.empty()) return next++;
        T result = unused.top();
        unused.pop();
        return result;
    }

    void release_index(T index) {
        unused.push(index);
    }
};