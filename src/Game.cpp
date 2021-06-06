#include "Game.h"

std::pair<bool, vec2> line_line_intersection(float l1x1, float l1y1, float l1x2, 
        float l1y2, float l2x1, float l2y1, float l2x2, float l2y2);

Game::Game(unsigned int width, unsigned int height, float visibleMin) 
    : width(width), height(height), visibleMin(visibleMin),
    cam(Camera(width, height, vec3(0,0,1), vec3(0,0,0), vec3(0,1,0), visibleMin)) {}

void Game::init() {
    load_resources();
    configure_shaders();
    zoom(4.1f);
    im->reset_inputs();
    worldGrid.init();

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    prevCursor = cursor_to_world(im->get_mouse_at());

    player = new Player(vec2(1.0f, 2.0f), 0.0f);
    player2 = new Player(vec2(3.0f, 2.0f), 0.0f);

    blocks.push_back(new Block(vec2(4.0f, -2.0f), vec2(4.0f, 1.0f), 0.0f, vec4(0.5f, 0.7f, 0.2f, 1.0f)));
    blocks.push_back(new Block(vec2(8.0f, -2.0f), vec2(1.0f, 3.0f), 0.0f, vec4(0.5f, 0.7f, 0.2f, 1.0f)));

    boids = new BoidSwarm(100);
    client.init();

    UIElement::init(window);
    gui = new WindowElement(vec2(0.0f), vec2(400.0f, 33.0f), vec4(0.162f, 0.162f, 0.162f, 0.8f));
    gui->add_child(new SpacerElement(gui->pos + gui->newChildRelPos, vec2(0.0f, 5.0f)));
    gui->add_child(new TextElement(gui->pos + gui->newChildRelPos, gui->size, "Boid Swarm Settings"));
    gui->add_child(new SpacerElement(gui->pos + gui->newChildRelPos, vec2(0.0f, 20.0f)));
    gui->add_child(new ValueSliderElement(gui->pos + gui->newChildRelPos, gui->size, "Separation Radius", &boids->separationRadius));
    gui->add_child(new SpacerElement(gui->pos + gui->newChildRelPos, vec2(0.0f, 20.0f)));
    gui->add_child(new ValueSliderElement(gui->pos + gui->newChildRelPos, gui->size, "Neighbor Radius", &boids->neighborRadius));
}

void Game::process_input(float dt) {
    if (im->get_key_newly_down(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, true);

    float currentTime = glfwGetTime();
    { // display state
        if (im->get_key_down(GLFW_KEY_LEFT_CONTROL)) {
            { // change zoom
                float zoomFactor = -1.0f;
                if (im->get_key_newly_down(GLFW_KEY_MINUS)) {
                    zoomFactor = 2.0f;
                } else if (im->get_key_newly_down(GLFW_KEY_EQUAL)) {
                    zoomFactor = 0.5f;
                }
                if (zoomFactor > 0.0f) {
                    zoom(zoomFactor);
                }
            }

            { // toggle showing gui
                if (im->get_key_newly_down(GLFW_KEY_G)) {
                    DO.gui = !DO.gui;
                    gui->start_animation();
                }
            }
        }
    }
    
    { // mouse input 
        // Toggle mouse control for switching between interacting with ui and controlling player
        if (im->get_key_newly_down(GLFW_KEY_M)) {
            mouseControlsPlayer = !mouseControlsPlayer;
            if (mouseControlsPlayer) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

        //getting cursor position for rotation of player
        vec2 currentCursor = cursor_to_world(im->get_mouse_at());
        vec2 cursorDiff = currentCursor - prevCursor;
        prevCursor = currentCursor;
        if (mouseControlsPlayer) player->change_view(cursorDiff);
        else gui->handle_mouse();

        // @TODO @DEBUG
        if (im->get_key_newly_down(GLFW_KEY_C)) {
            vec2 cursorPos = im->get_mouse_at();
            printf("World:  (%f, %f)\n", currentCursor.x, currentCursor.y);
            printf("Screen: (%f, %f)\n", cursorPos.x, cursorPos.y);
        }
    }

    { // player movement
        // @BUG normalize for diagonal
        float forwardForce = 0.0f;
        if (im->get_key_down(GLFW_KEY_W)) {
            forwardForce++;
        }
        if (im->get_key_down(GLFW_KEY_S)) {
            forwardForce--;
        }
        player->add_force(forwardForce * player->forward * player->FORWARD_SPEED);

        float horizontalForce = 0.0f;
        if (im->get_key_down(GLFW_KEY_A)) {
            horizontalForce--;
        }
        if (im->get_key_down(GLFW_KEY_D)) {
            horizontalForce++;
        }
        player->add_force(horizontalForce * -1 * player->left * player->FORWARD_SPEED * 0.2f);
    }
}

void Game::update(float dt) {
    gui->update();
    
    refresh_p2_data();
    player->update(dt);
    boids->update(dt);
    /* @TODO player pos/state updates should be done over UDP,
    will still want tcp ability though for general game setup/sync and player firing.
    Also will probably eventually just want to pass either player or game object to the client
    with an int to say which sort of data to send, playerdata or game state or whatever. will
    require developing a protocol between the client and server where there is a header that says
    what sort of data is about come in, as opposed to just knowing that it is gonna be 9 floats
    as has been hardcoded now. also probably should only send this update if the player 
    has actually moved */
    float playerState[9] = {
        player->physics.pos.x, 
        player->physics.pos.y,
        player->physics.rot,
        player->physics.vel.x,
        player->physics.vel.y,
        player->physics.forces.x,
        player->physics.forces.y,
        player->physics.forcesAt.x,
        player->physics.forcesAt.y
    };
    client.tcp_message_player_state(playerState, 9);
    player2->update(dt);
    
    check_collisions(dt);
}

void Game::refresh_p2_data() {
    // non blocking listen
    client.tcp_listen_player_state();
    if (client.hasUpdate) {
        player2->physics.pos = vec2(client.playerData[0], client.playerData[1]);
        player2->physics.rot = client.playerData[2];
        player2->physics.vel = vec2(client.playerData[3], client.playerData[4]);
        player2->physics.forces = vec2(client.playerData[5], client.playerData[6]);
        player2->physics.forcesAt = vec2(client.playerData[7], client.playerData[8]);
        client.hasUpdate = false;
    }
}

void Game::render() {
    if (DO.worldGrid) worldGrid.render();
    // @TODO make generic loop that renders all GameObjects? will need to think about class relations
    player->render();
    player2->render();
    boids->render();

    for (Block* b : blocks) {
        b->render();
    }
    if (DO.gui || gui->animating) gui->render();
}

void Game::load_resources() {
    ShaderManager::create_shader("simple_vert.glsl", "simple_color_frag.glsl", SHADER::PLAYER);
    ShaderManager::create_shader("line_vert.glsl", "simple_color_frag.glsl", SHADER::LINE);
    ShaderManager::create_shader("simple_vert.glsl", "simple_color_frag.glsl", SHADER::BLOCK);
    ShaderManager::create_shader("grid_vert.glsl", "grid_frag.glsl", SHADER::GRID);
    ShaderManager::create_shader("text_vert.glsl", "text_frag.glsl", SHADER::TEXT);
    ShaderManager::create_shader("screenspace_simple_vert.glsl", "simple_color_frag.glsl", SHADER::SCREENSPACE);

}

void Game::configure_shaders() {
    for (const std::pair<SHADER, unsigned int>& s : ShaderManager::shaders) {
        ShaderManager::set_mat4(
            s.first,
            "projection",
            cam.get_world_projection()
        );

        ShaderManager::set_mat4(
            s.first,
            "view",
            cam.get_view()
        );
    }

    ShaderManager::set_vec4(
        SHADER::LINE,
        "color",
        vec4(0.2f, 0.8f, 0.6f, 1.0f)
    );

    // overwrite projection matrix to render in 0->1 screenspace
    ShaderManager::set_mat4(
        SHADER::SCREENSPACE,
        "projection",
        cam.get_gui_projection()
    );

    ShaderManager::set_mat4(
        SHADER::TEXT,
        "projection",
        cam.get_gui_projection()
    );
}

void Game::set_window_size(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
    
    cam.update_bounds(width, height, visibleMin);
    configure_shaders();
}

vec2 Game::cursor_to_world(vec2 cursorPos) {
    cursorPos.x = (2.0f * cursorPos.x) / width - 1.0f;
    cursorPos.y = 1.0f - (2.0f * cursorPos.y) / height;
    mat4 screenToWorld = cam.get_screen_to_world();
    vec4 cursor = vec4(cursorPos, -1.0f, 0.0f);
    vec4 screenCoordinates = screenToWorld * cursor;
    return vec2(screenCoordinates.x, screenCoordinates.y);
}

void Game::zoom(float amount) {
    cam.slide_zoom(amount);
    visibleMin *= amount;
    configure_shaders();
}

// @TODO collisions arent working well when p2 moves over the network, investigate
void Game::check_collisions(float dt) {
    bool playerColliding = false;
    for (Block* b : blocks) {
        if (is_colliding(player, b)) {
            playerColliding = true;
        }
    }
    
    if (is_colliding(player, player2)) {
        // @TODO actually resolve collision, which is hard
        playerColliding = true;
    }

    // @TODO just a visualization for now since there is no actual collision resolution
    if (playerColliding) {
        vec4 c = vec4(0.1f, 0.5f, 0.8f, 1.0f);
        player->set_color(c);
    } else {
        vec4 c = vec4(1.0f, 0.4f, 0.4f, 1.0f);
        player->set_color(c);
    }
    
}

Game::~Game() {
    delete gui;
    delete player;
    delete player2;
    delete im;
    delete boids;
    for (Block* b : blocks) {
        delete b;
    }
    UIElement::shutdown();
    client.shutdown();
}
