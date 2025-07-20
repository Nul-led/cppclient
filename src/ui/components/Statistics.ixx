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

        constexpr Clay_ElementDeclaration container = {
            .layout = {
                .childGap = 10
            }
        };

        CLAY(container) {
            CLAY({
                 .id = CLAY_ID("StatNamesList"),
                 .layout = {
                 .layoutDirection = CLAY_TOP_TO_BOTTOM,
                 .childAlignment = { CLAY_ALIGN_X_LEFT }
                 }
                 }) {
                CLAY_TEXT(CLAY_STRING("Score:"), CLAY_TEXT_CONFIG({
                              .fontSize = 28,
                              .textColor = COLOR_WHITE,
                              }));
                CLAY_TEXT(CLAY_STRING("Level:"), CLAY_TEXT_CONFIG({
                              .fontSize = 28,
                              .textColor = COLOR_WHITE,
                              }));
                CLAY_TEXT(CLAY_STRING("Time:"), CLAY_TEXT_CONFIG({
                              .fontSize = 28,
                              .textColor = COLOR_WHITE,
                              }));
                CLAY_TEXT(CLAY_STRING("Kills:"), CLAY_TEXT_CONFIG({
                              .fontSize = 28,
                              .textColor = COLOR_WHITE,
                              }));
            }


            CLAY({
                 .id = CLAY_ID("StatValuesList"),
                 .layout = {
                 .layoutDirection = CLAY_TOP_TO_BOTTOM,
                 .childAlignment = { CLAY_ALIGN_X_LEFT }
                 }
                 }) {
                CLAY_TEXT(toUIString(scoreStat), CLAY_TEXT_CONFIG({
                              .fontSize = 28,
                              .textColor = COLOR_WHITE,
                              }));
                CLAY_TEXT(toUIString(levelStat), CLAY_TEXT_CONFIG({
                              .fontSize = 28,
                              .textColor = COLOR_WHITE,
                              }));
                CLAY_TEXT(toUIString(timeStat), CLAY_TEXT_CONFIG({
                              .fontSize = 28,
                              .textColor = COLOR_WHITE,
                              }));
                CLAY_TEXT(toUIString(killsStat), CLAY_TEXT_CONFIG({
                              .fontSize = 28,
                              .textColor = COLOR_WHITE,
                              }));
            }
        }
    }

    void Killer(
        const Clay_String &killerName = CLAY_STRING("An unnamed entity")
    ) {
        constexpr Clay_ElementDeclaration container = {
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

    export void Statistics(
        const uint64_t score,
        const uint64_t level,
        const uint64_t kills,
        const uint64_t time,
        const TankId tank = 0,
        const Clay_String &killerName = CLAY_STRING("An unnamed entity"),
    ) {
        constexpr Clay_ElementDeclaration container = {
            .id = CLAY_ID("Statistics"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childGap = 50,
                .sizing = {CLAY_SIZING_GROW(0, 300)}
            }
        };

        constexpr Clay_ElementDeclaration splitContainer = {
            .id = CLAY_ID("Statistics::SplitContainer"),
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            }
        };

        CLAY(container) {
            Killer(killerName);

        }
    }
}
