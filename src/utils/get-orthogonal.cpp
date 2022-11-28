
#include "get-orthogonal.hpp"

Vec3f get_orthogonal_vector(Vec3f vector) {
    Vec3f up_a = {vector.y, -vector.x, 0};
    Vec3f up_b = {0, -vector.z, vector.x};

    float projection_a = 1 - abs(up_a.dot(vector));
    float projection_b = 1 - abs(up_b.dot(vector));

    Vec3f different_vector = (up_a *= projection_a) += (up_b *= projection_b);
    Vec3f cross = vector.cross(different_vector);
    cross.normalize();
    return cross;
}
