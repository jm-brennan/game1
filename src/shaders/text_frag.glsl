#version 330 core
in vec2 texCoords;

uniform sampler2D text;
uniform vec4 color;

void main() {
    gl_FragColor = color * vec4(1.0, 1.0, 1.0, texture2D(text, texCoords).r);
}