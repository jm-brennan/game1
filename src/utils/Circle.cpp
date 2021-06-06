#include "Circle.h"

Circle::Circle(vec2 pos, float radius, unsigned int numSides, bool wire) {
    this->numSides = numSides;
    this->pos = pos;
    this->wire = wire;
    this->radius = radius;

    shader = SHADER::LINE;
    ShaderManager::set_float(shader, "linewidth", 0.05f);

    float vertices[numSides*2*2*2]; // xy, unit, xy, -unit

    unsigned int indices[numSides*2*3];

    const float TWO_PI = 6.2831852f;

    for (int i = 0; i < numSides; i++) {
        float x = cos(i * TWO_PI / numSides);
        float y = sin(i * TWO_PI / numSides);
        vec2 unit = normalize(vec2(x, y));
        vertices[(i*8)]   = x;
        vertices[(i*8)+1] = y;
        vertices[(i*8)+2] = unit.x;
        vertices[(i*8)+3] = unit.y;
        vertices[(i*8)+4] = x; // should be able to get rid of these if i have two ebos?
        vertices[(i*8)+5] = y;
        vertices[(i*8)+6] = -1*unit.x;
        vertices[(i*8)+7] = -1*unit.y;
    }
    
    for (int i = 0; i < numSides*2; i++) {
        indices[(i*3)]   = i;
        indices[(i*3)+1] = i+1;
        indices[(i*3)+2] = i+2;
    }
    indices[((numSides-1)*2*3)+2] = 0;
    indices[((((numSides-1)*2)+1)*3)+1] = 0;
    indices[((((numSides-1)*2)+1)*3)+2] = 1;

    
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

    GLEC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
    GLEC(glEnableVertexAttribArray(0));
    GLEC(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
    GLEC(glEnableVertexAttribArray(1));

    GLEC(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
    GLEC(glBindVertexArray(0)); 

}
    
void Circle::move(vec2 newPos) {
    pos = newPos;
}

void Circle::render() {
    ShaderManager::use(shader);
    if (wire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GLEC(glBindVertexArray(VAO));
    
    mat4 model = mat4(1.0f);
    model = translate(model, vec3(pos.x, pos.y, 0.0f));
    model = scale(model, vec3(radius, radius, 0.0));
    ShaderManager::set_mat4(shader, "model", model);
    
    GLEC(glDrawElements(GL_TRIANGLES, numSides*2*3, GL_UNSIGNED_INT, 0)); // humSides*2*3 is the number of indices, so 3 times number of triangles
    GLEC(glBindVertexArray(0));
    if (wire) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}