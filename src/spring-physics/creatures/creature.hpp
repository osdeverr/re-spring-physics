#pragma once

#include <vector>
#include "../objects/vertex-object.hpp"
#include "../objects/spring-object.hpp"
#include "creature-config.hpp"
#include "../objects/muscle.hpp"
#include "../../utils/vec2.hpp"
#include "../objects/surface-object.hpp"
#include "../objects/jet-object.hpp"

class Creature : public WorldObject {
    std::vector<VertexObject*> m_vertices {};
    std::vector<SpringObject*> m_springs {};
    std::vector<Muscle*> m_muscles {};
    std::vector<JetObject*> m_jet_objects {};
    std::vector<SurfaceObject*> m_surfaces {};

public:
    Creature(World* world, const CreatureConfig& config);

    ~Creature() override;

    Vec3f get_center() const;

    const std::vector<VertexObject*>& get_vertices() { return m_vertices; }
    const std::vector<SpringObject*>& get_springs() { return m_springs; }
    const std::vector<SurfaceObject*>& get_surfaces() { return m_surfaces; }
    const std::vector<Muscle*>& get_muscles() { return m_muscles; }
    const std::vector<JetObject*>& get_jet_objects() { return m_jet_objects; }

    void tick(float dt) override;
    void physics_tick();

    void make_visible() {
        for(auto& vertex : m_vertices) {
            float friction = vertex->get_physics_vertex()->m_floor_friction;
            friction = (friction - 0.01) / (2.0 - 0.01);

            vertex->create_colored_mesh({1, 1 - friction, 1 - friction});
        }

        for(auto& muscle : m_muscles) {
            float strength = muscle->m_spring->get_physics_spring()->m_strength;
            strength = (strength - 25.0) / (100.0 - 25.0);

            muscle->m_spring->create_colored_mesh({1 - strength, 1, 1 - strength});
        }

        for(auto& spring : m_springs) {
            float strength = spring->get_physics_spring()->m_strength;
            strength = (strength - 25.0) / (100.0 - 25.0);

            if(!spring->get_geometry_object()) spring->create_colored_mesh({1 - strength, 1 - strength, 1});
        }

        for(auto& jet : m_jet_objects) {
            jet->create_colored_mesh({0, 1, 0});
        }

        for(auto& surface : m_surfaces) {
            surface->create_colored_mesh({0.5, 0.5, 0.5});
        }
    }
};