module;

#include <vector>

#include "clay.h"

export module Upgrades;

import TankData;
import UIColor;

using TankId = uint16_t;

namespace UI {
    void Upgrade(const size_t idx, const TankId tankId) {
        const auto id = CLAY_IDI("UpgradeSlot", idx);

        CLAY({
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(140), CLAY_SIZING_FIXED(140) },
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
            },
            .backgroundColor = COLOR_GRAY,
            .cornerRadius = CLAY_CORNER_RADIUS(7),
        }) {
            CLAY({
                .id = id,
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED(130), CLAY_SIZING_FIXED(130) },
                    .childAlignment = { CLAY_ALIGN_X_CENTER },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = {
                        .top = 10,
                    }
                },
                .backgroundColor = COLOR_BLUE,
                .cornerRadius = CLAY_CORNER_RADIUS(7),
            }) {
                // TODO: Replace with tank image using .image config
                CLAY({
                    .backgroundColor = { 255, 0, 0, 0.5 },
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIXED(110),
                            .height = CLAY_SIZING_FIXED(110),
                        }
                    }
                }) {
                    CLAY_TEXT(CLAY_STRING("TankRender\nPlaceholder"), CLAY_TEXT_CONFIG({
                        .textColor = COLOR_WHITE,
                        .fontSize = 10
                    }));
                }
            }

            CLAY({
                .layout = {
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                    .padding = {
                        .top = 107,
                    }
                },
                .floating = {
                    .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID,
                    .parentId = id.id,
                    .attachPoints = {
                        .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                        .parent = CLAY_ATTACH_POINT_CENTER_CENTER,
                    },
                    .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                }
            }) {
                CLAY_TEXT(CLAY_STRING("Overlord"), CLAY_TEXT_CONFIG({
                    .textColor = COLOR_WHITE,
                    .fontSize = 16,
                    .textAlignment = CLAY_TEXT_ALIGN_CENTER,
                }));
            }
        }
    }

    void Ignore() {
        // Renders a dark gray "Ignore" button with the same border style as the Upgrade button.
        CLAY({
            .id = CLAY_ID("IgnoreButton"),
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(90), CLAY_SIZING_FIXED(35) },
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
            },
            .backgroundColor = COLOR_BLUE,
            .cornerRadius = CLAY_CORNER_RADIUS(5),
            .border = {
                .color = COLOR_GRAY,
                .width = CLAY_BORDER_ALL(2) // Same border width as Upgrade Button
            }
        }) {
            CLAY_TEXT(CLAY_STRING("Ignore"), CLAY_TEXT_CONFIG({.textColor = COLOR_WHITE, .fontSize = 14}));
        }
    }

    constexpr auto MAX_UPGRADES_PER_LINE = 3;

    export void Upgrades(const std::vector<uint16_t> &upgradeIds, std::vector<TankDefinition> &tanks) {
        CLAY({
            .id = CLAY_ID("Upgrades"),
            .layout = {
                .sizing = { CLAY_SIZING_FIT(), CLAY_SIZING_FIT() },
                .padding = CLAY_PADDING_ALL(7),
                .childGap = 15,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childAlignment = { .x = CLAY_ALIGN_X_LEFT }
            },
        }) {
            for (size_t i = 0; i < upgradeIds.size(); i += MAX_UPGRADES_PER_LINE) {
                CLAY({
                    .id = CLAY_IDI("UpgradeLine", i),
                    .layout = {
                        .sizing = { CLAY_SIZING_FIT(), CLAY_SIZING_FIT() },
                        .childGap = 20,
                        .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    }
                }) {
                    for (size_t j = 0; j < MAX_UPGRADES_PER_LINE; ++j) {
                        const size_t upgradeIndex = i + j;

                        if (upgradeIndex >= upgradeIds.size()) {
                            break;
                        }

                        Upgrade(upgradeIndex, upgradeIds[upgradeIndex]);
                    }
                }
            }

            CLAY({
                .id = CLAY_ID("ActionButtonsRow"),
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT() },
                    .padding = { .top = 10 },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER }
                }
            }) {
                Ignore();
            }
        }
    }
}