#pragma once

namespace Graphics {

class GeometryPool;

}

#include <vector>
#include "../../utils/matrix4.hpp"

namespace Graphics {

struct GeometryObject {
    GeometryPool* m_pool;

    int m_vertex_buffer_offset;
    int m_vertex_buffer_length;
    int m_matrix_buffer_index;
    GeometryObject* m_parent = nullptr;

    bool m_needs_transform_update = false;

    GeometryObject* m_next = nullptr;
    GeometryObject* m_prev = nullptr;

    std::vector<GeometryObject*> m_children {};

    Matrix4f m_transform {};
    Matrix4f m_world_transform {};

    GeometryObject(GeometryPool* pool,
                   int vertex_buffer_offset,
                   int vertex_buffer_length,
                   int matrix_buffer_index,
                   GeometryObject* parent);

    void set_needs_transform_update();

    void update_transform();
    void update_subsidiary_transformations();

    void set_transform(const Matrix4f &matrix4);
};

}