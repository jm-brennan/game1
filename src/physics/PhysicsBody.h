#pragma once

#include <stdio.h>
#include <glm/glm.hpp>
using namespace glm;

class PhysicsBody {
public:
    PhysicsBody() {}
    PhysicsBody(vec2 pos, float rot, float mass, float moment, float frictionCoeff);
    void add_force(vec2 f, vec2 at = vec2(0.0f, 0.0f));
    void update(float dt);
    void rotate_to(float r);

    vec2 pos;
    vec2 vel;
    vec2 forces;
    vec2 forcesAt;
    float rot;
    float mass;
    float moment;
    //float angVel;
    float frictionCoeff;
private:
    void apply_friction();
};
