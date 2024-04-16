#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 256) out;

uniform vec4 lCol;
uniform float lWidth;

out vec4 color;

void createQuad(vec4 p1, vec4 p2, vec4 p3, vec4 p4) {
    gl_Position = p1;
    color = lCol;
    EmitVertex();

    gl_Position = p2;
    color = lCol;
    EmitVertex();

    gl_Position = p3;
    color = lCol;
    EmitVertex();

    gl_Position = p4;
    color = lCol;
    EmitVertex();

    EndPrimitive();
}
void main() {
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;

    vec2 dir = normalize(p2.xy - p1.xy);
    vec2 normal = vec2(-dir.y, dir.x);
    vec2 offset = normal * lWidth / 2.0; // Adjust the offset as needed

    float len = length(p2.xy - p1.xy);
    float dashLength = len / 3;
    float gapLength = dashLength / 10;
    float currentLength = 0.0;

    while (currentLength < len) {
        // Dash
            vec2 position = mix(p1.xy, p2.xy, currentLength / len);
            vec4 p1Quad = vec4(position - offset, p1.z, p1.w);
            vec4 p2Quad = vec4(position + offset, p1.z, p1.w);
            vec4 p3Quad = vec4(position - offset + vec2(0.0, lWidth), p1.z, p1.w);
            vec4 p4Quad = vec4(position + offset + vec2(0.0, lWidth), p1.z, p1.w);
            createQuad(p1Quad, p2Quad, p3Quad, p4Quad);
            currentLength += dashLength + gapLength;
        // Gap
        float gapEnd = min(currentLength + gapLength, len);
        while (currentLength < gapEnd) {
            currentLength += gapLength;
        }
    }
}
