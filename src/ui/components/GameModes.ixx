module;

#include <vector>
#include "clay.h"

export module GameModes;

import Servers;
import UIColor;

namespace UI {

    void GameModeButton(
        const ServerGameOptions& mode,
        const bool isSelected,
        const size_t index
    ) {
        CLAY({
            .id = CLAY_IDI("GameModes::Button", index),
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(90), CLAY_SIZING_FIXED(35) },
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
            },
            .backgroundColor = Clay_Hovered() ? COLOR_BLUE : COLOR_CYAN,
            .cornerRadius = CLAY_CORNER_RADIUS(5),
            .border = {
                .color = COLOR_GRAY,
                .width = CLAY_BORDER_ALL(2)
            }
        }) {
            CLAY_TEXT(CLAY_STRING("Ignore"), CLAY_TEXT_CONFIG({.textColor = COLOR_WHITE, .fontSize = 14}));
        }
    }

    export void GameModes(
        const std::vector<ServerGameOptions> &modes,
        const GameModeIds selectedId = GameModeIds::FFA
    ) {
        const Clay_ElementDeclaration panel = {
            .id = CLAY_ID("GameModes"),
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_FIT(),
                    .height = CLAY_SIZING_FIT(),
                },
                .padding = CLAY_PADDING_ALL(7),
                .childGap = 5,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            .backgroundColor = Clay_Color(0, 0, 0, 0.2),
            .cornerRadius = CLAY_CORNER_RADIUS(20),
        };

        CLAY(panel) {
            for (size_t i = 0; i < modes.size(); ++i) {
                const auto& mode = modes[i];
                GameModeButton(mode, selectedId == mode.id, i);
            }
        }

    }
}