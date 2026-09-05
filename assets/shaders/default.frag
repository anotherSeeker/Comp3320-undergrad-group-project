#version 460 core
in vec3 aPos;
out vec4 fragColour;

in vec2 frag_UV;
in vec3 frag_Normal;

void main() {
    fragColour = vec4(frag_Normal * 0.5 + 0.5, 1.0);
}
