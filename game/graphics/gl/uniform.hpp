#pragma once

namespace Graphics {
class Uniform;
class UniformProgramNotUsedError;
}


#include <GL/glew.h>
#include <exception>
#include "program.hpp"
#include "../../utils/matrix4.hpp"
#include "../camera.hpp"
#include "../../utils/vec4.hpp"
#include "../../utils/vec2.hpp"
#include <iostream>

namespace Graphics {
class UniformProgramNotUsedError : public std::exception {
    [[nodiscard]] const char*
    what() const noexcept override { return "Cannot set uniform because its program is not active"; }
};

class Uniform {
    const Program* program;
    std::string name;
    GLint location = 0;

    void check_program_used() {
        GLint prog = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
        if((GLuint) prog != program->get_handle()) {
            throw UniformProgramNotUsedError();
        }
    }

public:
    Uniform(const Program* program, const std::string &name) : program(program), name(name) {
        location = glGetUniformLocation(program->get_handle(), name.c_str());
        if(location == -1) std::cout << "[WARNING] Uniform " << name << " could not be located\n";
    }

    void set1f(float value) {
        check_program_used();
        glUniform1f(location, value);
    };

    void set2f(float value1, float value2) {
        check_program_used();
        glUniform2f(location, value1, value2);
    };

    void set3f(float value1, float value2, float value3) {
        check_program_used();
        glUniform3f(location, value1, value2, value3);
    };

    void set4f(float value1, float value2, float value3, float value4) {
        check_program_used();
        glUniform4f(location, value1, value2, value3, value4);
    };

    void set1i(int value) {
        check_program_used();
        glUniform1i(location, value);
    };

    void set2i(int value1, int value2) {
        check_program_used();
        glUniform2i(location, value1, value2);
    };

    void set3i(int value1, int value2, int value3) {
        check_program_used();
        glUniform3i(location, value1, value2, value3);
    };

    void set4i(int value1, int value2, int value3, int value4) {
        check_program_used();
        glUniform4i(location, value1, value2, value3, value4);
    };

    void set1ui(unsigned value) {
        check_program_used();
        glUniform1ui(location, value);
    };

    void set2ui(unsigned value1, unsigned value2) {
        check_program_used();
        glUniform2ui(location, value1, value2);
    };

    void set3ui(unsigned value1, unsigned value2, unsigned value3) {
        check_program_used();
        glUniform3ui(location, value1, value2, value3);
    };

    void set4ui(unsigned value1, unsigned value2, unsigned value3, unsigned value4) {
        check_program_used();
        glUniform4ui(location, value1, value2, value3, value4);
    };

    void set2f(const Vec2f& vector) {
        set2f(vector.x, vector.y);
    }

    void set3f(const Vec3f& vector) {
        set3f(vector.x, vector.y, vector.z);
    }

    void set4f(const Vec4f& vector) {
        set4f(vector.x, vector.y, vector.z, vector.w);
    }

    void setm44f(const Matrix4f &matrix) {
        check_program_used();
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.m_data);
    }

    void set_camera_matrix(const Camera& camera) {
        setm44f(camera.get_matrix());
    }
};
}