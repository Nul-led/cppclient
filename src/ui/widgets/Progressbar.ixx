module;

#include "clay.h"

export module Progressbar;

namespace UI {
    export void ProgressBar(
        const Clay_ElementId &id,
        const float progress,
        const Clay_Color &backgroundColor,
        const Clay_Color &progressColor,
        const float width,
        const float height,
        const Clay_String &label = CLAY_STRING(""),
        const Clay_Color &labelColor = Clay_Color(255, 255, 255, 1)
        ) {
        CLAY({
            .id = id,
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_FIXED(width),
                    .height = CLAY_SIZING_FIXED(height)
                },
                .padding = CLAY_PADDING_ALL(static_cast<uint16_t>(height * 0.15f)),
            },
            .cornerRadius = CLAY_CORNER_RADIUS(50),
            .backgroundColor = backgroundColor,
        }) {
            CLAY({
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_PERCENT(progress),
                        .height = CLAY_SIZING_GROW(0),
                    },
                },
                .cornerRadius = CLAY_CORNER_RADIUS(50),
                .backgroundColor = progressColor,
            }) {}
        }

        if (!label.length) return;

        CLAY({
            .layout = {
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
            },
            .floating = {
                .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID,
                .parentId = id.id,
                .attachPoints = {
                    .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                    .parent = CLAY_ATTACH_POINT_CENTER_CENTER
                },
                .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
            }
        }) {
            CLAY_TEXT(label, CLAY_TEXT_CONFIG({
                .textColor = labelColor,
                .textAlignment = CLAY_TEXT_ALIGN_CENTER,
                .fontSize = static_cast<uint16_t>(height * 0.75f), // Scale font size with height
            }));
        }
    }
}
