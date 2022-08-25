#include "opengl_wrapper/vertex_array_object.hpp"

namespace app {

void VertexArrayObject::bind() {
    if(handle_ == 0) {
        glGenVertexArrays(1, &handle_);
    }

    glBindVertexArray(handle_);
}

void VertexArrayObject::unbind() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArrayObject::set_attribute(GLint attrib_location, const std::vector<float>& data, int size) {
    GLint program{0};
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    if(program > 0) {
        // glUseProgram(program);
        bind();

        GLuint vbo{0};

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), data.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(attrib_location, size, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(attrib_location);
        
        buffer_handles_.emplace_back(vbo);
    }
}

void VertexArrayObject::remove_from_gpu() {
    if(handle_ > 0) {
        glDeleteVertexArrays(1, &handle_);
    }

    for(auto vbo : buffer_handles_) {
        glDeleteBuffers(1, &vbo);
    }
}

}