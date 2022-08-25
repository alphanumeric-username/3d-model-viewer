#include "opengl_wrapper/texture.hpp"

#include <iostream>

namespace app {

void Texture::bind() {
    if(handle_ == 0) {
        glGenTextures(1, &handle_);
    }

    glBindTexture(GL_TEXTURE_2D, handle_);
}

GLuint Texture::handle() const {
    return handle_;
}

GLuint Texture::width() const {
    return width_;
}

GLuint Texture::height() const {
    return height_;
}

void setup_texture_params() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void Texture::send_texture_data(std::vector<unsigned char> tex_data, 
    unsigned int width, unsigned int height) {
    
    bind();

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data.data()
    );
    width_ = width;
    height_ = height;

    setup_texture_params();
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::send_texture_data(std::nullptr_t tex_data, 
    unsigned int width, unsigned int height) {
    
    bind();

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data
    );
    width_ = width;
    height_ = height;

    setup_texture_params();
}

void Texture::assign_to_texture_unit(int n) {
    glActiveTexture(GL_TEXTURE0 + n);
    bind();
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::remove_from_gpu() {
    glDeleteTextures(1, &handle_);
    handle_ = 0;
}


}