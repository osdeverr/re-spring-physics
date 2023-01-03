#pragma once

namespace Graphics {

class SceneRenderer;

}

#include "programs/shadow-projection-program.hpp"
#include "programs/shadow-mapping-program.hpp"
#include "programs/scene-rendering-program.hpp"
#include "scene-directional-light.hpp"
#include "../orthogonal_camera.hpp"
#include "shadow-map-framebuffer.hpp"
#include "geometry-pool.hpp"
#include "particles/particle-drawer.hpp"
#include "textures/texture-manager.hpp"

namespace Graphics {

class SceneRenderer {
    const Vec2i m_shadow_map_resolution = {4096, 4096};

    ShadowProjectionProgram m_shadow_projection_program {};
    ShadowMappingProgram m_shadow_mapping_program {};
    SceneRenderingProgram m_final_program {};

    ShadowMapFramebuffer m_shadow_map_framebuffer;
    TexturedFramebuffer m_shadow_buffers[2];

    TextureManager m_texture_manager {};
    GeometryPool m_geometry_pool {};
    std::vector<SceneDirectionalLight> m_directional_lights {};

    ParticleDrawer m_particle_drawer {};

    Camera* m_main_camera = nullptr;
    OrthogonalCamera m_light_camera {};

    Vec3f m_ambient_light { 0.3, 0.3, 0.3 };
    Vec2i m_screen_size { 1000, 1000 };

    int m_current_light_index = 0;

    Vec2f m_shadow_map_extent = {20, 20};
    float m_shadow_camera_distance = 20;

public:

    SceneRenderer();

    void set_screen_size(const Vec2i& size);

    void set_camera(Camera* camera) {
        m_main_camera = camera;
    }
    Camera* get_camera() const { return m_main_camera; }
    Camera* get_current_light_camera() { return &m_light_camera; }

    SceneDirectionalLight get_current_light() const { return m_directional_lights[m_current_light_index]; }
    ShadowMapFramebuffer* get_shadow_map_framebuffer() { return &m_shadow_map_framebuffer; }

    SceneDirectionalLight* get_light(int i) {
        return &m_directional_lights[i];
    }

    ParticleDrawer* get_particle_drawer() {
        return &m_particle_drawer;
    }

    int get_light_count() { return m_directional_lights.size(); }

    void add_light(SceneDirectionalLight&& light) {
        m_directional_lights.push_back(light);
    }

    GeometryPool* get_geometry_pool() { return &m_geometry_pool; }

    TexturedFramebuffer* get_active_shadow_framebuffer() {
        return &m_shadow_buffers[m_current_light_index % 2];
    }

    TexturedFramebuffer* get_previous_shadow_framebuffer() {
        if(m_current_light_index == 0) return nullptr;

        return &m_shadow_buffers[1 - m_current_light_index % 2];
    }

    TextureManager* get_texture_manager() { return &m_texture_manager; }

    static Vec3f get_light_up(const Vec3f &direction);
    void prepare_light(const SceneDirectionalLight& light);
    void reset_gl() const;
    void draw();

    Vec3f get_ambient_light();
};

}