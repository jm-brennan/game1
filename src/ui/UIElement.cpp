#include "UIElement.h"
GLFWcursor* UIElement::grabCursor = nullptr;
GLFWwindow* UIElement::window = nullptr;
TextRenderer* UIElement::textRenderer = nullptr;
InputManager* UIElement::im = InputManager::get_instance();


void UIElement::init(GLFWwindow* w) {
    window = w;
    textRenderer = new TextRenderer();
    grabCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
}

void UIElement::parent_bounds_change(vec2 parentPos, vec2 parentSize) {
    pos = parentPos;
    size = parentSize;
}

bool UIElement::mouse_in_bounds() {
    vec2 bounds = pos + size;
    if (im->get_mouse_down()) {
        vec2 mouseDownAt = im->get_mouse_down_at();
        if (mouseDownAt.x > pos.x && mouseDownAt.x <= bounds.x
        && mouseDownAt.y > pos.y && mouseDownAt.y <= bounds.y) {
            return true;
        } else {
            return false;
        }
    } else {
        vec2 mouseAt = im->get_mouse_at();
        if (mouseAt.x > pos.x && mouseAt.x <= bounds.x
        && mouseAt.y > pos.y && mouseAt.y <= bounds.y) {
            return true;
        } else {
            return false;
        }
    }

}

void UIElement::shutdown() {
    delete textRenderer;
    window = nullptr;
    grabCursor = nullptr;
    im = nullptr;

}