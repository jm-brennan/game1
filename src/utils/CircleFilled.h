#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderManager.h"
#include "GLResourceManager.h"

using namespace glm;

class CircleFilled {
public:
    vec2 pos;
    CircleFilled(vec2 pos, float radius, unsigned int numSides, SHADER s = SHADER::PLAYER);
    void init();
    void render();

private:
    SHADER shader;
    unsigned int VAO, EBO, numSides;
    float radius;
    vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};
