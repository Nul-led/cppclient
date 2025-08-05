module;

#include <format>

#include "clay.h"

export module Statistics;

import UIColor;
import UIString;

using TankId = uint64_t;

namespace UI {
    std::string scoreStat;
    std::string levelStat;
    std::string killsStat;
    std::string timeStat;

    void StatLines(
        const uint64_t score,
        const uint64_t level,
        const uint64_t kills,
        const uint64_t time
    ) {
        scoreStat = std::format("{}", score);
        levelStat = std::format("{}", level);
        killsStat = std::format("{}", kills);
        timeStat = std::format("{}", formatSeconds(time));

        const Clay_ElementDeclaration container = {
            .id = CLAY_ID("Statistics::StatLines"),
            .layout = {
                .childGap = 10,
            }
        };

        const Clay_ElementDeclaration names = {
            .id = CLAY_ID("Statistics::StatLines::Names"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childAlignment = {CLAY_ALIGN_X_LEFT},
                .childGap = 10,
            }
        };

        const Clay_ElementDeclaration values = {
            .id = CLAY_ID("Statistics::StatLines::Values"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childAlignment = {CLAY_ALIGN_X_LEFT},
                .childGap = 10,
            }
        };

        const auto textConfig = CLAY_TEXT_CONFIG({ .fontSize = 28, .textColor = COLOR_WHITE,
                                                 .wrapMode = CLAY_TEXT_WRAP_NONE });

        CLAY(container) {
            CLAY(names) {
                CLAY_TEXT(CLAY_STRING("Score:"), textConfig);
                CLAY_TEXT(CLAY_STRING("Level:"), textConfig);
                CLAY_TEXT(CLAY_STRING("Time:"), textConfig);
                CLAY_TEXT(CLAY_STRING("Kills:"), textConfig);
            }

            CLAY(values) {
                CLAY_TEXT(toClayString(scoreStat), textConfig);
                CLAY_TEXT(toClayString(levelStat), textConfig);
                CLAY_TEXT(toClayString(timeStat), textConfig);
                CLAY_TEXT(toClayString(killsStat), textConfig);
            }
        }
    }

    void TankIcon(TankId id) {
        const Clay_ElementDeclaration container = {
            .id = CLAY_ID("Statistics::TankIcon"),
            .layout = {
                .sizing = {CLAY_SIZING_GROW()},
                .childAlignment = {.x = CLAY_ALIGN_X_RIGHT},
            }
        };

        CLAY(container) {
            CLAY({
                 .layout = {
                 .sizing = { CLAY_SIZING_FIXED(164), CLAY_SIZING_FIXED(164) }
                 },
                 .backgroundColor = COLOR_CYAN,
                 }) {
            }
        }
    }

    void Killer(
        const Clay_String &killerName = CLAY_STRING("An unnamed entity")
    ) {
        const Clay_ElementDeclaration container = {
            .id = CLAY_ID("Statistics::Killer"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childAlignment = {CLAY_ALIGN_X_CENTER},
            }
        };

        CLAY(container) {
            CLAY_TEXT(CLAY_STRING("You were killed by"),
                      CLAY_TEXT_CONFIG({ .fontSize = 28, .textColor = COLOR_WHITE }));
            CLAY_TEXT(killerName, CLAY_TEXT_CONFIG({ .fontSize = 28, .textColor = COLOR_WHITE }));
        }
    }

    void Actions() {
        const Clay_ElementDeclaration container = {
            .id = CLAY_ID("Statistics::Actions"),
            .layout = {
                .childAlignment = {.y = CLAY_ALIGN_Y_CENTER},
                .childGap = 20,
            }
        };

        CLAY(container) {
            CLAY({
                 .id = CLAY_ID("Statistics::SpectateButton"),
                 .backgroundColor = Clay_Hovered() ? blend(COLOR_GRAY, COLOR_WHITE, 0.2) : COLOR_GRAY,
                 .layout = {
                 .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                 .padding = CLAY_PADDING_ALL(10),
                 },
                 .cornerRadius = CLAY_CORNER_RADIUS(7),
                 }) {
                CLAY_TEXT(CLAY_STRING("Spectate"), CLAY_TEXT_CONFIG({ .fontSize = 32, .textColor = COLOR_WHITE }));
            }

            CLAY({
                 .id = CLAY_ID("Statistics::ContinueButton"),
                 .backgroundColor = Clay_Hovered() ? blend(COLOR_GRAY, COLOR_WHITE, 0.2) : COLOR_GRAY,
                 .layout = {
                 .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                 .padding = CLAY_PADDING_ALL(10),
                 },
                 .cornerRadius = CLAY_CORNER_RADIUS(7)
                 }) {
                CLAY_TEXT(CLAY_STRING("Continue"), CLAY_TEXT_CONFIG({ .fontSize = 32, .textColor = COLOR_WHITE }));
            }
        }
    }

    export void Statistics(
        const uint64_t score,
        const uint64_t level,
        const uint64_t kills,
        const uint64_t time,
        const TankId tank = 0,
        const Clay_String &killerName = CLAY_STRING("An unnamed entity")
    ) {
        const Clay_ElementDeclaration container = {
            .id = CLAY_ID("Statistics"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childGap = 100,
                .childAlignment = {.x = CLAY_ALIGN_X_CENTER}
            }
        };

        const Clay_ElementDeclaration splitContainer = {
            .id = CLAY_ID("Statistics::SplitContainer"),
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = {CLAY_SIZING_FIXED(600)}
            }
        };


        CLAY(container) {
            Killer(killerName);
            CLAY(splitContainer) {
                StatLines(score, level, kills, time);
                TankIcon(tank);
            }
            Actions();
        }
    }
}
