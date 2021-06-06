#pragma once
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/paths.h"
#include "definitions.h"

using namespace glm;

enum SHADER {
    PLAYER,
    LINE,
    BLOCK,
    GRID,
    TEXT,
    SCREENSPACE,
};

class ShaderManager {
public:
    static std::map<SHADER, unsigned int> shaders;
    static void create_shader(std::string vShaderFile, std::string fShaderFile, SHADER s);
    static void use(SHADER s);

    static void set_float(SHADER s, const char* name, float value);
    static void set_vec4 (SHADER s, const char* name, const vec4& vec);
    static void set_mat4 (SHADER s, const char* name, const mat4& matrix);

    static void delete_shaders();
};
