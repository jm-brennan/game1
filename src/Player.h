#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "ShaderManager.h"
#include "GLResourceManager.h"
#include "physics/PhysicsBody.h"
#include "definitions.h"
#include "utils/Circle.h"

using namespace glm;

class Player {
public:
    std::vector<vec2> vertices; // front, left, bottom, right
    PhysicsBody physics;
    vec2 forward;
    vec2 left;
    vec2 lookat;
    float scale, length, width;

    Player(vec2 pos = vec2(0.0f, 0.0f), float rot = 0.0f);

    void change_view(vec2 diff);
    void add_force(vec2 f, vec2 at = vec2(0.0f, 0.0f)) { physics.add_force(f, at); }
    void update(float dt) { physics.update(dt); }
    void render();

    void set_color(const vec4& c);
    //@cleanup can these be private?
    const float FORWARD_SPEED = 150.0f;
    const float FRONT_BACK_LENGTH_RATIO = 1.0f / 6.0f;
private:
    SHADER shader;
    vec2 vCursor;
    unsigned int VAO;
    float hSpeedMult = 0.2f;
    const float frictionCoeff = 40.0f;

    void look_at_vcursor();
};
