#pragma once

#include "../builder/model-builder.hpp"
#include "../../../utils/get-orthogonal.hpp"
#include "../builder/wheel.hpp"

class CarFactory {

    struct Suspension {
        int wheel_anchor1;
        int wheel_anchor2;
        union {
            int body_connection_vertices[4];
            struct {
                int bottom_front;
                int up_front;
                int up_back;
                int bottom_back;
            } body_connections;
        };
        int steering_node;
    };

public:
    CarFactory(ModelBuilder::Builder &builder) : m_builder(builder) {};

    ~CarFactory() = default;

    Suspension wheel_suspension(Vec3f position, Vec3f direction, float height, float length) {

        float hub_width = 2;
        float hub_height = 1;
        float wheel_width = 0.5;

        Suspension suspension{};

        Vec3f up = Vec3f(0, 1, 0);
        Vec3f back = Vec3f(1, 0, 0);

        Vec3f wheel_center = position + direction * length - up * height + back * hub_width / 2;

        int wheel_vertex = m_builder.vertex(wheel_center);
        int wheel_vertex_up = m_builder.vertex(wheel_center + up * hub_height);
        int wheel_vertex_in = m_builder.vertex(wheel_center + direction * wheel_width);
        int wheel_vertex_steer = m_builder.vertex(wheel_center + back * (hub_width / 2) + up * (hub_height / 2));

        m_builder.springs_between(wheel_vertex, wheel_vertex_up, wheel_vertex_in, wheel_vertex_steer);

        int body_point = m_builder.vertex(position);
        int body_point_up = m_builder.vertex(position + up * hub_height);
        int body_point_back = m_builder.vertex(position + back * hub_width);
        int body_point_up_back = m_builder.vertex(position + up * hub_height + back * hub_width);

        m_builder.springs_between(body_point, body_point_up, body_point_up_back, body_point_back);

        // Build a steering node

        int body_vertex_steer = m_builder.vertex(position + back * (hub_width / 2) + up * (hub_height / 2));

        m_builder.springs_between(body_vertex_steer, body_point, body_point_up);
        m_builder.spring(body_vertex_steer, wheel_vertex_steer);

        // Bottom arm
        m_builder.springs_between(body_point, wheel_vertex, body_point_back);

        // Top arm
        m_builder.springs_between(body_point_up, wheel_vertex_up, body_point_up_back);

        float strength = m_builder.get_state().m_spring_strength;
        m_builder.get_state().m_spring_strength = 300;

        m_builder.spring(body_point_up, wheel_vertex);
        m_builder.spring(body_point_up_back, wheel_vertex);

        m_builder.get_state().m_spring_strength = strength;

        suspension.wheel_anchor1 = wheel_vertex;
        suspension.wheel_anchor2 = wheel_vertex_in;
        suspension.body_connection_vertices[0] = body_point;
        suspension.body_connection_vertices[1] = body_point_up;
        suspension.body_connection_vertices[2] = body_point_up_back;
        suspension.body_connection_vertices[3] = body_point_back;
        suspension.steering_node = body_vertex_steer;

        return suspension;
    }

    void build() {

//        m_builder.m_vertex_weight = 1.0f;
        // The car is headed in the negative x direction

        m_builder.get_state().m_spring_strength = 5000;

        auto caster_matrix = Matrix4f::rotation_z_matrix(-0.1f);

        m_builder.get_state().set_matrix(caster_matrix * Matrix4f::translation_matrix(-2, 3, -0.7));
        Suspension front_left_suspension = wheel_suspension({}, Vec3f(0, 0, -1), 0.0f, 1.5);

        m_builder.get_state().set_matrix(caster_matrix * Matrix4f::translation_matrix(2, 3, -0.7));
        Suspension back_left_suspension = wheel_suspension({}, Vec3f(0, 0, -1), 0.0f, 1.5);

        m_builder.get_state().set_matrix(caster_matrix * Matrix4f::translation_matrix(-2, 3, 0.7));
        Suspension front_right_suspension = wheel_suspension({}, Vec3f(0, 0, 1), 0.0f, 1.5);

        m_builder.get_state().set_matrix(caster_matrix * Matrix4f::translation_matrix(2, 3, 0.7));
        Suspension back_right_suspension = wheel_suspension({}, Vec3f(0, 0, 1), 0.0f, 1.5);

        m_builder.get_state().set_matrix({});

        // Add four wheels

        m_builder.get_state().m_spring_strength = 300;
        m_builder.get_state().m_vertex_floor_friction = 2.0f;

        ModelBuilder::build_wheel(m_builder, front_left_suspension.wheel_anchor1, front_left_suspension.wheel_anchor2,
                                  0.7f, 16, 0.5);
        ModelBuilder::build_wheel(m_builder, back_left_suspension.wheel_anchor1, back_left_suspension.wheel_anchor2,
                                  0.7f, 16, 0.5);
        ModelBuilder::build_wheel(m_builder, front_right_suspension.wheel_anchor1, front_right_suspension.wheel_anchor2,
                                  0.7f, 16, 0.5);
        ModelBuilder::build_wheel(m_builder, back_right_suspension.wheel_anchor1, back_right_suspension.wheel_anchor2,
                                  0.7f, 16, 0.5);

        m_builder.get_state().m_vertex_floor_friction = 0.5f;
        m_builder.get_state().m_spring_strength = 1000;

        // Connect suspension steer nodes

        m_builder.spring(front_left_suspension.steering_node, front_right_suspension.steering_node);
        m_builder.spring(back_left_suspension.steering_node, back_right_suspension.steering_node);

        // Connect suspensions together

        m_builder.springs_between(front_left_suspension.body_connections.bottom_front,
                                  front_right_suspension.body_connections.bottom_front,
                                  front_left_suspension.body_connections.up_front,
                                  front_right_suspension.body_connections.up_front,
                                  front_left_suspension.body_connections.bottom_back,
                                  front_right_suspension.body_connections.bottom_back,
                                  front_left_suspension.body_connections.up_back,
                                  front_right_suspension.body_connections.up_back);

        m_builder.springs_between(back_left_suspension.body_connections.bottom_front,
                                  back_right_suspension.body_connections.bottom_front,
                                  back_left_suspension.body_connections.up_front,
                                  back_right_suspension.body_connections.up_front,
                                  back_left_suspension.body_connections.bottom_back,
                                  back_right_suspension.body_connections.bottom_back,
                                  back_left_suspension.body_connections.up_back,
                                  back_right_suspension.body_connections.up_back);

        m_builder.springs_between(
                front_left_suspension.body_connections.up_back,
                front_left_suspension.body_connections.bottom_back,
                back_left_suspension.body_connections.up_front,
                back_left_suspension.body_connections.bottom_front);

        m_builder.springs_between(
                front_right_suspension.body_connections.up_back,
                front_right_suspension.body_connections.bottom_back,
                back_right_suspension.body_connections.up_front,
                back_right_suspension.body_connections.bottom_front);

        m_builder.springs_between(
                front_left_suspension.body_connections.up_back,
                front_left_suspension.body_connections.bottom_back,
                back_right_suspension.body_connections.up_front,
                back_right_suspension.body_connections.bottom_front);

        m_builder.springs_between(
                front_right_suspension.body_connections.up_back,
                front_right_suspension.body_connections.bottom_back,
                back_left_suspension.body_connections.up_front,
                back_left_suspension.body_connections.bottom_front);

        m_front_steering_spring = m_builder.spring(front_left_suspension.body_connections.bottom_front, front_right_suspension.steering_node);
        m_back_steering_spring = m_builder.spring(back_left_suspension.body_connections.bottom_front, back_right_suspension.steering_node);

        m_builder.jet(front_left_suspension.body_connections.up_front, front_left_suspension.body_connections.up_back, 40.0f);
        m_builder.jet(front_right_suspension.body_connections.up_front, front_right_suspension.body_connections.up_back, 40.0f);

        m_builder.calculate_mass();
    }

    int m_front_steering_spring = -1;
    int m_back_steering_spring = -1;

    ModelBuilder::Builder &m_builder;
};