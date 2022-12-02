#pragma once

#include "../../creatures/creature-config.hpp"
#include <span>

namespace ModelBuilder {

    struct BuilderState {
        float m_frame_weight = 0.0000066f; // Per one strength unit per one meter
        float m_spring_strength = 1000.0f;
        float m_spring_damping = 3.0f;
        float m_vertex_weight = 0.005f;
        float m_vertex_floor_friction = 0.5f;

        void set_matrix(const Matrix4f &matrix) {
            m_transform = matrix;
            m_inverse_transform = matrix.inverse();
        }

        const Matrix4f &get_matrix() const {
            return m_transform;
        }

        const Matrix4f &get_inverse_matrix() const {
            return m_inverse_transform;
        }

    private:
        Matrix4f m_transform{};
        Matrix4f m_inverse_transform{};
    };

    class Builder {
        CreatureConfig m_config;
        BuilderState m_state{};
        std::vector<BuilderState> m_state_stack{};
    public:
        Builder() = default;

        ~Builder() = default;

        BuilderState &get_state() {
            return m_state;
        }

        void push_state() {
            m_state_stack.push_back(m_state);
        }

        void pop_state() {
            m_state = m_state_stack.back();
            m_state_stack.pop_back();
        }

        int vertex(Vec3f point) {
            point *= m_state.get_matrix();
            m_config.m_vertices.push_back(
                    VertexConfig{m_state.m_vertex_weight, m_state.m_vertex_floor_friction, point});
            return m_config.m_vertices.size() - 1;
        }

        int vertex(float x, float y, float z) {
            return vertex(Vec3f{x, y, z});
        }

        int spring(int a, int b) {
            assert((get_vertex_pos(a) - get_vertex_pos(b)).len_squared() > 0.05f * 0.05f);
            m_config.m_springs.push_back(SpringConfig{m_state.m_spring_strength, m_state.m_spring_damping, a, b});
            return m_config.m_springs.size() - 1;
        }

        float get_spring_length(int spring_index) {
            auto &spring = m_config.m_springs[spring_index];
            return (m_config.m_vertices[spring.m_vertex_a_index].m_position -
                    m_config.m_vertices[spring.m_vertex_b_index].m_position).len();
        }

        int surface(int a, int b, int c) {
            m_config.m_surfaces.push_back(SurfaceConfig{a, b, c});
            return m_config.m_surfaces.size() - 1;
        }

        int jet(int a, int b, float force) {
            m_config.m_jets.push_back(JetConfig{a, b, force});
            return m_config.m_jets.size() - 1;
        }

        void translate(Vec3f offset) {
            for (auto &vertex: m_config.m_vertices) {
                vertex.m_position += offset;
            }
        }

        void scale(Vec3f scale) {
            for (auto &vertex: m_config.m_vertices) {
                vertex.m_position *= scale;
            }
        }

        Vec3f get_vertex_pos(int index) {
            Vec3f global_position = m_config.m_vertices[index].m_position;
            return global_position *= m_state.get_inverse_matrix();
        }

        void springs_between_range(int begin, int end) {
            for (int i = begin; i < end; i++) {
                for (int j = i + 1; j < end; j++) {
                    spring(i, j);
                }
            }
        }

        template<typename... Args>
        void springs_between(Args... args) {
            int vertices[] = {args...};
            for (int i = 0; i < sizeof...(args); i++) {
                for (int j = i + 1; j < sizeof...(args); j++) {
                    spring(vertices[i], vertices[j]);
                }
            }
        }

        void springs_between(std::span<int> vertices) {
            for (int i = 0; i < vertices.size(); i++) {
                for (int j = i + 1; j < vertices.size(); j++) {
                    spring(vertices[i], vertices[j]);
                }
            }
        }

        CreatureConfig &get_config() {
            return m_config;
        }

        void calculate_mass();

        int current_vertex();
    };

}