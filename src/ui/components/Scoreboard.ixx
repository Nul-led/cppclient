module;

#include <format>
#include <string>
#include <vector>

#include "clay.h"

export module Scoreboard;

import Color;
import UIColor;
import UIString;



namespace UI {
    std::string header("Scoreboard");

    export struct UIScoreboardEntry {
        std::string name;
        std::string suffix;
        Clay_Color color;
        uint64_t value;
    };

    std::string thing("playername - 50.3k");

    void Bar(
        const size_t idx,
        const uint64_t max,
        const UIScoreboardEntry &entry
    ) {
        const float progress = std::clamp(static_cast<float>(static_cast<double>(entry.value) / static_cast<double>(max)), 0.0f, 1.0f);

        const auto barId = CLAY_IDI("Scoreboard::Entry", idx);
        CLAY({
            .id = barId,
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_FIXED(320.0f),
                    .height = CLAY_SIZING_FIXED(80.0f / 3.0f),
                },
                .padding = CLAY_PADDING_ALL(4), // .15
            },
            .cornerRadius = CLAY_CORNER_RADIUS(15),
            .backgroundColor = COLOR_BLACK,
        }) {
            CLAY({
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_PERCENT(progress),
                        .height = CLAY_SIZING_GROW(),
                    },
                },
                .cornerRadius = CLAY_CORNER_RADIUS(15),
                .backgroundColor = entry.color,
            }) {}
        }

        CLAY({
            .layout = {
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
            },
            .floating = {
                .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID,
                .parentId = barId.id,
                .attachPoints = {
                    .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                    .parent = CLAY_ATTACH_POINT_CENTER_CENTER
                },
                .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
            }
        }) {


            CLAY_STR_TEXT(
            thing,
                //std::format("{} - {}{}", entry.name, formatCompact(entry.value), entry.suffix),
                20
            );
        }
    }

    export void Scoreboard(const std::vector<UIScoreboardEntry> &entries) {
        const Clay_ElementDeclaration panel = {
            .id = CLAY_ID("Scoreboard"),
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_FIT(),
                    .height = CLAY_SIZING_FIT(),
                },
                .padding = CLAY_PADDING_ALL(7),
                .childGap = 5,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childAlignment =  {
                    .x = CLAY_ALIGN_X_CENTER
                }
            },
            .backgroundColor = Clay_Color(0, 0, 0, 0.2),
            .cornerRadius = CLAY_CORNER_RADIUS(20),
        };

        CLAY(panel) {
            CLAY_STR_TEXT(header, 32);

            for (size_t i = 0; i < entries.size(); i++) {
                Bar(i, entries[0].value, entries[i]);
            }
        }
    }
}
