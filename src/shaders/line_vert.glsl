#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 normal;

uniform float linewidth;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 delta = vec4(normal * linewidth, 0, 0);
    vec4 pos = view * model * vec4(position, 0, 1);
    gl_Position = projection * (pos + delta);
}