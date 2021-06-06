#include "TextRenderer.h"

TextRenderer::TextRenderer() {
    shader = SHADER::TEXT;

    FT_Library library;
    if (FT_Init_FreeType(&library)) {
        printf("ERROR::FreeType init\n");
        return;
    }

    FT_Face face;
    // @TODO better mechanism for different fonts, probably will want to have a bunch of 
    // different instances of this class for different fonts and sizes, but for now we hardcode!
    std::string filepath = std::string(FONT_DIR) + std::string("OpenSans-Regular.ttf");
    if (FT_New_Face(library, filepath.c_str(), 0, &face)) {
        printf("ERROR::FreeType load font\n");
        return;
    }

    if (FT_Set_Pixel_Sizes(face, 0, 32)) {
        printf("ERROR::FreeType pixel size\n");
    }

    // calculate width and height of single row atlas, store width for use during rendering, but keep
    // uints around for gl texture setup calls
    unsigned int atlasWidth  = 0;
    unsigned int atlasHeight = 0;
    for (unsigned char c = 32; c < 128; c++) {
         if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            printf("ERROR::FreeType load char [%c]\n", c);
            continue;
        }

        atlasWidth += face->glyph->bitmap.width + 1;
        atlasHeight = (face->glyph->bitmap.rows > atlasHeight) ? face->glyph->bitmap.rows : atlasHeight;
    }
    atlasSize = vec2(atlasWidth, atlasHeight);

    GLEC(glActiveTexture(GL_TEXTURE0));
    GLEC(glGenTextures(1, &atlasTexture));
    GLEC(glBindTexture(GL_TEXTURE_2D, atlasTexture));
    GLEC(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction since only using 1 byte to store bitmap
    GLEC(glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RED, 
        atlasWidth, 
        atlasHeight, 
        0, 
        GL_RED, 
        GL_UNSIGNED_BYTE, 
        nullptr
    ));
    GLResourceManager::add_texture(atlasTexture);
    // clamp to edge to prevent artifacts during scale
    GLEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    // linear filtering is good for text
    GLEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    unsigned int glyphStart = 0;
    for (unsigned char c = 32; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
        GLEC(glTexSubImage2D(
            GL_TEXTURE_2D,
            0, 
            glyphStart,
            0, 
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        ));

        Glyph g = {
            vec2((float)glyphStart / atlasSize.x, 0.0f),
            vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            vec2(face->glyph->advance.x >> 6, 0.0f),
            vec2(face->glyph->bitmap_left, face->glyph->bitmap_top)
        };
        glyphs[c] = g;

        glyphStart += face->glyph->bitmap.width + 1;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    GLEC(glGenVertexArrays(1, &VAO));
    GLEC(glGenBuffers(1, &VBO));
    GLEC(glBindTexture(GL_TEXTURE_2D, 0));
    GLResourceManager::add_vao(VAO);  
}

void TextRenderer::set_color(const vec4& c) {
    ShaderManager::set_vec4(shader, "color", c);
}

vec2 TextRenderer::get_size(const std::string& s) {
    float xSize = 0.0f;
    float yMin  = 0.0f; // positive units below writing line
    float yMax  = 0.0f; // positive units above writing line

    for (auto c = s.begin(); c != s.end(); c++) {
        const Glyph glyph = glyphs[*c];
        if (glyph.size.y - glyph.bearing.y > yMin) yMin = glyph.size.y - glyph.bearing.y;
        if (glyph.bearing.y > yMax) yMax = glyph.bearing.y;

        xSize += glyph.advance.x;
    }
    return vec2(xSize, yMax + yMin);
}

void TextRenderer::calculate_render_coords_for_string(const std::string& s, vec2 pos, TexPoint* coords) {
    for (const char& c : s) {
        Glyph glyph = glyphs[c];
        float x = pos.x + glyph.bearing.x;
        float y = pos.y + (atlasSize.y - glyph.size.y) + (glyph.size.y - glyph.bearing.y);
        float w = glyph.size.x;
        float h = glyph.size.y;
        
        *coords++ = (TexPoint){x,     y,      glyph.texCoord.x,                     glyph.texCoord.y};
        *coords++ = (TexPoint){x,     y+h,    glyph.texCoord.x,                     glyph.texCoord.y + (h / atlasSize.y)};
        *coords++ = (TexPoint){x + w, y+h,    glyph.texCoord.x + (w / atlasSize.x), glyph.texCoord.y + (h / atlasSize.y)};
        *coords++ = (TexPoint){x,     y,      glyph.texCoord.x,                     glyph.texCoord.y};
        *coords++ = (TexPoint){x + w, y+h,    glyph.texCoord.x + (w / atlasSize.x), glyph.texCoord.y + (h / atlasSize.y)};
        *coords++ = (TexPoint){x + w, y,      glyph.texCoord.x + (w / atlasSize.x), glyph.texCoord.y};
        pos += glyph.advance;
    }
}

void TextRenderer::render_text(TexPoint* coords, int coordsSize) {
    ShaderManager::use(shader);
    GLEC(glBindTexture(GL_TEXTURE_2D, atlasTexture));
	GLEC(glBindVertexArray(VAO));
	GLEC(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLEC(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));

    GLEC(glEnableVertexAttribArray(0));
    GLEC(glBufferData(GL_ARRAY_BUFFER, coordsSize*4*4, (float*)coords, GL_DYNAMIC_DRAW));
    GLEC(glDrawArrays(GL_TRIANGLES, 0, coordsSize));
    
    GLEC(glBindVertexArray(0));
    GLEC(glBindTexture(GL_TEXTURE_2D, 0));
}
