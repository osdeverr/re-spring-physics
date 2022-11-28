//
// Created by Артем on 06.02.2022.
//

#include "material.hpp"
#include "geometry-pool.hpp"

Vec3f Graphics::Material::get_color() {
    return m_color;
}

void Graphics::Material::set_color(const Vec3f &color) {
    m_color = color;
    m_geometry_pool->update_material_delayed(this);
}

int Graphics::Material::get_buffer_index() {
    return m_buffer_index;
}

void Graphics::Material::set_buffer_index(int index) {
    m_buffer_index = index;
}

bool Graphics::Material::get_grid() {
    return m_grid;
}

void Graphics::Material::set_grid(bool grid) {
    m_grid = grid;
    m_geometry_pool->update_material_delayed(this);
}

float Graphics::Material::get_specular_factor() {
    return m_specular_factor;
}

void Graphics::Material::set_specular_factor(float factor) {
    m_specular_factor = factor;
    m_geometry_pool->update_material_delayed(this);
}
