module;

#include <cstdint>

export module Extraction;

export struct ExtractedEntityRenderable {
    float x;
    float y;
    float rotation;

    uint8_t bbType;
    float bbSide1;
    float bbSide2;

    float health;
    float maxHealth;

    uint64_t score;



};


