#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager* InputManager::get_instance() {
    if (!instance) instance = new InputManager();
    return instance;
}


//////////////////////////////////////////////////////////////
//                          mouse                           //
//////////////////////////////////////////////////////////////

void InputManager::update_mouse_pos() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    mouseAt = vec2(xpos, ypos);
}

void InputManager::set_mouse_status(bool isDown) {
    if (isDown) {
        mouseNewlyDown = true;
        mouseDownAt = mouseAt;
    } else {
        mouseDown = false;
        mouseNewlyDown = false;
    }
}

bool InputManager::get_mouse_down() {
    return mouseDown || mouseNewlyDown;
}

bool InputManager::get_mouse_newly_down() {
    return mouseNewlyDown;
}


//////////////////////////////////////////////////////////////
//                          keys                            //
//////////////////////////////////////////////////////////////

// @TODO this two array implementatino is quite naive, is there a better way?
void InputManager::set_key_status(int key, bool isDown) {
    if (isDown) {
        keyNewlyDown[key] = true;
    } else {
        // have to set both to false in case there was a down and up in between the call to
        // update_new_keys at the end of the game loop and the poll call at 
        // the beginning of the game loop. That would be super fast keypressing, but probably possible
        keyDown[key] = false;
        keyNewlyDown[key] = false;
    }
}

bool InputManager::get_key_down(int key) {
    return keyDown[key] || keyNewlyDown[key];
}

bool InputManager::get_key_newly_down(int key) {
    return keyNewlyDown[key];
}


//////////////////////////////////////////////////////////////
//                     mouse and keys                       //
//////////////////////////////////////////////////////////////

void InputManager::update_new_inputs() {
    if (mouseNewlyDown) {
        mouseDown = true;
        mouseNewlyDown = false;
    }
    // @TODO this is probably the most expensive part of the naive system,
    // have to loop through 300+ keys to find just one or two keys that are down
    // every loop. a better way might involve a small array that only contains
    // the newly down keys??
    for (int i = 0; i < GLFW_KEY_LAST; i++) {
        if (keyNewlyDown[i]) {
            keyDown[i] = true;
            keyNewlyDown[i] = false;
        }
    }
}

void InputManager::reset_inputs() {
    mouseDown = false;
    mouseNewlyDown = false;

    for (int i = 0; i < GLFW_KEY_LAST; i++) {
        keyDown[i] = false;
        keyNewlyDown[i] = false;
    }
}