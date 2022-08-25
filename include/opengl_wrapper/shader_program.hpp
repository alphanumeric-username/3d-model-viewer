#pragma once

#include "glad/glad.h"

#include <filesystem>

namespace app {

namespace fs = std::filesystem;

class ShaderProgram {
private:
    GLuint handle_;

public:
    ShaderProgram();
    ShaderProgram(fs::path vertex_shader_src_path, fs::path fragment_shader_src_path);

    GLuint handle() const;
    GLint get_uniform_location(std::string name) const;
    GLint get_attrib_location(std::string name) const;

    void use() const;
    void delete_program();

    // void set_uniform(std::string name, glm::mat4 value) const;
};

}