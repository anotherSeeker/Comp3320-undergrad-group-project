#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aUV;

uniform mat4 camera;
uniform mat4 model;

out vec2 frag_UV;
out vec3 frag_Normal;

void main() {
    gl_Position = camera * model * vec4(aPos, 1);
    frag_UV = aUV;
    frag_Normal = aNorm;
}
