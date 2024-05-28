#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 4) in vec3 col;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_src_pos;

out vec3 varying_col;
out vec3 varying_norm;
out vec3 frag_pos;
out vec2 var_tex;

void main()
{
    varying_col = col;
    varying_norm = norm;
    var_tex = tex;
    gl_Position = proj * view * model * vec4(pos, 1.0);
    frag_pos = vec3(gl_Position);
}