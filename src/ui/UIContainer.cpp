#include "UIContainer.h"

void UIContainer::parent_bounds_change(vec2 parentPos, vec2 parentSize) {
    UIElement::parent_bounds_change(parentPos, parentSize);
    newChildRelPos.y = 0.0f;
    for (UIElement* child : children) {
        // @TODO this sorta doesnt allow the child to grow vertically?
        child->parent_bounds_change(pos + newChildRelPos, vec2(size.x, child->size.y));
        newChildRelPos.y += child->size.y;
    }
}

void UIContainer::add_child(UIElement* child) {
    // @TODO, @BUG animation will need to account for changes in size/pos

    // right now everything is always expanding to fit children exactly (or can already be
    // larger than children), but will be wanting more complex behavior eventually
    children.push_back(child);
    bool sizeChange = false;
    if (child->size.x > size.x) {
        size.x = child->size.x;
        sizeChange = true;
    }
    newChildRelPos.y += child->size.y;

    if (newChildRelPos.y > size.y) {
        size.y = newChildRelPos.y;
        sizeChange = true;
    }

    if (sizeChange) {
        newChildRelPos.y = 0.0f;
        for (UIElement* child : children) {
            // @TODO this sorta doesnt allow the child to grow vertically?
            child->parent_bounds_change(pos + newChildRelPos, vec2(size.x, child->size.y));
            newChildRelPos.y += child->size.y;
        }
    }
}

void UIContainer::handle_mouse() {
    if (mouse_in_bounds()) {
        // pass mouse coordinates to relevant child
        UIElement* activeChild = nullptr;
        for (UIElement* child : children) {
            if (child->mouse_in_bounds()) {
                activeChild = child;
                child->handle_mouse();
            }
        }

        // if mouse is not within bounds of any child, set cursor to default
        if (activeChild == nullptr) {
            glfwSetCursor(window, nullptr);
        }
    } else {
        glfwSetCursor(window, nullptr);
    }
}

void UIContainer::update() {
    for (UIElement* child : children) {
        child->update();
    }
}

void UIContainer::render() {
    for (UIElement* child : children) {
        child->render();
    }
}