#include "CircleFilled.h"

CircleFilled::CircleFilled(vec2 pos, float radius, unsigned int numSides, SHADER s) 
    : pos(pos), radius(radius), numSides(numSides), shader(s) {
    init();
}

void CircleFilled::init() {
    float vertices[(numSides+1)*5];
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;
    vertices[3] = 1.0f;
    vertices[4] = 0.2f;

    unsigned int indices[numSides*3];

    const float TWO_PI = 6.2831852f;

    for (int i = 1; i < numSides + 1; i++) {
        vertices[(i*5)]   = cos(i * TWO_PI / numSides);
        vertices[(i*5)+1] = sin(i * TWO_PI / numSides);
        vertices[(i*5)+2] = 0.0f;
        vertices[(i*5)+3] = 1 / (float)i;
        vertices[(i*5)+4] = 0.2f;
    }
    
    for (int i = 0; i < numSides; i++) {
        indices[(i*3)]   = 0;
        indices[(i*3)+1] = i+1;
        indices[(i*3)+2] = (i+2);
    }
    indices[(numSides*3)-1] = 1;
    
    
    unsigned int VBO, EBO;
    GLEC(glGenVertexArrays(1, &VAO));
    GLEC(glGenBuffers(1, &VBO));
    GLEC(glGenBuffers(1, &EBO));
    GLResourceManager::add_vao(VAO);

    GLEC(glBindVertexArray(VAO));
    GLEC(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    
    GLEC(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    
    GLEC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLEC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
    
    GLEC(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GLEC(glEnableVertexAttribArray(0));
    GLEC(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
    GLEC(glEnableVertexAttribArray(1));
    
    GLEC(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
    GLEC(glBindVertexArray(0)); 
}

void CircleFilled::render() {
    ShaderManager::use(shader);
    ShaderManager::set_vec4(shader, "color", vec4(color.r, color.g, color.b, color.a));
    //GLEC(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    GLEC(glBindVertexArray(VAO));
    
    mat4 trans = mat4(1.0f);
    trans = translate(trans, vec3(pos.x, pos.y, 0.0f));
    mat4 scale = mat4(1.0f);
    scale = glm::scale(scale, vec3(radius, radius, 0.0f));
    mat4 model = trans * scale;
    ShaderManager::set_mat4(shader, "model", model);
    
    //GLEC(glDrawArrays(GL_TRIANGLES, 0, 6*3)); // this is for without indexes/ebo
    GLEC(glDrawElements(GL_TRIANGLES, numSides*3, GL_UNSIGNED_INT, 0));
    GLEC(glBindVertexArray(0)); 
    //GLEC(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}