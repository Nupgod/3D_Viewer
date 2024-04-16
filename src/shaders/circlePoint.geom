#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 44) out; // 360 vertices for the circle + 2 for the center

uniform vec4 vCol;
uniform float pSize;

out vec4 Col;

const float PI = 3.14159265359;
const float twoPI = PI * 2.0;

void main() {

    Col = vCol;
    vec4 center = gl_in[0].gl_Position;
    float radius = pSize - 0.0005; // Adjust the radius as needed
    int vertNum = 20;
    float angle = twoPI / vertNum;
    // Emit the center of the circle

    // Generate vertices around the circle
    for (int i = 0; i <= vertNum; i++) {
        gl_Position = vec4(center.x, center.y, center.z, center.w);
        EmitVertex();
        float x = (cos(angle * i) * radius + center.x);
        float y = (sin(angle * i) * radius + center.y);
        gl_Position = vec4(x, y, center.z, center.w);
        EmitVertex();
    }
    EndPrimitive();
}
