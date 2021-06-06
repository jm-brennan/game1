#pragma once

#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "TextRenderer.h"
#include "InputManager.h"
using namespace glm;

class UIElement {
public:
    static void init(GLFWwindow* w);
    static void shutdown();

    virtual void handle_mouse() = 0;
    virtual void render() = 0;
    virtual void parent_bounds_change(vec2 parentPos, vec2 parentSize);
    virtual void update() = 0;

    bool mouse_in_bounds();

    virtual ~UIElement() {}

    vec2 size;
    vec2 pos;
    vec4 color;
protected:
    static GLFWwindow* window;
    static GLFWcursor* grabCursor;
    static TextRenderer* textRenderer;
    static InputManager* im;
};