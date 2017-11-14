#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

uniform vec4 Color;
out vec4 out_Color;

void main(void) {
    vec3 dif = vec3(exNormal.x*0.2 + exNormal.y*0.1 , exNormal.z*0.2 + exNormal.y*0.1, exNormal.z*0.1 + exNormal.x*0.1 +  exNormal.y * 0.2);
    out_Color = vec4(Color.x + dif.x, Color.y + dif.y, Color.z + dif.z, 1);
}
