#include "TextElement.h"

TextElement::TextElement(vec2 parentPos, vec2 parentSize, std::string s) 
  : str(s) {
    color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
    activeColor = vec4(0.9f, 0.8f, 0.2f, 1.0f);
    currentColor = color;
    parent_bounds_change(parentPos, parentSize);
    size = textRenderer->get_size(str);
    renderCoordsSize = 6 * str.size(); // two triangles for each glyph
    if (!renderCoords) renderCoords = new TexPoint[renderCoordsSize];
    textRenderer->calculate_render_coords_for_string(s, pos, renderCoords);
}

void TextElement::parent_bounds_change(vec2 parentPos, vec2 parentSize) {
    pos = parentPos + vec2((parentSize.x - size.x) / 2.0f, 0.0f);
    pos = floor(pos); // want text to be integer aligned to look its best
    if (!renderCoords) {
        renderCoordsSize = 6 * str.size();
        renderCoords = new TexPoint[renderCoordsSize];
    }
    textRenderer->calculate_render_coords_for_string(str, pos, renderCoords);
}

void TextElement::set_text(std::string s) {
    if (s.size() != str.size()) {
        renderCoordsSize = 6 * s.size();
        delete[] renderCoords;
        printf("getting new TexPoint array of size %d\n", renderCoordsSize);
        renderCoords = new TexPoint[renderCoordsSize];
    }
    str = s;
    size = textRenderer->get_size(str);
    textRenderer->calculate_render_coords_for_string(str, pos, renderCoords);
}

void TextElement::handle_mouse() {
    if (mouse_in_bounds()) {
        glfwSetCursor(window, grabCursor);
        currentColor = activeColor;
        if (im->get_mouse_newly_down()) printf("clicked element that says: %s\n", str.c_str());;
    } else {
        glfwSetCursor(window, nullptr);
    }
}

void TextElement::update() {
    if (!mouse_in_bounds()) {
        deselect();
    }
}

void TextElement::deselect() {
    currentColor = color;
}

void TextElement::render() {
    textRenderer->set_color(currentColor);
    textRenderer->render_text(renderCoords, renderCoordsSize);
}