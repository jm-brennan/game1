#pragma once

#include "utils/Rectangle.h"
#include "UIContainer.h"

class WindowElement : public UIContainer {
public:
    WindowElement(vec2 posIn, vec2 sizeIn, vec4 color);
    void start_animation();

    void add_child(UIElement* child) override;
    void render() override;

    bool animating = false;
private:
    void animate();
    Rectangle rect;
    
    // animation stuff
    vec2 startPos;
    vec2 destPos;
    vec2 animationStep;
    const float animationLength = 10.0f;
    int animationFrames;
};