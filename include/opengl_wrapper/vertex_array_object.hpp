#pragma once

#include "glad/glad.h"

#include <string>
#include <vector>

namespace app {

class VertexArrayObject{
private:
    GLuint handle_{0};
    std::vector<GLuint> buffer_handles_{};

public:
    void bind();
    void unbind();
    void set_attribute(GLint attrib_location, const std::vector<float>& data, int size);
    void remove_from_gpu();
};

}