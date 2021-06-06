#include "WindowElement.h"

WindowElement::WindowElement(vec2 posIn, vec2 sizeIn, vec4 color) {
    color = color;

    destPos = posIn;
    size = sizeIn;
    pos = vec2(-size.x, 0.0f);
    startPos = pos;
    animationFrames = 10;
    animationStep = (destPos - pos) / animationLength;

    rect = Rectangle(pos, size, color, SHADER::SCREENSPACE);
}

void WindowElement::add_child(UIElement* child) {
    UIContainer::add_child(child);
    rect.size = size;
}

void WindowElement::start_animation() {
    animationFrames = 10;
    animating = true;
}

// @TODO, animation will need to account for changes in size/pos
void WindowElement::animate() {
    pos += animationStep;
    animationFrames--;
    rect.pos = pos;

    newChildRelPos.y = 0.0f;
    for (UIElement* child : children) {
        child->parent_bounds_change(pos + newChildRelPos, vec2(size.x, child->size.y));
        newChildRelPos.y += child->size.y;
    }

    if (animationFrames == 0) {
        animating = false;
        vec2 temp = startPos;
        startPos = destPos;
        destPos = temp;
        animationStep = (destPos - startPos) / animationLength;
    }
}

void WindowElement::render() {
    if (animationFrames > 0) animate();
    rect.render();
    for (UIElement* child : children) {
        child->render();
    }
}