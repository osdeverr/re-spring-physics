#pragma once

#include "../../camera.hpp"
#include "../../gl/vertex-fragment-program.hpp"
#include "scene-program.hpp"
#include "../../gl/uniform.hpp"

namespace Graphics {

class ShadowProjectionProgram : public SceneProgram {

    Uniform m_light_camera_matrix_uniform;
public:
    ShadowProjectionProgram();

    void draw();
};

}