
#include "terrain-polygon.hpp"
#include "physics-thread.hpp"
#include "../../utils/capsule-intersect.hpp"

bool TerrainPolygon::check_hit(const Vec3f &point, const Vec3f &direction, float* distance) const {
    // Calculate the distance from the ray origin to the plane
    float t = (m_points[0] - point).dot(m_normal) / direction.dot(m_normal);

    Vec3f intersection = point + direction * t;

    Vec3f v0 = m_points[2] - m_points[0];
    Vec3f v1 = m_points[1] - m_points[0];
    Vec3f v2 = intersection - m_points[0];

    float dot00 = v0.dot(v0);
    float dot01 = v0.dot(v1);
    float dot02 = v0.dot(v2);
    float dot11 = v1.dot(v1);
    float dot12 = v1.dot(v2);

    float inv_denom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
    float v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

    if(distance != nullptr) {
        *distance = t;
    }

    return (u >= 0) && (v >= 0) && (u + v < 1);
}

bool
TerrainPolygon::collides(Vec3f ray_origin, Vec3f ray_direction, float distance, float &collision_distance, Vec3f& normal) const {

    Vec3f ray_end = ray_origin + ray_direction * distance;

    // Find out distances to the triangle edges and check if the ray nears any of them.

    for(int i = 0; i < 3; i++) {
        Vec3f edge_start = m_points[i];
        Vec3f edge_end = m_points[(i + 1) % 3];

        float capsule_intersect_distance = capsule_first_intersect(ray_origin, ray_direction, edge_start, edge_end, m_thickness);
        float capsule_distance = dist_point_segment(ray_origin, edge_start, edge_end);

        if(capsule_distance < m_thickness || (capsule_intersect_distance >= 0 && capsule_intersect_distance < distance)) {
            if(capsule_intersect_distance < -0.5f) {
                std::cout << "what?\n";
            }
            collision_distance = capsule_intersect_distance;
            Vec3f collision = ray_origin + ray_direction * collision_distance;
            // Find out if collision does not have a projection on the triangle.
            // Otherwise, the collision is not valid (it is on the triangle itself)
            if(!check_hit(collision, m_normal)) {
                normal = capsule_normal(collision, edge_start, edge_end);
                return true;
            }
        }
    }

    float ray_end_direction = (ray_end - m_points[0]).dot(m_normal);
    bool should_project = (abs(ray_end_direction) < m_thickness);

    if (should_project) {

        float ray_origin_distance = (ray_origin - m_points[0]).dot(m_normal);
        float ray_origin_distance_sign = ray_origin_distance > 0 ? 1 : -1;

        bool origin_projects_on_triangle = check_hit(ray_origin, m_normal);

        if(!origin_projects_on_triangle && ray_origin_distance < m_thickness) {
            // The ray starts from the side of the triangle, so it cannot collide with
            // the triangle itself.
            return false;
        }

        Vec3f thickness_offset = m_normal * m_thickness * ray_origin_distance_sign;

        float hit_distance = 0.0f;

        // Check if ray hits the triangle safe zone boundary
        if(check_hit(ray_origin - thickness_offset, ray_direction, &hit_distance)) {
            if(hit_distance < -0.5f) {
                std::cout << "what?\n";
            }
            collision_distance = hit_distance;
            normal = m_normal;
            return true;
        }
    }

    return false;
}

TerrainPolygon::TerrainPolygon(Vec3f point_a, Vec3f point_b, Vec3f point_c) {
    set_points(point_a, point_b, point_c);
}

void TerrainPolygon::set_points(Vec3f point_a, Vec3f point_b, Vec3f point_c) {
    m_points[0] = point_a;
    m_points[1] = point_b;
    m_points[2] = point_c;

    m_normal = (point_b - point_a).cross(point_c - point_a).normalize();
}
