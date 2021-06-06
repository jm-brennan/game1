#include "ShaderManager.h"

std::map<SHADER, unsigned int> ShaderManager::shaders;

int compile_shader(std::string shaderFilename, int shaderType);
std::string load_file(std::string filename);

void ShaderManager::create_shader(std::string vShaderFile, std::string fShaderFile, SHADER s) {
    if (shaders.find(s) != shaders.end()) {
        printf("Warning::Redefinition of shader enum %d\n", s);
    }
    unsigned int id = glCreateProgram(); // @TODO change GLEC to be able get the value out?
    unsigned int vid = compile_shader(vShaderFile, GL_VERTEX_SHADER);
    GLEC(glAttachShader(id, vid));
    
    unsigned int fid = compile_shader(fShaderFile, GL_FRAGMENT_SHADER);
    GLEC(glAttachShader(id, fid));

    int success;
    GLEC(glLinkProgram(id));
    GLEC(glGetProgramiv(id, GL_LINK_STATUS, &success));
    if(!success) {
        char error[512];
        GLEC(glGetProgramInfoLog(id, 512, nullptr, error));
        printf("ERROR::Shader link:\n%s\n", error);
    }
    GLEC(glDeleteShader(vid));
    GLEC(glDeleteShader(fid));
    shaders[s] = id;
}

void ShaderManager::use(SHADER s) {
    unsigned int id = shaders[s];
    GLEC(glUseProgram(id));
}

void ShaderManager::delete_shaders() {
    for (auto iter : shaders) {
        GLEC(glDeleteProgram(iter.second));
    }
}

void ShaderManager::set_float(SHADER s, const char* name, float value) {
    unsigned int id = shaders[s];
    GLEC(glUseProgram(id));
    GLEC(glUniform1f(glGetUniformLocation(id, name), value));
}

void ShaderManager::set_vec4(SHADER s, const char* name, const vec4& vec) {
    unsigned int id = shaders[s];
    GLEC(glUseProgram(id));
    GLEC(glUniform4fv(glGetUniformLocation(id, name), 1, value_ptr(vec)));
}

void ShaderManager::set_mat4(SHADER s, const char* name, const mat4& matrix) {
    unsigned int id = shaders[s];
    GLEC(glUseProgram(id));
    GLEC(glUniformMatrix4fv(glGetUniformLocation(id, name), 1, false, value_ptr(matrix)));
}


int compile_shader(std::string shaderFilename, int shaderType) {
    std::string shaderSource = load_file(shaderFilename);
    unsigned int id = glCreateShader(shaderType);
    const char* cShaderSource = shaderSource.c_str();
    GLEC(glShaderSource(id, 1, &cShaderSource, nullptr));
    GLEC(glCompileShader(id));
    int success;
    GLEC(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
    if (!success) {
        char error[512];
        GLEC(glGetShaderInfoLog(id, 512, nullptr, error));
        printf("ERROR: Shader compilation failed for file %s: %s\n", shaderFilename.c_str(), error);
    }
    return id;
}

std::string load_file(std::string filename) {
    std::ifstream shaderFile;
    std::string shaderCode;
    std::string filepath = std::string(SHADER_DIR) + filename;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        shaderFile.open(filepath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    } catch (std::ifstream::failure e) {
        printf("ERROR::Shader file %s\n", filepath.c_str());
    }
    return shaderCode;
}



