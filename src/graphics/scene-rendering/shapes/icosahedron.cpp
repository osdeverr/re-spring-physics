//
// Created by Артем on 07.02.2022.
//

#include "icosahedron.hpp"

const float IcosahedronShape::golden_ratio = (1.0f + (float)sqrt(5)) / 2;
const float IcosahedronShape::icosahedron_radius = (float) sqrt(1 + golden_ratio * golden_ratio);

const std::vector<Vec3f> IcosahedronShape::vertices {
        {-1, 0, golden_ratio}, {1, 0, golden_ratio}, {-1, 0, -golden_ratio}, {1, 0, -golden_ratio},
        {0, golden_ratio, 1}, {0, golden_ratio, -1}, {0, -golden_ratio, 1}, {0, -golden_ratio, -1},
        {golden_ratio, 1, 0}, {-golden_ratio, 1, 0}, {golden_ratio, -1, 0}, {-golden_ratio, -1, 0}
};

const std::vector<Vec3i> IcosahedronShape::faces = {
        {0, 4, 1},  {0, 9, 4},  {9, 5, 4},  {4, 5, 8},  {4, 8, 1},
        {8, 10, 1}, {8, 3, 10}, {5, 3, 8},  {5, 2, 3},  {2, 7, 3},
        {7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1, 6},
        {6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5},  {7, 2, 11}
};