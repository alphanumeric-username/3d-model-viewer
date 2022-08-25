#version 450 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_tex_coord;

out vec4 FragColor;

struct Material {
    vec3 Kd;
    vec3 Ks;
    vec3 Ka;
    float Ns;

    sampler2D map_Kd;
    sampler2D map_Ks;
    sampler2D map_Ka;

    bool has_map_Kd;
    bool has_map_Ks;
    bool has_map_Ka;
};

uniform vec3 light_dir;
uniform Material mat;

vec4 blinn_color(vec3 w, vec3 n, vec3 v, vec4 I, Material m, vec2 tex_coord);

void main() {
    vec3 unit_normal = normalize(v_normal);
    vec3 unit_position = normalize(v_position);
    vec3 unit_light_dir = normalize(light_dir);

    // Material mat;
    // mat.Kd = vec4(1.0, 0.0, 0.0, 1.0);
    // mat.Ks = vec4(1.0);
    // mat.Ns = 500.0;
    // mat.Ka = vec4(0.2, 0.0, 0.0, 1.0);

    FragColor = blinn_color(unit_light_dir, unit_normal, -unit_position, vec4(1.0), mat, v_tex_coord);
    // FragColor = vec4(vec3(0.5) + 0.5*unit_normal, 1.0);
    // FragColor = vec4(v_tex_coord, 0.0, 1.0);
    // FragColor = vec4(0.5, 0.5, 0.5, 1.0) + 
    // 0.0000001*(vec4(v_position, 1.0) + vec4(v_normal, 1.0) + vec4(v_tex_coord, 0.0, 1.0));
}

vec4 blinn_color(vec3 w, vec3 n, vec3 v, vec4 I, Material m, vec2 tex_coord) {
    vec3 r = normalize(w + v);

    vec4 Kd = vec4(m.Kd, 1.0);
    if(m.has_map_Kd) {
        Kd = Kd*texture(m.map_Kd, tex_coord);
    }

    vec4 Ks = vec4(m.Ks, 1.0);
    if(m.has_map_Ks) {
        Ks = Ks*texture(m.map_Ks, tex_coord);
    }

    vec4 Ka = vec4(m.Ka, 1.0);
    if(m.has_map_Ka) {
        Ka = Ka*texture(m.map_Ka, tex_coord);
    }

    vec4 diffuse = clamp(dot(w, n)*I*Kd, 0.0, 1.0);
    vec4 specular = clamp(pow(dot(n, r), m.Ns)*I*Ks, 0.0, 1.0);
    vec4 ambient = Ka;

    return specular + diffuse + ambient;
}
