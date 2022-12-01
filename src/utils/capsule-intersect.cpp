
#include "capsule-intersect.hpp"

float dist_point_segment(const Vec3f& p, const Vec3f& a, const Vec3f& b) {
    Vec3f pa = p - a;
    Vec3f ba = b - a;
    float h = std::clamp(pa.dot(ba) / ba.dot(ba), 0.0f, 1.0f);
    return (a + ba * h - p).len();
}

Vec3f capsule_project(Vec3f point, Vec3f cap_a, Vec3f cap_b, float cap_radius) {
    Vec3f ba = cap_b - cap_a;
    Vec3f pa = point - cap_a;
    float h = pa.dot(ba) / ba.dot(ba);

    if(h < 0) {
        return cap_a + (point - cap_a).normalize() * cap_radius;
    }
    if(h > 1) {
        return cap_b + (point - cap_b).normalize() * cap_radius;
    }

    Vec3f segment_point = cap_a + ba * h;
    return segment_point + (point - segment_point).normalize() * cap_radius;
}

// Taken from https://iquilezles.org/articles/intersectors/

float capsule_first_intersect(Vec3f ray_origin, Vec3f ray_direction, Vec3f cap_a, Vec3f cap_b, float cap_radius) {
    Vec3f ba = cap_b - cap_a;
    Vec3f oa = ray_origin - cap_a;

    float baba = ba.dot(ba);
    float bard = ba.dot(ray_direction);
    float baoa = ba.dot(oa);
    float rdoa = ray_direction.dot(oa);
    float oaoa = oa.dot(oa);

    float a = baba - bard * bard;
    float b = baba * rdoa - baoa * bard;
    float c = baba * oaoa - baoa * baoa - cap_radius * cap_radius * baba;
    float h = b * b - a * c;

    if (h >= 0.0) {
        float t = (-b - sqrt(h)) / a;
        float y = baoa + t * bard;

        // Body collision
        if (y > 0.0 && y < baba) {
            return t;
        }

        // Cap collision
        Vec3f oc = (y <= 0.0) ? oa : ray_origin - cap_b;
        b = ray_direction.dot(oc);
        c = oc.dot(oc) - cap_radius * cap_radius;
        h = b * b - c;
        if (h > 0.0) {
            return -b - sqrt(h);
        }
    }
    return std::numeric_limits<float>::infinity();
}

Vec3f capsule_normal(Vec3f position, Vec3f cap_a, Vec3f cap_b) {
    Vec3f ba = cap_b - cap_a;
    Vec3f pa = position - cap_a;
    float h = std::clamp(pa.dot(ba) / ba.dot(ba), 0.0f, 1.0f);
    return (position - cap_a - ba * h).normalize();
}
