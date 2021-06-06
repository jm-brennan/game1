#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderManager.h"
#include "GLResourceManager.h"
#include "physics/PhysicsBody.h"
#include "definitions.h"

#include "utils/CircleFilled.h"

using namespace glm;    

class Boid {
public:
    Boid(vec2 pos = vec2(0.0f, 0.0f), float rot = 0.0f);
    void add_destination(vec2 loc);

    void render();
    PhysicsBody physics;
    float speed = 100.0f;
    float maxForce = 75.0f;
    float frictionCoeff = 40.0f;
private:
    //CircleFilled* circle = nullptr; // @DEBUG
    SHADER shader;
    std::vector<vec2> vertices;
    vec2 destination;
    unsigned int VAO;
};