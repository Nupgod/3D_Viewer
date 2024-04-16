#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float lWidth;
uniform vec4 lCol;

out vec4 color;

void main() {
    color = lCol;
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;

    vec2 dir = normalize(p2.xy - p1.xy);
    vec2 normal = vec2(-dir.y, dir.x) * lWidth / 2.0;

    gl_Position = p1 + vec4(-normal, 0.0, 0.0);
    EmitVertex();

    gl_Position = p1 + vec4(normal, 0.0, 0.0);
    EmitVertex();

    gl_Position = p2 + vec4(-normal.x, normal.y, 0.0, 0.0);
    EmitVertex();

    gl_Position = p2 + vec4(normal.x, -normal.y, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
