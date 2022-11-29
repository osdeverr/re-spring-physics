
#include "model-builder.hpp"

namespace ModelBuilder {

    void Builder::calculate_mass() {
        for (int i = 0; i < m_config.m_springs.size(); i++) {
            auto &spring = m_config.m_springs[i];
            float frame_length = (m_config.m_vertices[spring.m_vertex_a_index].m_position -
                                  m_config.m_vertices[spring.m_vertex_b_index].m_position).len();
            float frame_weight = frame_length * m_state.m_frame_weight * spring.m_strength;
            m_config.m_vertices[spring.m_vertex_a_index].m_mass += frame_weight / 2;
            m_config.m_vertices[spring.m_vertex_b_index].m_mass += frame_weight / 2;
        }
    }

    int Builder::current_vertex() {
        return m_config.m_vertices.size();
    }

}