//
// Created by Артем on 05.02.2022.
//

#include "physics-thread.hpp"

PhysicsThread::PhysicsThread(ConcurrentPhysicsEngine* engine): AsyncTask(), m_engine(engine) {}

void PhysicsThread::save_forces() {
    for(auto& creature : m_creatures) {
        for(auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_step_force = vertex->get_physics_vertex()->m_force;
        }
    }
}

void PhysicsThread::restore_forces() {
    for(auto& creature : m_creatures) {
        for(auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_force = vertex->get_physics_vertex()->m_step_force;
        }
    }
}

void PhysicsThread::clear_forces() {
    for(auto& creature : m_creatures) {
        for(auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_force = Vec3f(0, 0, 0);
        }
    }
}

void PhysicsThread::apply_forces(float dt) {
    for(auto& creature : m_creatures) {
        for(auto &vertex: creature->get_vertices()) {
            process_vertex_forces(dt, vertex->get_physics_vertex());
        }
    }
}

void PhysicsThread::apply_velocities(float dt) {
    for(auto& creature : m_creatures) {
        for(auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_position += vertex->get_physics_vertex()->m_velocity * dt;
        }
    }
}

void PhysicsThread::substep(float dt) {
    Vec3f gravity = this->m_engine->get_gravity();

    save_forces();

    for(auto& creature : m_creatures) {
        creature->physics_tick();

        for(auto &spring: creature->get_springs()) {
            spring->get_physics_spring()->force_tick();
        }

        for(auto &surface: creature->get_surfaces()) {
            surface->get_physics_surface()->force_tick();
        }

        for(auto &jet: creature->get_jet_objects()) {
            jet->get_physics_jet()->force_tick(dt);
        }

        for(auto &vertex: creature->get_vertices()) {
            vertex->get_physics_vertex()->m_force += gravity * vertex->get_physics_vertex()->m_mass;
        }
    }

    apply_forces(dt);
    apply_velocities(dt);

    if(m_engine->threads_should_clear_forces()) {
        clear_forces();
    } else {
        restore_forces();
    }

    m_phase = 1 - m_phase;
}

void PhysicsThread::process_vertex_forces(float dt, PhysicsVertex* vertex) {
    float floor_level = m_engine->get_floor_level();

    if(vertex->m_position.y < floor_level) {
        vertex->m_position.y = floor_level;

        if(vertex->m_velocity.y < 0) vertex->m_velocity.y = 0;

        if(vertex->m_force.y < 0) {
            float friction = -vertex->m_force.y * vertex->m_floor_friction;
            float epsilon = friction * dt / vertex->m_mass;

            Vec2f horizontal_velocity = { vertex->m_velocity.x, vertex->m_velocity.z };
            Vec2f horizontal_force = { vertex->m_force.x, vertex->m_force.z };
            float velocity_modulo = (horizontal_force * dt += horizontal_velocity).len();

            if(velocity_modulo < epsilon) {
                horizontal_velocity = {0, 0};
                horizontal_force = {0, 0};
            }
            else horizontal_force -= (horizontal_velocity / velocity_modulo) * friction;

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
