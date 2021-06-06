#include "Rectangle.h"

Rectangle::Rectangle(vec2 pos, vec2 size, vec4 color, SHADER s) 
: pos(pos), size(size), color(color), shader(s) {
    float vertices[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
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
    GLEC(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    
    GLEC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLEC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
    
    GLEC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
    GLEC(glEnableVertexAttribArray(0));
    
    GLEC(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
    GLEC(glBindVertexArray(0)); 
}


void Rectangle::set_color(const vec4& c) {
    color = c;
    ShaderManager::set_vec4(shader, "color", color);
}

void Rectangle::render() {
    ShaderManager::use(shader);
    // @TODO having these double calls to set vec4 is ugly and i should do figure out a different paradigm
    ShaderManager::set_vec4(shader, "color", color);
    GLEC(glBindVertexArray(VAO));

    mat4 trans = mat4(1.0f);
    trans = glm::translate(trans, vec3(pos.x, pos.y, 0.0f));

    /* mat4 rotation = mat4(1.0f);
    rotation = glm::rotate(rotation, rot, vec3(0.0f, 0.0f, 1.0f)); */

    mat4 scale = mat4(1.0f);
    scale = glm::scale(scale, vec3(size, 0.0f));

    mat4 model = trans /* * rotation */ * scale; 

    ShaderManager::set_mat4(shader, "model", model);

    GLEC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    GLEC(glBindVertexArray(0));
}