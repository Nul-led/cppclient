module;

#include <vector>
#include "clay.h"

export module GameModes;

import Servers;

namespace UI {

    export void GameModes(
        const std::vector<ServerGameOptions> &modes,
        const GameModeIds selectedId = GameModeIds::FFA
    ) {


    }
}