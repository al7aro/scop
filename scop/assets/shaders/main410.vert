#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 4) in vec3 col;

uniform mat4 proj;
uniform mat4 rot;

out vec3 varying_col;
out vec2 var_tex;

void main()
{
    varying_col = col;
    var_tex = tex;
    gl_Position = proj * rot * vec4(pos, 1.0);
}