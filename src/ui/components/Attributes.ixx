module;

#include <algorithm>
#include <cassert>
#include <format>
#include <vector>

#include "clay.h"

export module Attributes;

import Progressbar;
import UIColor;
import UIString;

namespace UI {
    constexpr std::array ATTRIBUTE_COLORS = {
        Clay_Color(95, 158, 160, 1),  // Cadet Blue - a sophisticated, muted blue
        Clay_Color(205, 92, 92, 1),   // Indian Red - a warm, rich red
        Clay_Color(218, 165, 32, 1),  // Goldenrod - a deep, mellow gold
        Clay_Color(107, 142, 35, 1),  // Olive Drab - an earthy, refined green
        Clay_Color(138, 43, 226, 1),  // Blue Violet - a luxurious purple
        Clay_Color(255, 160, 122, 1), // Light Salmon - a soft, inviting peach
        Clay_Color(70, 130, 180, 1),  // Steel Blue - a strong, elegant blue
        Clay_Color(199, 21, 133, 1)   // Medium Violet Red - a vibrant, deep rose
    };

    std::array ATTRIBUTE_ALLOCATION_STRINGS = {
        CLAY_STRING("[1]"),
        CLAY_STRING("[2]"),
        CLAY_STRING("[3]"),
        CLAY_STRING("[4]"),
        CLAY_STRING("[5]"),
        CLAY_STRING("[6]"),
        CLAY_STRING("[7]"),
        CLAY_STRING("[8]"),
        CLAY_STRING("[9]"),
        CLAY_STRING("[0]"),
        CLAY_STRING("[ ]"),
    };

    void Bar(
        const size_t idx,
        const uint32_t current,
        const uint32_t max,
        const Clay_String &label
        ) {

        const float progress = static_cast<float>(current) / static_cast<float>(max);

        const auto barId = CLAY_IDI("AttributeBar", idx);
        const auto progressColor = ATTRIBUTE_COLORS[idx % ATTRIBUTE_COLORS.size()];

        CLAY({
            .id = barId,
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_FIXED(280.0f),
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
                .backgroundColor = progressColor,
            }) {}
        }

        if (!label.length) return;

        const auto gap = 280.0f / static_cast<float>(max - 1); // width / separators
        constexpr auto separatorWidth = 3.0f;

        CLAY({
            .layout = {
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                .childGap = static_cast<uint16_t>(gap - separatorWidth),
                .padding = {
                    .left = static_cast<uint16_t>(gap - 1.5f),
                }
            },
            .floating = {
                .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID,
                .parentId = barId.id,
                .attachPoints = {
                    .element = CLAY_ATTACH_POINT_LEFT_CENTER,
                    .parent = CLAY_ATTACH_POINT_LEFT_CENTER
                },
                .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
            }
        }) {
            for (size_t i = 0; i < max - 1; ++i) {
                CLAY({
                    .backgroundColor = COLOR_BLACK,
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIXED(separatorWidth),
                            .height = CLAY_SIZING_FIXED(16.0f / 3.0f), // .2
                        }
                    },
                }) {}
            }
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
            CLAY_TEXT(label, CLAY_TEXT_CONFIG({
                .textColor = COLOR_WHITE,
                .textAlignment = CLAY_TEXT_ALIGN_CENTER,
                .fontSize = 20, // .75
            }));
        }
    }

    void Button(const size_t idx) {
        CLAY({
            .id = CLAY_IDI("AttributeButton", idx),
            .layout = {
                .childAlignment = {
                    .x = CLAY_ALIGN_X_CENTER,
                    .y = CLAY_ALIGN_Y_CENTER,
                },
                .sizing = {
                    .width = CLAY_SIZING_FIT(25),
                    .height = CLAY_SIZING_FIT(25),
                }
            },
            .backgroundColor = Clay_Hovered() ? blend(ATTRIBUTE_COLORS[idx % ATTRIBUTE_COLORS.size()], COLOR_BLACK, 0.2) : ATTRIBUTE_COLORS[idx % ATTRIBUTE_COLORS.size()],
            .cornerRadius = CLAY_CORNER_RADIUS(12.5),
            .border = {
                .width = CLAY_BORDER_OUTSIDE(2),
                .color = COLOR_BLACK,
            }
        }) {
            CLAY_TEXT(Clay_Hovered() ? ATTRIBUTE_ALLOCATION_STRINGS[idx % ATTRIBUTE_ALLOCATION_STRINGS.size()] : CLAY_STRING("+"), CLAY_TEXT_CONFIG({
                .fontSize = 13,
                .textAlignment = CLAY_TEXT_ALIGN_CENTER,
                .textColor = COLOR_WHITE,
            }));
        }
    }

    std::string unallocatedPointsLabel;

    export void Attributes(
        const std::vector<Clay_String>& names,
        const std::vector<uint32_t>& allocations,
        const std::vector<uint32_t>& maxAllocations,
        const uint32_t unallocated
    ) {
        CLAY({
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .childGap = 5,
            }
        }) {
            const Clay_ElementDeclaration panel = {
                .id = CLAY_ID("Attributes"),
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
                for (size_t i = 0; i < names.size(); i++) {
                    CLAY({
                        .id = CLAY_IDI("Attribute", i),
                        .layout = {
                            .childGap = 5,
                            .childAlignment = {
                                .y = CLAY_ALIGN_Y_CENTER,
                            },
                            .layoutDirection = CLAY_LEFT_TO_RIGHT,
                            .sizing = {
                                .width = CLAY_SIZING_GROW()
                            }
                        }
                    }) {
                        Bar(i, allocations[i], maxAllocations[i], names[i]);
                        Button(i);
                    }
                }
            }

            if (unallocated) {
                CLAY({
                    .id = CLAY_ID("UnallocatedAttributes"),
                }) {
                    unallocatedPointsLabel = std::format("{}x", unallocated);
                    CLAY_TEXT(toUIString(unallocatedPointsLabel), CLAY_TEXT_CONFIG({
                        .fontSize = 28,
                        .textColor = COLOR_WHITE,
                    }));
                }
            }
        }
    }
}
