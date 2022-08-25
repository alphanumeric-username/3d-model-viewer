#version 450 core

in vec2 v_tex_coord;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = clamp(texture(tex, v_tex_coord) + vec4(0.2, 0.2, 0.2, 0.0), 0.0, 1.0);
    // FragColor = vec4(0.5, 0.5, 0.5, 1.0);
}
