#pragma once

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;


class Camera {
public:
    Camera(float screenWidth, float screenHeight, vec3 pos, vec3 lookAt, vec3 up, float squareBound);

    void update_bounds(float newScreenWidth, float newScreenHeight, float newSquareBound);

    void calculate_world_projection();
    void calculate_gui_projection();
    void calculate_view(vec3 pos, vec3 lookat, vec3 up);

    void slide_zoom(float multiplier);

    mat4 get_world_projection();
    mat4 get_gui_projection();
    mat4 get_view();
    mat4 get_screen_to_world();

private:
    float screenWidth, screenHeight, squareBound;
    // use a different projection matrix to render worldspace or gui elements in screenspace
    mat4 worldProjection;
    mat4 guiProjection;
    mat4 view;
    mat4 screenToWorld;
    vec3 pos, lookAt, up;
};