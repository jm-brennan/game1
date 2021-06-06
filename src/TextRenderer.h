#pragma once

#include <unordered_map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H 

#include "ShaderManager.h"
#include "GLResourceManager.h"
#include "definitions.h"

#include "utils/paths.h"
#include "utils/Rectangle.h"
#include "utils/CircleFilled.h"

using namespace glm;

struct Glyph {
    vec2 texCoord; // 0 to 1 start of the glyph texture in the atlas
    vec2 size;
    vec2 advance;
    vec2 bearing; // offset from baseline to left/top of glyph
};

// @TODO there should probably be a texture class that stores and handles this,
// but right now text is the only thing using textures

// TexPoint just to help wrangle the floats around. each glyph needs 
// 6 points (2 triangles, not trying to use indexing to only need 4), 
// each point contains xy coord and st texture coord 
struct TexPoint { float x; float y; float s; float t; };

class TextRenderer {
public:
    TextRenderer();
    int init();
    vec2 get_size(const std::string& s);
    void set_color(const vec4& c);
    void calculate_render_coords_for_string(const std::string& s, vec2 pos, TexPoint* renderCoords);
    void render_text(TexPoint* coords, int coordsSize);
private:
    SHADER shader;
    Glyph glyphs[128]; // first 32 will be empty, store ascii characters [32, 128)
    unsigned int atlasTexture;
    vec2 atlasSize;
    unsigned int VAO, VBO;
};