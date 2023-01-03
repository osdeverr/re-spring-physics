
#include "full-cube.hpp"

ModelBuilder::FullCube ModelBuilder::build_cube(ModelBuilder::Builder &builder, Vec3f center, Vec3f size) {
    FullCube cube{};

    for(int i = 0; i < 8; i++) {
        cube.vertices[i] = builder.vertex(center + Vec3f(
                (i & 4) ? -size.x : size.x,
                (i & 2) ? -size.y : size.y,
                (i & 1) ? -size.z : size.z
        ));
    }

    builder.springs_between_range(cube.vertices[0], cube.vertices[0] + 8);

    return cube;
}
