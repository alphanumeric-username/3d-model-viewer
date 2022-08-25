#include "opengl_wrapper/shader_program.hpp"
#include "io.hpp"

#include <fstream>
#include <iostream>
#include <cstdlib>

namespace app {

GLuint create_shader(std::string src, int type) {
    auto shader = glCreateShader(type);

    GLint len{static_cast<GLint>(src.length())};
    const GLchar* const c_src = src.c_str();

    glShaderSource(shader, 1, &c_src, &len);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(success == GL_FALSE) {
        int info_log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

        int length;
        char* info_log = new char[info_log_length];
        glGetShaderInfoLog(shader, info_log_length, &length, info_log);

        std::cerr << "[OPEN GL]: Could not compile shader:\n"
                  << "Info log: " << info_log << '\n';
        
        delete[] info_log;
    }

    return shader;
}

GLuint create_program(GLuint vertex_shader, GLuint fragment_shader) {
    auto program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    if(success == GL_FALSE) {
        int info_log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

        int length;
        char* info_log = new char[info_log_length];
        glGetProgramInfoLog(program, info_log_length, &length, info_log);

        std::cerr << "[OPEN GL]: Could not link shader program:\n"
                  << "Info log: " << info_log << '\n';
        
        delete[] info_log;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

ShaderProgram::ShaderProgram() {}

ShaderProgram::ShaderProgram(fs::path vertex_shader_src_path, fs::path fragment_shader_src_path) {
    std::string vs_src{read_file(vertex_shader_src_path)};
    std::string fs_src{read_file(fragment_shader_src_path)};

    auto vs = create_shader(vs_src, GL_VERTEX_SHADER);
    auto fs = create_shader(fs_src, GL_FRAGMENT_SHADER);

    handle_ = create_program(vs, fs);
}

GLuint ShaderProgram::handle() const {
    return handle_;
}

void ShaderProgram::use() const {
    glUseProgram(handle_);
}

GLint ShaderProgram::get_uniform_location(std::string name) const {
    use();
    return glGetUniformLocation(handle_, name.c_str());
}

GLint ShaderProgram::get_attrib_location(std::string name) const {
    use();
    return glGetAttribLocation(handle_, name.c_str());
}

void ShaderProgram::delete_program() {
    glDeleteProgram(handle_);
    handle_ = 0;
}

// void ShaderProgram::set_uniform(std::string name, glm::mat4 value) const {
//     auto location = get_uniform_location(name);

//     glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
// }

}