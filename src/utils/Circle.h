#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderManager.h"
#include "GLResourceManager.h"
#include "definitions.h"

using namespace glm;

class Circle {
public:
    Circle(vec2 pos, float radius, unsigned int resolution, bool wire);
    
    void move(vec2 newPos);

    void render();

private:
    SHADER shader;
    vec2 pos;
    unsigned int VAO, numSides;
    bool wire;
    float radius;
};