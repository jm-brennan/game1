#pragma once

#include "UIElement.h"
#include "utils/Rectangle.h"

class SliderBarElement : public UIElement {
public:
    SliderBarElement(vec2 parentPos, vec2 parentSize);
    float get_fraction();

    void parent_bounds_change(vec2 parentPos, vec2 parentSize) override;
    void handle_mouse();
    void update() {}
    void render();
    
private:
    vec2 calc_horizontal_pos();
    vec2 calc_horizontal_size();
    vec2 calc_vertical_pos();
    vec2 calc_vertical_size();
    Rectangle horizontal;
    Rectangle vertical;
};