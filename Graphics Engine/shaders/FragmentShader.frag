#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

uniform vec4 Color;
out vec4 out_Color;

void main(void) {
    out_Color = Color;
}
