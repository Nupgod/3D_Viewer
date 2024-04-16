#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

uniform vec4 vCol;
uniform float pSize;

out vec4 Col;

void main() {
    Col = vCol;
    vec4 center = gl_in[0].gl_Position;
    // Define the vertices of the square
    vec4 vertices[5] = vec4[5](
        vec4(center.x - pSize, center.y - pSize, center.z, center.w),
        vec4(center.x + pSize, center.y - pSize, center.z, center.w),
        vec4(center.x + pSize, center.y + pSize, center.z, center.w),
        vec4(center.x - pSize, center.y + pSize, center.z, center.w),
        vec4(center.x - pSize, center.y - pSize, center.z, center.w)
    );

    // Emit the vertices of the square
    for (int i = 0; i < 5; i++) {
        gl_Position = vertices[i];
        EmitVertex();
    }

    EndPrimitive();
}
