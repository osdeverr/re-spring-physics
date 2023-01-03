#pragma once

#include "world-object.hpp"
#include "../physics/vertex.hpp"
#include "../../graphics/scene-rendering/shape-generator.hpp"
#include "../../utils/get-orthogonal.hpp"
#include "../../graphics/scene-rendering/geometry-object.hpp"
#include "../physics/jet.hpp"

class JetObject : public WorldObject {
    Graphics::GeometryObject *m_geometry_object = nullptr;
    Graphics::Material *m_material = nullptr;
    std::unique_ptr<PhysicsJet> m_physics_jet = nullptr;
    float m_max_force = 1.0f;
    float m_particles_accumulator = 0.0f;
    float m_particle_intensity = 300.0f; // Particles per second on max thrust
public:
    JetObject(World *world, PhysicsVertex *head_vertex, PhysicsVertex *tail_vertex);

    ~JetObject() override;

    void set_max_force(float force);

    float get_max_force() const;

    void tick(float dt) override;

    void create_colored_mesh(const Vec3f &color);

    PhysicsJet *get_physics_jet();
};