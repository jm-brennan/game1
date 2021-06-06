#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderManager.h"
#include "GLResourceManager.h"

#include "definitions.h"

using namespace glm;

class Rectangle {
public:
    Rectangle() {}
    Rectangle(vec2 pos, vec2 size, vec4 color, SHADER s);
    void set_color(const vec4& c);
    void render();
  
    vec2 pos = vec2(0.0f);
    vec2 size = vec2(0.0f);
private:
    // will use different shader based on space to make
    // rectangles in world space (for game objects) or screen space for UI
    SHADER shader;
    vec4 color = vec4(0.0f);

    unsigned int VAO = 0;
    bool wire = false;
};