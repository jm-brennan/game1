#version 330 core
layout (location = 0) in vec2 position;

out float alpha;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    alpha = 1.0;
    gl_Position = projection * view * model * vec4(position.xy, 0.0, 1.0);
}