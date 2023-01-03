#pragma once

#include <vector>
#include "scene-vertex.hpp"

namespace Graphics {

struct GeometryObjectConfig {
    std::vector<SceneVertex> m_mesh;
};

}