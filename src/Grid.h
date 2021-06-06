#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "ShaderManager.h"
#include "GLResourceManager.h"

using namespace glm;

class Grid {
public:
    Grid();
    void init();
    void render();

private:
    SHADER shader;
    const int size = 20;
    unsigned int VAO;

};



