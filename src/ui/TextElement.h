#pragma once
#include <string>
#include "UIElement.h"
#include "TextRenderer.h"

class TextElement : public UIElement {
public:
    TextElement(vec2 parentPos, vec2 parentSize, std::string s);
    void set_text(std::string s);
    std::string& get_text() { return str; }
    
    void parent_bounds_change(vec2 parentPos, vec2 parentSize) override;
    void handle_mouse() override;
    void update();
    void render();
    ~TextElement() { delete[] renderCoords; }
private:
    void deselect();
    std::string str;
    vec4 currentColor;
    vec4 activeColor;
    TexPoint* renderCoords = nullptr;
    int renderCoordsSize;
};