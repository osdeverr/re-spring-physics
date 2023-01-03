#pragma once

#include <map>
#include <unordered_set>
#include "../gl/gl-buffer.hpp"
#include "../gl/gl-texture-buffer.hpp"
#include "../../utils/vec2.hpp"
#include "../../utils/index-pool.hpp"
#include "../../utils/linked-list.hpp"
#include "geometry-object.hpp"
#include "geometry-object-config.hpp"
#include "material.hpp"
#include "scene-vertex.hpp"

namespace Graphics {

class GeometryPool {
    const int material_stride = 8;
    const int transform_stride = 16;

    std::vector<GLint> start_indices {};
    std::vector<GLsizei> size_array {};

    std::unique_ptr<GLBuffer<float>> m_vertex_buffer {};
    BufferRange m_vertex_buffer_dirty_range {};

    // TODO: It seems that these two identical field groups could be wrapped in some nice fancy structures

    std::unique_ptr<GLTextureBuffer<float>> m_matrix_buffer {};
    BufferRange m_matrix_buffer_dirty_range {};
    IndexPool<int> m_matrix_buffer_index_pool {};
    std::unordered_set<GeometryObject*> m_dirty_transform_objects {};

    std::unique_ptr<GLTextureBuffer<float>> m_material_buffer {};
    BufferRange m_material_buffer_dirty_range {};
    IndexPool<int> m_material_buffer_index_pool {};
    std::unordered_set<Material*> m_dirty_materials {};

    DoubleEndedLinkedList<GeometryObject> m_objects {};
    std::unordered_set<Material*> m_materials {};

    GeometryObject* m_defragmented_elements = nullptr;

    void defragment_single_object(GeometryObject* object, int offset);

    static void extend_buffer(std::vector<float>& buffer, int min_capacity);
    int allocate_buffer(int size);
    int get_free_vertex_buffer_offset();

    void copy_geometry(int offset, const std::vector<SceneVertex>& vertices, int matrix_index);

    void insert_offsets_to_arrays(int free_index, int buffer_stride);
    void remove_offset_from_arrays(int offset);
    void adjust_offset_from_arrays(int offset, int new_offset);

    int create_matrix();
    void copy_matrix(int index, const Matrix4f& matrix);

public:

    GLint* get_start_indices() { return &start_indices[0]; };
    GLsizei* get_size_array() { return &size_array[0]; };
    int get_object_count() { return m_objects.size(); }

    GeometryPool();

    GeometryObject* create_object(const GeometryObjectConfig &object_config, GeometryObject* parent);
    void destroy_object(GeometryObject* object);

    GLBuffer<float>* get_vertex_buffer() { return m_vertex_buffer.get(); };
    GLTextureBuffer<float>* get_material_buffer() { return m_material_buffer.get(); };
    GLTextureBuffer<float>* get_matrix_buffer() { return m_matrix_buffer.get(); };

    Material* create_material();
    void destroy_material(Material* material);

    void synchronize();
    void defragment_buffer(int limit);

    void update_transforms();
    void update_materials();

    void update_material_delayed(Material* material);
    void update_transform_delayed(GeometryObject* object);

    void update_object_transform(GeometryObject* object);
    void update_material(Material* material);
};

}