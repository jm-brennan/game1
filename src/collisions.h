#pragma once
#include "Player.h"
#include "Block.h"

bool is_colliding(Player* p, Block* b);
bool is_colliding(Player* a, Player* b);

std::pair<bool, vec2> line_line_intersection(float l1x1, float l1y1, float l1x2, 
        float l1y2, float l2x1, float l2y1, float l2x2, float l2y2);
