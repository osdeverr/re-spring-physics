#pragma once

#include <vector>
#include "physics/physics-engine.hpp"
#include "../graphics/scene-rendering/scene-renderer.hpp"
#include "objects/world-object.hpp"
#include "../graphics/scene-rendering/particles/particle-host.hpp"

class World {
    std::unique_ptr<ConcurrentPhysicsEngine> m_physics_engine {};
    std::unique_ptr<Graphics::SceneRenderer> m_renderer {};
    std::unique_ptr<Graphics::ParticleHost> m_particle_host {};
    std::unordered_set<WorldObject*> m_objects {};

    Graphics::GeometryObject* m_floor = nullptr;
    Graphics::Material* m_floor_material = nullptr;

public:
    void add_floor();

    World() {
        m_physics_engine = std::make_unique<ConcurrentPhysicsEngine>();
        m_renderer = std::make_unique<Graphics::SceneRenderer>();
        m_particle_host = std::make_unique<Graphics::ParticleHost>();

        add_floor();
    }

    ~World() {
        m_renderer->get_geometry_pool()->destroy_material(m_floor_material);
    }

    void set_screen_size(const Vec2i& size) { m_renderer->set_screen_size(size); }
    Graphics::SceneRenderer* get_renderer() { return m_renderer.get(); }
    ConcurrentPhysicsEngine* get_physics_engine() { return m_physics_engine.get(); }
    Graphics::ParticleHost* get_particle_host() { return m_particle_host.get(); }

    void tick(float dt);
    void physics_tick(float dt);

    void draw() { m_renderer->draw(); }

    void add_object(WorldObject* object) { m_objects.insert(object); }
    void remove_object(WorldObject* object) { m_objects.erase(object); }
};