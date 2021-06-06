#version 330 core
layout (location = 0) in vec4 posAndTexture; // vec2 of position +vec2 of texture
out vec2 texCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(posAndTexture.xy, 0.0, 1.0);
    texCoords = posAndTexture.zw;
}