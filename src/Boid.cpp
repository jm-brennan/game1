#include "Boid.h"

Boid::Boid(vec2 pos, float rot) {
    shader = SHADER::PLAYER;
    float moment = (0.6f*0.2f*0.2f*0.2f) + (0.2f*0.6f*0.6f*0.6f) / 6.0f;
    physics = PhysicsBody(pos, rot, 10.0f, moment, frictionCoeff);
    // the normalized velocity is used as the boids forward vector, so have to init it
    // to the rotation, just with a very small multiplier to have it not affect things
    physics.vel = normalize(vec2(cos(rot+PI_HALF), sin(rot+PI_HALF))) * 0.0001f;
    vertices = {
        vec2(0.0f, 0.6f),
        vec2(-0.2f, 0.0f),
        vec2(0.2f, 0.0f)
    };

    //circle = new CircleFilled(pos, 0.1f, 16);

    { // opengl to create player object shape
        float glVertices[] = {
            vertices[0].x, vertices[0].y,
            vertices[1].x, vertices[1].y,
            vertices[2].x, vertices[2].y,
        };

        unsigned int indices[] = {
            0, 1, 2,
        };

        unsigned int VBO, EBO;
        GLEC(glGenVertexArrays(1, &VAO));
        GLEC(glGenBuffers(1, &EBO));
        GLEC(glGenBuffers(1, &VBO));
        GLResourceManager::add_vao(VAO);

        GLEC(glBindVertexArray(VAO));
        GLEC(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GLEC(glBufferData(GL_ARRAY_BUFFER, sizeof(glVertices), glVertices, GL_STATIC_DRAW));

        GLEC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
        GLEC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

        GLEC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
        GLEC(glEnableVertexAttribArray(0));

        GLEC(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
        GLEC(glBindVertexArray(0)); 
    }
}

void Boid::add_destination(vec2 loc) {
    destination += loc;
}

void Boid::render() {
    ShaderManager::use(shader);
    GLEC(glBindVertexArray(VAO));

    mat4 trans = mat4(1.0f);
    trans = translate(trans, vec3(physics.pos, 0.0f));

    mat4 rotation = mat4(1.0f);
    rotation = glm::rotate(rotation, physics.rot, vec3(0.0f, 0.0f, 1.0f));

    mat4 scale = mat4(1.0f);
    scale = glm::scale(scale, vec3(1.0f, 1.0f, 0.0f));

    mat4 model = trans * rotation * scale; 

    ShaderManager::set_mat4(shader, "model", model);

    GLEC(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
    GLEC(glBindVertexArray(0));
   
}