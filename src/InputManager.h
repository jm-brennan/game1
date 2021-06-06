#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

// Singleton input manager
class InputManager {
public:
    static InputManager* get_instance();
    void set_window(GLFWwindow* window) { this->window = window; }

    void update_mouse_pos();
    void set_mouse_status(bool isDown);
    bool get_mouse_down();
    bool get_mouse_newly_down();
    vec2 get_mouse_at() { return mouseAt; }
    vec2 get_mouse_down_at() { return mouseDownAt; }

    void set_key_status(int key, bool isDown);
    bool get_key_down(int key);
    bool get_key_newly_down(int key);

    void update_new_inputs();
    void reset_inputs();

private:
    InputManager() {}
    static InputManager* instance;
    GLFWwindow* window;
    
    bool mouseDown; // so far we only care about the left mouse button
    bool mouseNewlyDown;
    vec2 mouseAt;
    vec2 mouseDownAt;
    
    bool keyDown[GLFW_KEY_LAST];
    bool keyNewlyDown[GLFW_KEY_LAST];


};