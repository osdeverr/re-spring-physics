//
// Created by Артем on 05.02.2022.
//

// #include <MacTypes.h>
#include "physics-thread.hpp"

#include <cassert>

PhysicsThread::PhysicsThread(ConcurrentPhysicsEngine *engine) : AsyncTask(), m_engine(engine) {}

void PhysicsThread::save_forces() {
    for (auto &creature: m_creatures) {
        for (auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_step_force = vertex->get_physics_vertex()->m_force;
        }
    }
}

void PhysicsThread::restore_forces() {
    for (auto &creature: m_creatures) {
        for (auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_force = vertex->get_physics_vertex()->m_step_force;
        }
    }
}

void PhysicsThread::clear_forces() {
    for (auto &creature: m_creatures) {
        for (auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_force = Vec3f(0, 0, 0);
        }
    }
}

void PhysicsThread::apply_forces(float dt) {
    for (auto &creature: m_creatures) {
        for (auto &vertex: creature->get_vertices()) {
            process_vertex_forces(dt, vertex->get_physics_vertex());
        }
    }
}

void PhysicsThread::apply_velocities(float dt) {
    for (auto &creature: m_creatures) {
        for (auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_position += vertex->get_physics_vertex()->m_velocity * dt;
        }
    }
}

void PhysicsThread::substep(float dt) {
    Vec3f gravity = this->m_engine->get_gravity();

    save_forces();

    for (auto &creature: m_creatures) {
        creature->physics_tick();

        for (auto &spring: creature->get_springs()) {
            spring->get_physics_spring()->force_tick();
        }

        for (auto &surface: creature->get_surfaces()) {
            surface->get_physics_surface()->force_tick();
        }

        for (auto &jet: creature->get_jet_objects()) {
            jet->get_physics_jet()->force_tick(dt);
        }

        for (auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_force += gravity * vertex->get_physics_vertex()->m_mass;
        }
    }

    handle_terrain_collision(dt);

    apply_forces(dt);
    apply_velocities(dt);

    if (m_engine->threads_should_clear_forces()) {
        clear_forces();
    } else {
        restore_forces();
    }

    m_phase = 1 - m_phase;
}

void PhysicsThread::handle_terrain_collision(float dt) {
    for (auto creature: get_creatures()) {
        for (auto vertex: creature->get_vertices()) {
            auto* physics_vertex = vertex->get_physics_vertex();
            auto from = physics_vertex->m_position;
            auto to = from + physics_vertex->m_velocity * dt;

            handle_vertex_terrain_collisions(dt, physics_vertex, from, to);

//            for(int i = 0;; i++) {
//                if(!handle_vertex_terrain_collisions(dt, physics_vertex, from, to)) {
//                    break;
//                }
//                to = physics_vertex->m_position;
//                if(i >= m_max_vertex_collisions) {
//                    physics_vertex->m_position = from;
//                    physics_vertex->m_velocity = Vec3f(0, 0, 0);
//                    break;
//                }
//            }
        }
    }
}

bool PhysicsThread::handle_vertex_terrain_collisions(float dt, PhysicsVertex *vertex, Vec3f& from, Vec3f& to) {
    float min_collision_distance = std::numeric_limits<float>::infinity();
    Vec3f min_collision_normal {};

    Vec3f movement = to - from;
    float movement_len = movement.len();
    movement /= movement_len;

#ifdef NO_BVH
    for(auto& triangle : m_engine->get_terrain()->get_surface_mesh()) {
#else
    m_triangle_buffer.clear();
    m_engine->get_terrain()->query(from, to, m_triangle_buffer);
    for (auto &triangle: m_triangle_buffer) {
#endif
        float collision_distance = 0.0f;
        Vec3f collision_normal {};
        if (triangle->collides(from, movement, movement_len, collision_distance, collision_normal)) {
            if(collision_distance < min_collision_distance) {
                min_collision_distance = collision_distance;
                min_collision_normal = collision_normal;
            }
        }
    }

    if(min_collision_distance < std::numeric_limits<float>::infinity()) {
        Vec3f velocity = vertex->m_velocity;
        Vec3f force = vertex->m_force;
        float floor_friction = vertex->m_floor_friction;
        float mass = vertex->m_mass;

        // Calculate the floor friction force

        float floor_force = force.dot(min_collision_normal);
        Vec3f projected_force = force - min_collision_normal * floor_force;
        Vec3f projected_velocity = velocity - min_collision_normal * velocity.dot(min_collision_normal);

        float friction = abs(floor_force) * floor_friction;
        float projected_velocity_len = projected_velocity.len();
        float single_tick_velocity = (force * (dt / mass) + velocity).len();
        float single_tick_friction = friction * (dt / mass);

        if (single_tick_velocity <= single_tick_friction || projected_velocity_len < 0.0001) {
            projected_velocity = {};
            projected_force = {};
        } else {
            projected_force -= projected_velocity / projected_velocity_len * friction;
        }

        Vec3f new_force = projected_force + min_collision_normal * floor_force;

        // project the `to` point onto the collision plane
        Vec3f collision_point = from + movement * (min_collision_distance - 0.001f);

        Vec3f to_projected = to - min_collision_normal * min_collision_normal.dot(to - collision_point);

        Vec3f new_position = to_projected;

        vertex->m_force = new_force;
        vertex->m_velocity = projected_velocity;
        vertex->m_position = new_position;

        return true;
    }

    return false;
}

void PhysicsThread::process_vertex_forces(float dt, PhysicsVertex *vertex) {
    float floor_level = m_engine->get_floor_level();

    if (vertex->m_position.y < floor_level) {
        vertex->m_position.y = floor_level;

        if (vertex->m_velocity.y < 0) vertex->m_velocity.y = 0;

        if (vertex->m_force.y < 0) {
            float friction = -vertex->m_force.y * vertex->m_floor_friction;

            Vec2f horizontal_velocity = {vertex->m_velocity.x, vertex->m_velocity.z};
            Vec2f horizontal_force = {vertex->m_force.x, vertex->m_force.z};

            float horizontal_velocity_len = horizontal_velocity.len();
            float single_tick_velocity = (horizontal_velocity + horizontal_force * (dt / vertex->m_mass)).len();
            float single_tick_friction = friction * dt / vertex->m_mass;

            if (single_tick_velocity <= single_tick_friction || horizontal_velocity_len < 0.0001) {
                horizontal_velocity = {0, 0};
                horizontal_force = {0, 0};
            } else {
                horizontal_force -= horizontal_velocity * (friction / horizontal_velocity_len);
            }

            if(isnan(horizontal_force.x) || isnan(horizontal_force.y) || isnan(horizontal_velocity.x) || isnan(horizontal_velocity.y)) {
                std::cout << "nan" << std::endl;
                std::cout << "horizontal_velocity: " << horizontal_velocity.x << ", " << horizontal_velocity.y << std::endl;
                std::cout << "horizontal_force: " << horizontal_force.x << ", " << horizontal_force.y << std::endl;
                std::cout << "friction: " << friction << std::endl;
                std::cout << "single_tick_velocity: " << single_tick_velocity << std::endl;
                std::cout << "single_tick_friction: " << single_tick_friction << std::endl;
                std::cout << "horizontal_velocity_len: " << horizontal_velocity_len << std::endl;
                std::cout << "vertex->m_mass: " << vertex->m_mass << std::endl;
                std::cout << "dt: " << dt << std::endl;
                std::cout << "vertex->m_position: " << vertex->m_position.x << ", " << vertex->m_position.y << ", " << vertex->m_position.z << std::endl;
                std::cout << "vertex->m_velocity: " << vertex->m_velocity.x << ", " << vertex->m_velocity.y << ", " << vertex->m_velocity.z << std::endl;
                std::cout << "vertex->m_force: " << vertex->m_force.x << ", " << vertex->m_force.y << ", " << vertex->m_force.z << std::endl;
                std::cout << "vertex->m_floor_friction: " << vertex->m_floor_friction << std::endl;
                assert(false);
            }

            vertex->m_velocity.x = horizontal_velocity.x;
            vertex->m_velocity.z = horizontal_velocity.y;

            vertex->m_force.x = horizontal_force.x;
            vertex->m_force.z = horizontal_force.y;
        }
    }

    vertex->m_velocity += vertex->m_force * dt / vertex->m_mass;
}

void PhysicsThread::async_task() {
    AsyncTask::async_task();

    int substeps = m_engine->get_substeps();
    float dt = m_engine->get_dt();

    substep(dt / (float) substeps);
}
