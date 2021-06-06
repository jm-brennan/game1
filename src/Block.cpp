#include "Block.h"

Block::Block(vec2 pos, vec2 size, float rot, vec4 color) 
    : pos(pos), size(size), rot(rot), color(color) {
    shader = SHADER::BLOCK;
    rect = Rectangle(pos, size, color, shader);
}

void Block::render() {
   rect.render();
}