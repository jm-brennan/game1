#include "Grid.h"

Grid::Grid() {}

void Grid::init() {
    // @TODO this shader was made with the intention of having the alpha
    // of the lines depend on its position relative to like whole numbers or something like
    // that but is not being used currently
    shader = SHADER::GRID;
    
    // @TODO this could be done with indexing to save on repeat values
    float vertices[((size*2)+1)*2*4];
    int index = 0;
    for (int i = -size; i <= size; i++) {
        float leftx = (float)-size;
        vertices[index++] = leftx;
        float lefty = i;
        vertices[index++] = lefty;
        float rightx = (float)size;
        vertices[index++] = rightx;
        float righty = i;
        vertices[index++] = righty;
    }

    for (int i = -size; i <= size; i++) {
        float topx = i;
        vertices[index++] = topx;
        float topy = (float)size;
        vertices[index++] = topy;
        float bottomx = i;
        vertices[index++] = bottomx;
        float bottomy = (float)-size;
        vertices[index++] = bottomy;
    }

    unsigned int VBO;
    GLEC(glGenVertexArrays(1, &VAO));
    GLEC(glGenBuffers(1, &VBO));
    GLResourceManager::add_vao(VAO);

    GLEC(glBindVertexArray(VAO));
    GLEC(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLEC(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GLEC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
    GLEC(glEnableVertexAttribArray(0));

    GLEC(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLEC(glBindVertexArray(0));
}

void Grid::render() {
    ShaderManager::use(shader);
    GLEC(glBindVertexArray(VAO));

    ShaderManager::set_mat4(shader, "model", mat4(1.0f));
    GLEC(glDrawArrays(GL_LINES, 0, ((size*2)+1)*2*4));
}
