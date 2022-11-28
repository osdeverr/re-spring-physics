#pragma once

namespace Graphics {
class GLBufferBase;
}

#include <GL/glew.h>
#include <vector>
#include "../../error-handling.hpp"
#include "gl-enums.hpp"
#include "../../utils/buffer-range.hpp"

namespace Graphics {



class GLBufferBase {
protected:
    GLuint m_gl_buffer_handle = 0;
    long gl_buffer_capacity = -1;
    GLenum m_gl_type = GL_INT;
    int m_gl_size = 4;
    GLBufferType m_type;
    GLBufferUsage m_usage;
public:
    explicit GLBufferBase(GLBufferType type, GLBufferUsage usage = GLBufferUsage::static_draw): m_type(type), m_usage(usage) {}
    virtual ~GLBufferBase() = default;
    virtual void create_buffer() {
        glGenBuffers(1, &m_gl_buffer_handle);
        GLException::check();
    }

    virtual void synchronize(BufferRange range = BufferRange::global) = 0;

    virtual void bind() { glBindBuffer((GLenum)m_type, m_gl_buffer_handle); }

    GLuint get_handle() const { return m_gl_buffer_handle; }
    GLenum get_gl_type() const { return m_gl_type; }
    int get_gl_size() const  { return m_gl_size; }
};

template<typename T>
class GLBuffer : public GLBufferBase {
    std::vector<T> m_host_array;
public:
    explicit GLBuffer(GLBufferType type, GLBufferUsage usage = GLBufferUsage::static_draw): GLBufferBase(type, usage), m_host_array() {
        set_gl_type();
    };

    void synchronize(BufferRange range = BufferRange::global) override;
    void set_gl_type();

    std::vector<T>& get_storage() { return m_host_array; }
    const std::vector<T>& get_storage() const { return m_host_array; };
};

template<typename T>
void GLBuffer<T>::synchronize(BufferRange range) {
    glBindBuffer((GLenum)m_type, m_gl_buffer_handle);
    if((long)(m_host_array.size() * sizeof(T)) != gl_buffer_capacity) {
        glBufferData((GLenum)m_type, m_host_array.size() * sizeof(T), &m_host_array[0], (GLenum)m_usage);
        gl_buffer_capacity = m_host_array.size() * sizeof(T);
    } else {
        range.trim(0, m_host_array.size());

        glBufferSubData((GLenum)m_type, range.m_from * sizeof(T), range.size() * sizeof(T), &m_host_array[range.m_from]);
    }
    GLException::check();
}

}