#version 460 core

#define MAX_POINT_LIGHTS 64
struct point_light
{
    vec3 pos;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
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

uniform point_light lights[MAX_POINT_LIGHTS];
uniform int point_light_cnt;
uniform material mat;
uniform vec3 view_pos;

vec4 compute_point_lights()
{
    vec3 material_diffuse, material_specular, material_bump;
    if (mat.diffuse_map_enabled)
        material_diffuse = vec3(texture(mat.diffuse_map, var_tex));
    else
        material_diffuse = mat.Kd;
    if (mat.specular_map_enabled)
        material_specular = vec3(texture(mat.specular_map, var_tex));
    else
        material_specular = mat.Ks;
    if (mat.bump_map_enabled)
        material_bump = vec3(texture(mat.bump_map, var_tex));
    else
        material_bump = varying_norm;

    vec4 col = vec4(0.0);
    vec3 ambient, diffuse, specular;
    for (int i = 0; i < point_light_cnt; i++)
    {
        point_light light = lights[i];
        // AMBIENT
        ambient = material_diffuse * mat.Ka * light.ambient;
        // DIFFUSE LIGHT
        vec3 light_dir = normalize(light.pos - frag_pos);
        vec3 norm = normalize(material_bump);
        float diff = max(dot(norm, light_dir), 0.0);
        diffuse = light.diffuse * diff * material_diffuse;
        // SPECULAR TODO: SOMETHING WEIRD HAPPENS TO THE SPECULAR COMPONENT OF LIGHT
        vec3 view_dir = normalize(view_pos - frag_pos);
        vec3 reflec_dir = reflect(-light_dir, norm);
        float spec = pow(max(dot(view_dir, reflec_dir), 0.0), mat.Ns);
        specular = max(light.specular * spec * material_specular, vec3(0.0));

        col += (vec4(diffuse, 1.0) + vec4(ambient, 1.0) + vec4(specular, 1.0));
    }
    return col;
}

void main()
{
    frag_color = compute_point_lights();
}