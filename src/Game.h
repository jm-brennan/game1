#pragma once
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Client.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "Player.h"
#include "Camera.h"
#include "Block.h"
#include "Grid.h"
#include "BoidSwarm.h"
#include "ui/WindowElement.h"
#include "ui/TextElement.h"
#include "ui/ValueSliderElement.h"
#include "ui/SpacerElement.h"

#include "DebugOptions.h"
#include "definitions.h"

#include "collisions.h"

using namespace glm;

class Game {
public:
    Game(unsigned int width, unsigned int height, float visibleMin);

    void init();
    void process_input(float dt);
    void update(float dt);
    void refresh_p2_data();
    void render();

    void set_window(GLFWwindow* window) { this->window = window; }
    void set_window_size(unsigned int width, unsigned int height);

    ~Game();

private:
    vec2 cursor_to_world(vec2 cursorPos);
    void load_resources();
    void configure_shaders();
    void resolve_collisions();
    void check_collisions(float dt);
    void zoom(float amount);
    
    GLFWwindow* window;
    Client client;
    Camera cam;
    InputManager* im = InputManager::get_instance();
    WindowElement* gui;
    // @TODO make players not have to be pointers
    Player* player;
    Player* player2;
    
    DebugOptions DO;
    Grid worldGrid;
    
    std::vector<Block*> blocks;
    BoidSwarm* boids = nullptr;

    vec2 prevCursor;

    float visibleMin, mouseRad;
    unsigned int width, height;

    bool mouseControlsPlayer = true;
};