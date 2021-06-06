#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderManager.h"
#include "utils/Rectangle.h"

#include "definitions.h"

using namespace glm;

// @TODO do we want block to inherit from rectangle??
class Block {
public:
    vec2 pos;
    vec2 size;
    float rot;

    Block(vec2 pos, vec2 size, float rot, vec4 color);
    
    void render();

private:
    SHADER shader;
    Rectangle rect;
    vec4 color;
    unsigned int VAO;
};
