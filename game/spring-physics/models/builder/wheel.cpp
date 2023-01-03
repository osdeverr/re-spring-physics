
#include "wheel.hpp"

ModelBuilder::Wheel
ModelBuilder::build_wheel(ModelBuilder::Builder &builder, int anchor1, int anchor2, float radius, int steps, float alpha) {
    Wheel wheel{};

    wheel.anchor1 = anchor1;
    wheel.anchor2 = anchor2;

    Vec3f center_position = builder.get_vertex_pos(anchor1) * (1 - alpha) + builder.get_vertex_pos(anchor2) * alpha;

    Vec3f support_direction = builder.get_vertex_pos(anchor2) - center_position;
    support_direction.normalize();
    Vec3f basis1 = get_orthogonal_vector(support_direction);
    Vec3f basis2 = support_direction.cross(basis1);

    for (int i = 0; i < steps; i++) {
        int node = builder.vertex(center_position +
                                  basis1 * (radius * cos(2 * M_PI * i / steps)) +
                                  basis2 * (radius * sin(2 * M_PI * i / steps))
        );
        builder.spring(wheel.anchor1, node);
        builder.spring(wheel.anchor2, node);

        if (i > 0) {
            builder.spring(node, node - 1);
        } else {
            wheel.first_rim = node;
        }
    }

    wheel.last_rim = wheel.first_rim + steps - 1;

    builder.spring(wheel.first_rim, wheel.last_rim);

    return wheel;
}
