#pragma once

#include "spring-object.hpp"

struct Muscle {
    SpringObject* m_spring = nullptr;
    float m_min_length = 0.8;
    float m_max_length = 1.2;

    Muscle() {}

    void update_input(float input) const;
};