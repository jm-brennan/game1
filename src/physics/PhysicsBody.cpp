#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(vec2 pos, float rot, float mass, float moment, float frictionCoeff) 
    : pos(pos), rot(rot), mass(mass),  moment(moment), frictionCoeff(frictionCoeff) {
        vel = vec2(0.0f, 0.0f);
        forces = vec2(0.0f, 0.0f);
        forcesAt = vec2(0.0f, 0.0f);
    }

void PhysicsBody::add_force(vec2 f, vec2 at) {
    forces += f;
    forcesAt += at;
}

void PhysicsBody::rotate_to(float r) {
    rot = r;
}

void PhysicsBody::update(float dt) {
    //printf("force: (%f, %f)\n", forces.x, forces.y);
    apply_friction();
    //vec2 r = pos - forcesAt; // moment arm
    vel = vel + (forces / mass) * dt;
    pos = pos + vel * dt;

    //float torque = r.x*forces.y - r.y*forces.x;
    //float angularAcceleration = torque / moment;
    //angVel += angularAcceleration * dt;
    //rot += angVel * dt;
    forces = vec2(0.0f, 0.0f);
    forcesAt = vec2(0.0f, 0.0f);
}

void PhysicsBody::apply_friction() {
    vec2 friction = -1 * frictionCoeff * vel;
    //@incomplete need to remove angular velocity in new force paradigm
    //angVel *= 0.1f * dt;
    //if (angVel < 0.0f) angVel = 0.0f;
    add_force(friction);
}
