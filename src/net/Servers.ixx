module;

#include <string>

export module Servers;

import Color;

export enum class GameModeIds : uint64_t {
    FFA = 0,
    Maze,
    Sandbox
};

export struct ServerGameOptions {
    GameModeIds id;
    std::string name;
    Color color;
};
