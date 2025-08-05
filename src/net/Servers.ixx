module;

#include <string>

export module Servers;

export enum class GameModeIds : uint64_t {
    FFA = 0,
    Maze,
    Sandbox
};

export struct ServerGameOptions {
    GameModeIds mode;
    std::string name;
};
