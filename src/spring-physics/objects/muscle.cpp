//
// Created by Артем on 04.02.2022.
//

#include "muscle.hpp"

void Muscle::update_input(float input) const {
    if(input > 1) input = 1;
    if(input < 0) input = 0;

    float length = m_min_length * (1 - input) + m_max_length * input;
    m_spring->get_physics_spring()->m_target_length = length;
}
