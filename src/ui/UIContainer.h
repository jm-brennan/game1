#pragma once

#include "UIElement.h"

class UIContainer : public UIElement{
public:
    virtual void add_child(UIElement* child);
    virtual void render();

    void parent_bounds_change(vec2 parentPos, vec2 parentSize) override;
    void handle_mouse();
    void update();

    ~UIContainer() {
        for (UIElement* child : children) {
            delete child;
        }
    }
    
    vec2 newChildRelPos = vec2(0.0f); // contains bottom left corner of vertical stack of children
    std::vector<UIElement*> children;
};