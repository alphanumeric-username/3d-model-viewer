#version 450 core

in vec3 a_position;
in vec3 a_normal;
in vec2 a_tex_coord;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_tex_coord;

uniform mat4 Mworld;
uniform mat4 Mvp;

void main() {
    gl_Position = Mvp * Mworld * vec4(a_position, 1.0);
    
    v_position = (Mworld * vec4(a_position, 1.0)).xyz;
    v_normal = (transpose(inverse(Mworld)) * vec4(a_normal, 0.0)).xyz;
    v_tex_coord = a_tex_coord;
}