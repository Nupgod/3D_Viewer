#version 330 core

layout (location = 0) in vec3 position;
uniform mat4x4 persp;
uniform int perspV;

void main() {
    if(perspV == 1) {
        gl_Position = persp * vec4(position.xyz, 1.0);
    } else {
        gl_Position = vec4(position.xyz, 1.0);
    }
}
