#include "Player.h"

Player::Player(vec2 pos, float rot) {
    shader = SHADER::PLAYER;
    // @TODO need to figure out how these are gonna be organized
    forward = vec2(0.0f, 1.0f);
    left = vec2(-1.0f, 0.0f);
    vCursor = forward;
    length = 0.6f;
    width = 0.2f;
    scale = 1.0f;
    float rear = length * FRONT_BACK_LENGTH_RATIO;    

    float moment = (length*width*width*width)*scale + (width*length*length*length)*scale / 6.0f;
    moment += (rear*width*width*width)*scale + (width*rear*rear*rear)*scale / 6.0f;
    physics = PhysicsBody(pos, rot, 10.0f, moment, frictionCoeff);
    
    vertices = {
        vec2(0.0f, length) * scale,
        vec2(-1.0f*width, 0.0f) * scale,
        vec2(0.0f, -1.0f*rear) * scale,
        vec2(width, 0.0f) * scale
    };

    { // opengl to create player object shape
        float glVertices[] = {
            vertices[0].x, vertices[0].y,
            vertices[1].x, vertices[1].y,
            vertices[2].x, vertices[2].y,
            vertices[3].x, vertices[3].y,
        };

        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
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

    set_color(vec4(1.0f, 0.4f, 0.4f, 1.0f));
};

void Player::change_view(vec2 diff) {
    // @TODO probably want to change this to a more standard diff, virtual cursor within
    // a circle is a neat idea but the gameplay just doesnt feel very good with the sudden flip
    // when the vcursor is near the center
    float MAG = 1.0f;
    vec2 newVCursor = vCursor + diff;
    vec2 newVCursorU = normalize(newVCursor);
    // @TODO @cleanup can do better with efficiency
    if ((newVCursor.x * newVCursor.x) + (newVCursor.y * newVCursor.y) > MAG * MAG) {
        newVCursor = newVCursorU * MAG;
    }
    vCursor = newVCursor;
    look_at_vcursor();
}

void Player::look_at_vcursor() {
    forward = normalize(vCursor);
    left.x = -forward.y;
    left.y = forward.x;

    vertices[0] = forward * length * scale;
    vertices[1] = left * width * scale;
    vertices[2] = -1.0f * FRONT_BACK_LENGTH_RATIO * forward * length * scale;
    vertices[3] = -1.0f * left * width * scale;

    float angleCorrector = 0.0f;
    if (forward.y < 0) angleCorrector = PI;
    physics.rotate_to(angleCorrector - atan(forward.x / forward.y));
}


void Player::render() {
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

    GLEC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    GLEC(glBindVertexArray(0));
}


void Player::set_color(const vec4& c) {
    ShaderManager::set_vec4(shader, "color", c);
}
