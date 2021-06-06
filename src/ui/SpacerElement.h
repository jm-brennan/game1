#pragma once

#include "UIElement.h"

class SpacerElement : public UIElement {
public:
    SpacerElement(vec2 pos, vec2 size) {
        this->pos = pos;
        this->size = size;
    }

    void handle_mouse() {}
    void update() {}
    void render() {}
};