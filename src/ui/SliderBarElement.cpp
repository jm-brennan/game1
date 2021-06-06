#include "SliderBarElement.h"

SliderBarElement::SliderBarElement(vec2 parentPos, vec2 parentSize) {
    pos = parentPos;
    size = vec2(parentSize.x, 50.0f); // @TODO dont need full parentSize vec passed in
    horizontal = Rectangle(
        calc_horizontal_pos(), 
        calc_horizontal_size(), 
        vec4(0.2f, 0.9f, 0.5f, 1.0f), 
        SHADER::SCREENSPACE
    );
    
    vertical = Rectangle(
        vec2((pos.x + size.x) / 2.0f, pos.y),
        vec2(5.0f, size.y),
        vec4(1.0f, 1.0f, 1.0f, 1.0f),
        SHADER::SCREENSPACE
    );
}

vec2 SliderBarElement::calc_horizontal_pos() {
    return vec2(pos.x, pos.y + (size.y / 3.0f));
}

vec2 SliderBarElement::calc_horizontal_size() {
    return vec2(size.x, size.y / 3.0f);
}

vec2 SliderBarElement::calc_vertical_pos() {
    return vec2((pos.x + size.x) / 2.0f, pos.y);
}

vec2 SliderBarElement::calc_vertical_size() {
    return vec2(5.0f, size.y);
}


float SliderBarElement::get_fraction() {
    return (vertical.pos.x + (vertical.size.x / 2.0f)) / (pos.x + size.x);
}

void SliderBarElement::parent_bounds_change(vec2 parentPos, vec2 parentSize) {
    UIElement::parent_bounds_change(parentPos, parentSize);
    horizontal.pos = calc_horizontal_pos();
    horizontal.size = calc_horizontal_size();
    vertical.pos = calc_vertical_pos();
    vertical.size = calc_vertical_size();
}

void SliderBarElement::handle_mouse() {
    if (mouse_in_bounds() && im->get_mouse_down()) {
        vec2 horizontalBounds = horizontal.pos + horizontal.size;
        vec2 mouseDownAt = im->get_mouse_down_at();
        if (mouseDownAt.x >= horizontal.pos.x && mouseDownAt.x < horizontalBounds.x
         && mouseDownAt.y >= horizontal.pos.y && mouseDownAt.y < horizontalBounds.y) {
            float newXpos = im->get_mouse_at().x - (vertical.size.x / 2.0f);
            if (newXpos < horizontal.pos.x) {
                newXpos = horizontal.pos.x;
            } else if (newXpos > horizontal.pos.x + horizontal.size.x) {
                newXpos = horizontal.pos.x + horizontal.size.x;
            }
            vertical.pos = vec2(newXpos, vertical.pos.y);
            glfwSetCursor(window, grabCursor);
         } else {
             glfwSetCursor(window, nullptr);
         }
    } else {
        glfwSetCursor(window, nullptr);
    }
}

void SliderBarElement::render() {
    horizontal.render();
    vertical.render();
}