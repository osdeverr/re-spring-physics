#pragma once

#include "model-builder.hpp"
#include "../../../utils/get-orthogonal.hpp"

namespace ModelBuilder {

    struct Wheel {
        int anchor1;
        int anchor2;
        int first_rim;
        int last_rim;
    };

    Wheel build_wheel(Builder &builder, int center, int support, float radius, int steps, float alpha = 0.0f);

}