#version 450 core

in vec2 a_position;

out vec2 v_tex_coord;

uniform mat4 Mworld;
uniform mat4 Mvp;

void main() {
    gl_Position = Mvp * Mworld * vec4(a_position, 0.0, 1.0);
    // gl_Position = vec4(a_position, 0.0, 1.0);
    
    v_tex_coord = a_position/2.0 + vec2(0.5);
}