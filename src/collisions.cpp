#include "collisions.h"

/* 
@TODO these are all pretty naive checks, could start with some general checks if
the objects are within a radius of plausibly being able to collide, then do the
more intense calculation. might even need to do some grouping based on world
coordinates once the boids get involved if there are too many of them
 */


bool is_colliding(Player* p, Block* b) {
    for (vec2& v : p->vertices) {
        vec2 vPos = p->physics.pos + v;
        if (vPos.x >= b->pos.x && vPos.x <= (b->pos.x + b->size.x)) {
            if (vPos.y >= b->pos.y && vPos.y <= (b->pos.y + b->size.y)) {
                return true;
            }
        }
    }

    for (int i = 0; i < p->vertices.size(); i++) {
        int nextIndex = (i + 1) % (p->vertices.size());
        vec2 v1Pos = p->physics.pos + p->vertices[i];
        vec2 v2Pos = p->physics.pos + p->vertices[nextIndex];

        std::pair<bool, vec2> result;
        result = line_line_intersection(v1Pos.x, v1Pos.y, v2Pos.x, v2Pos.y, 
            b->pos.x, b->pos.y, b->pos.x + b->size.x, b->pos.y); // bottom edge
        if (result.first) return true;
        result = line_line_intersection(v1Pos.x, v1Pos.y, v2Pos.x, v2Pos.y, 
            b->pos.x + b->size.x, b->pos.y, b->pos.x + b->size.x, b->pos.y + b->size.y); // right edge
        if (result.first) return true;
        result = line_line_intersection(v1Pos.x, v1Pos.y, v2Pos.x, v2Pos.y, 
            b->pos.x + b->size.x, b->pos.y + b->size.y, b->pos.x, b->pos.y + b->size.y); // top edge
        if (result.first) return true;
        result = line_line_intersection(v1Pos.x, v1Pos.y, v2Pos.x, v2Pos.y, 
            b->pos.x, b->pos.y + b->size.y, b->pos.x, b->pos.y); // left edfe
        if (result.first) return true;
    }
    
    return false;
}

bool is_colliding(Player* a, Player* b) {
    vec2 diff = a->physics.pos - b->physics.pos;
    if ((diff.x * diff.x) + (diff.y * diff.y) > 
        (a->length * a->scale + b->length * b->scale)*(a->length * a->scale + b->length * b->scale)) {
        return false;
    }

    // @incomplete only checking line intersectsions, not vertex in player. maybe okay?

    for (int i = 0; i < a->vertices.size(); i++) {
        int nextAIndex = (i + 1) % a->vertices.size();
        vec2 av1 = a->physics.pos + a->vertices[i];
        vec2 av2 = a->physics.pos + a->vertices[nextAIndex];

        std::pair<bool, vec2> result;
        for (int j = 0; j < b->vertices.size(); j++) {
            int nextBIndex = (j + 1) % b->vertices.size();
            vec2 bv1 = b->physics.pos + b->vertices[i];
            vec2 bv2 = b->physics.pos + b->vertices[nextBIndex];

            result = line_line_intersection(av1.x, av1.y, av2.x, av2.y, bv1.x, bv1.y, bv2.x, bv2.y);
            if (result.first) return true;
        }
    }
    return false;
}

std::pair<bool, vec2> line_line_intersection(float l1x1, float l1y1, float l1x2, 
        float l1y2, float l2x1, float l2y1, float l2x2, float l2y2) {
    // get each line as Ax + By = C
    float a1 = l1y2 - l1y1;;
    float b1 = l1x1 - l1x2;
    float c1 = a1 * l1x1 + b1*l1y1;

    float a2 = l2y2 - l2y1;;
    float b2 = l2x1 - l2x2;
    float c2 = a2 * l2x1 + b2*l2y1;

    float det = a1 * b2 - a2*b1;
    if (det == 0.0f) { // lines are parallel, @SPEED maybe dont bother checking for this? seems unlikely and maybe not useful
        return std::make_pair(false, vec2(0.0f, 0.0f));
    } else {
        // get point of intersection for lines (as inifinite lines)
        float x = (b2*c1 - b1*c2) / det;
        float y = (a1*c2 - a2*c1) / det;

        float epsilon = 0.00001f;
        // check that the intersection is actually on the line segmments
        if (x >= min(l1x1, l1x2) - epsilon && x <= max(l1x1, l1x2) + epsilon
         && y >= min(l1y1, l1y2) - epsilon && y <= max(l1y1, l1y2) + epsilon
         && x >= min(l2x1, l2x2) - epsilon && x <= max(l2x1, l2x2) + epsilon
         && y >= min(l2y1, l2y2) - epsilon && y <= max(l2y1, l2y2) + epsilon) {
             return std::make_pair(true, vec2(x, y));
         } else {
             return std::make_pair(false, vec2(x, y)); 
         }
    }
}