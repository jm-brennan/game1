#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight, vec3 pos, vec3 lookat, vec3 up, float squareBound) 
    : screenWidth(screenWidth), screenHeight(screenHeight), pos(pos), lookAt(lookAt), up(up), 
      squareBound(squareBound) {
    
    calculate_world_projection();
    calculate_gui_projection();
    calculate_view(pos, lookat, up);
    
    screenToWorld = inverse(view) * inverse(worldProjection);
}

void Camera::update_bounds(float newScreenWidth, float newScreenHeight, float newSquareBound) {
    screenWidth = newScreenWidth;
    screenHeight = newScreenHeight;
    squareBound = newSquareBound;
    calculate_world_projection();
    calculate_gui_projection();
    screenToWorld = inverse(view) * inverse(worldProjection);
}

void Camera::calculate_world_projection() {
    float ratio, top, right;
    if (screenWidth >= screenHeight) {
        ratio = screenWidth / screenHeight;
        top = squareBound;
        right = squareBound * ratio;
    } else {
        ratio = screenHeight / screenWidth;
        right = squareBound;
        top = squareBound * ratio;
    }
    
    worldProjection = ortho(
        -1*right,   // left      
        right,      // right    
        -1*top,     // bottom
        top,        // top      
        0.0f,       // near clip
        100.0f      // far clip
    );
    screenToWorld = inverse(view) * inverse(worldProjection);
}

void Camera::calculate_gui_projection() {
    guiProjection = ortho(
        0.0f,       // left  
        screenWidth,       // right 
        screenHeight,       // top   
        0.0f,       // bottom
        0.0f,       // near clip
        100.0f      // far clip
    );
    screenToWorld = inverse(view) * inverse(worldProjection);
}

void Camera::calculate_view(vec3 pos, vec3 lookat, vec3 up) {
    view = glm::lookAt(
        pos,
        lookat,
        up
    );
    screenToWorld = inverse(view) * inverse(worldProjection);
}

void Camera::slide_zoom(float multiplier) {
    squareBound *= multiplier;
    calculate_world_projection();
}

mat4 Camera::get_world_projection() { return worldProjection; }
mat4 Camera::get_gui_projection() { return guiProjection; }
mat4 Camera::get_view() { return view; }
mat4 Camera::get_screen_to_world() { return screenToWorld; }