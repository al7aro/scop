#version 460 core

#define MAX_POINT_LIGHTS 64
struct point_light
{
    vec3 pos;
    vec3 col;
    float intensity;
};

struct material
{
    float Ns;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Ke;
    float Ni;
    float d;
    int illum;

    sampler2D diffuse_map;
    bool diffuse_map_enabled;

    sampler2D bump_map;
    bool bump_map_enabled;

    sampler2D specular_map;
    bool specular_map_enabled;
};

out vec4 frag_color;

in vec2 var_tex;
in vec3 varying_col;
in vec3 varying_norm;
in vec3 frag_pos;

uniform float offset;
uniform vec3 light_src_pos;

uniform point_light lights[MAX_POINT_LIGHTS];
uniform material mat;
uniform int point_light_cnt;

void main()
{
    vec3 norm = normalize(varying_norm);
    vec3 dir = normalize(light_src_pos - frag_pos);

    float diff = max(dot(norm, dir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    frag_color = vec4(diffuse + mat.Ka, 1.0) * vec4(1.0, 0.0, 0.0, 1.0);
    frag_color = vec4(mat.Kd, 1.0);

    if (mat.diffuse_map_enabled)
        frag_color = texture(mat.diffuse_map, var_tex);
}