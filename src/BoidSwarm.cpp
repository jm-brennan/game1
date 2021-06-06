#include "BoidSwarm.h"

BoidSwarm::BoidSwarm(int numBoids) {
    for (int i = 0; i < numBoids; i++) {
        vec2 newPos = vec2(linearRand(-20.0f, 20.0f), linearRand(-20.0f, 20.0f));
        float newRot = linearRand(0.0f, PI_2);
        Boid b = Boid(newPos, newRot);
        boids.push_back(b);
    }
}

void BoidSwarm::update(float dt) {
    for (Boid& b : boids) {
        vec2 forward = normalize(b.physics.vel);
        vec2 forceToAdd = vec2(forward * 1.0f * b.speed);
        b.physics.add_force(forceToAdd);

        b.physics.add_force(avoidEdgeWeight * avoid_edges(b));
        b.physics.add_force(separateWeight * separate(b));
        b.physics.add_force(towardCenterWeight * go_toward_center(b));
        b.physics.add_force(alignWeight * align(b));

        b.physics.update(dt);
        forward = normalize(b.physics.vel); // always just rotate to match velocity
        float angleCorrector = 0.0f;
        if (forward.y < 0) angleCorrector = PI;
        b.physics.rotate_to(angleCorrector - atan(forward.x / forward.y));
    }
}

// @TODO change so that boids cannot see behind them?
vec2 BoidSwarm::separate(Boid& b) {
    float awarenessRadius = separationRadius * separationRadius;
    vec2 steer = vec2(0.0f, 0.0f);
    int numSteerAwayFrom = 0;
    vec2 away;
    for (Boid& other : boids) {
        float dist = length2(b.physics.pos - other.physics.pos);
        if (dist < awarenessRadius && dist > 0.0001f) {
            away = b.physics.pos - other.physics.pos;
            away = normalize(away);
            // @TODO at least bind parameter to size of boid, but more importantly
            // there needs to be more awareness of the shape of the boid
            away /= max(dist - 0.2f, 0.0001f);
            steer += away;
            numSteerAwayFrom++;
        }
    }
    // @TODO is this necessary? cause we normalize after
    if (numSteerAwayFrom > 0) {
        steer /= (float)numSteerAwayFrom;
    } else {
        return vec2(0.0f, 0.0f);
    }

    if (length2(steer) >= 0.0001f) {
        steer = normalize(steer);
        steer *= b.speed;
        steer -= b.physics.vel;
        // @TODO limit, dont just auto set to cap
        steer = normalize(steer) * b.maxForce;
        return steer;
    }
    return vec2(0.0f, 0.0f);
}

vec2 BoidSwarm::avoid_edges(Boid& b) {
    bool needSteer = false;
    // @TODO tune boundary check parameter, make var
    if (abs(b.physics.pos.x) >= boundary.x - 1.5f) needSteer = true; 
    if (abs(b.physics.pos.y) >= boundary.y - 1.5f) needSteer = true; 
    if (!needSteer) return vec2(0.0f, 0.0f);

    vec2 steer = vec2(0.0f, 0.0f);
    vec2 away = vec2(0.0f, 0.0f);

    if (abs(b.physics.pos.x) > boundary.x) {
        away = -b.physics.pos;
        // @TODO TUNE PARAMETER
        away *= 20.0f;
    } else {
        int negativeMultiplier = b.physics.pos.x > 0.0f ? 1 : -1;
        away = b.physics.pos - vec2(boundary.x*negativeMultiplier, b.physics.pos.y);
        away = normalize(away) / (boundary.x - b.physics.pos.x);
    }
    steer += away;
    
    away = vec2(0.0f, 0.0f);
    if (abs(b.physics.pos.y) > boundary.y) {
        away = -b.physics.pos;
        // @TODO TUNE PARAMTER
        away *= 20.0f;
    } else {
        int negativeMultiplier = b.physics.pos.y > 0.0f ? 1 : -1;
        away = b.physics.pos - vec2(b.physics.pos.x, boundary.y*negativeMultiplier);
        away = normalize(away) / (boundary.y - b.physics.pos.y);
    }
    steer += away;

    steer = normalize(steer) * b.speed;
    steer -= b.physics.vel;
    steer = normalize(steer) * b.maxForce; // @TDOD limit, dont auto set to cap
    return steer;
}

vec2 BoidSwarm::go_toward_center(Boid& b) {
    vec2 centerOfMass = vec2(0.0, 0.0f);
    float neighborDist = neighborRadius * neighborRadius;
    int numNeighbors = 0;
    for (Boid& other : boids) {
        float dist = length2(b.physics.pos - other.physics.pos);
        if (dist < neighborDist && dist > 0.0001f) {
            centerOfMass += other.physics.pos;
            numNeighbors++;
        }
    }

    if (numNeighbors > 0) {
        // dont count this boid in the center of mass
        centerOfMass /= numNeighbors;

        vec2 steer = centerOfMass - b.physics.pos;
        steer = normalize(steer);
        steer *= b.speed;
        steer -= b.physics.vel;
        steer = normalize(steer) * b.maxForce; // @TODO limit, dont auto set to cap
        return steer;
    } else {
        return vec2(0.0f, 0.0f);
    }
}

vec2 BoidSwarm::align(Boid& b) {
    float neighborDist = neighborRadius * neighborRadius;
    int numNeighbors = 0;
    vec2 avgVel = vec2(0.0f, 0.0f);
    for (Boid& other : boids) {
        float dist = length2(b.physics.pos - other.physics.pos);
        if (dist < neighborDist && dist > 0.0001f) {
            avgVel += other.physics.vel;
            numNeighbors++;
        }
    }
    if (numNeighbors > 0) {
        avgVel /= numNeighbors;
        avgVel = normalize(avgVel) * b.speed;
        vec2 steer = avgVel - b.physics.vel;
        steer = normalize(steer) * b.maxForce;
        return steer;
    }
    return vec2(0.0f, 0.0f);
}

void BoidSwarm::render() {
    for (Boid& b : boids) {
        b.render();
    }
}