#pragma once

#include "glad/glad.h"

#include <vector>

namespace app {

class Texture {
private:
    GLuint handle_{0};
    GLuint width_;
    GLuint height_;

public:
    GLuint handle() const;
    GLuint width() const;
    GLuint height() const;

    void bind();
    void send_texture_data(std::vector<unsigned char> tex_data, unsigned int width, unsigned int height);
    void send_texture_data(std::nullptr_t tex_data, unsigned int width, unsigned int height);
    void assign_to_texture_unit(int n);
    void unbind();
    void remove_from_gpu();
};

}