#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>
#include "Boid.h"

using namespace glm;

class BoidSwarm {
public:
    BoidSwarm(int numBoids);
    void update(float dt);
    vec2 avoid_edges(Boid& b);
    vec2 separate(Boid& b);
    vec2 go_toward_center(Boid& b);
    vec2 align(Boid& b);
    void render();
    
    // world coordinate distances
    float separationRadius = 0.75f;
    float neighborRadius = 3.0f;

    float avoidEdgeWeight = 1.0f;
    float towardCenterWeight = 0.75f;
    float separateWeight = 1.0f;
    float alignWeight = 1.0f;

private:
    std::vector<Boid> boids;
    vec2 boundary = vec2(25.0f, 15.0f);
};