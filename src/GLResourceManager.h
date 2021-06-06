#pragma once

#include <vector>
#include <glad/glad.h>
#include "definitions.h"

class GLResourceManager {
public:
    static void add_texture(unsigned int texture) {
        textures.push_back(texture);
    }

    static void add_vao(unsigned int vao) {
        vaos.push_back(vao);
    }

    static void delete_textures() {
        for (unsigned int texture : textures) {
            GLEC(glDeleteTextures(1, &texture));
        }
    }

    static void delete_vaos() {
        for (unsigned int vao : vaos) {
            GLEC(glDeleteVertexArrays(1, &vao));
        }
    }
private:
    static std::vector<unsigned int> textures;
    static std::vector<unsigned int> vaos;
};