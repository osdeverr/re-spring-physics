
#include "map-object.hpp"
#include "../world.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../../lib/tiny_obj_loader.h"
#include "../../graphics/scene-rendering/shape-generator.hpp"

void MapObject::tick(float dt) {
    // Do nothing, the object is static
}

MapObject::MapObject(World *world, const std::string &path, const Matrix4f& transform) : WorldObject(world) {
    create_mesh(path, transform);
}

MapObject::~MapObject() {
    if (m_geometry_object) {
        auto geometry_pool = m_world->get_renderer()->get_geometry_pool();
        geometry_pool->destroy_object(m_geometry_object);
        geometry_pool->destroy_material(m_material);
    }

    for(auto triangle : m_physics_triangles) {
        m_world->get_physics_engine()->get_terrain()->destroy_triangle(triangle);
    }

    m_physics_triangles.clear();

    m_world->remove_object(this);
}

void MapObject::create_mesh(const std::string &path, const Matrix4f& transform) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::string err;

    std::cout << "Loading .obj file: " << path << "\n";

    if(!tinyobj::LoadObj(&attrib, &shapes, nullptr, &err, path.c_str(), nullptr)) {
        std::cout << "Failed to load .obj file: " << path << ": " << err << "\n";
    }

    std::cout << "Vertex count: " << attrib.vertices.size() << "\n";

    Graphics::GeometryPool* geometry_pool = m_world->get_renderer()->get_geometry_pool();
    Terrain* terrain = m_world->get_physics_engine()->get_terrain();
    Graphics::ShapeGenerator shape_generator;

    m_material = geometry_pool->create_material();
    m_material->set_color({ 0.9f, 0.4f, 0.4f });

    for(auto& shape : shapes) {
        for(int i = 0; i < (int)shape.mesh.indices.size(); i += 3) {
            int index_a = shape.mesh.indices[i].vertex_index;
            int index_b = shape.mesh.indices[i + 1].vertex_index;
            int index_c = shape.mesh.indices[i + 2].vertex_index;

            Vec3f point_a { attrib.vertices[index_a * 3 + 0], attrib.vertices[index_a * 3 + 1], attrib.vertices[index_a * 3 + 2] };
            Vec3f point_b { attrib.vertices[index_b * 3 + 0], attrib.vertices[index_b * 3 + 1], attrib.vertices[index_b * 3 + 2] };
            Vec3f point_c { attrib.vertices[index_c * 3 + 0], attrib.vertices[index_c * 3 + 1], attrib.vertices[index_c * 3 + 2] };

            point_a *= transform;
            point_b *= transform;
            point_c *= transform;

            shape_generator.add_triangle(point_a, point_b, point_c, m_material);
            m_physics_triangles.push_back(terrain->add_triangle(point_a, point_b, point_c));
        }
    }

    m_geometry_object = geometry_pool->create_object({shape_generator.get_mesh()}, nullptr);
}
