module;

#include "clay.h"

export module Tooltip;

import UIColor;

/*

namespace UI {
    struct Tooltip {
        Clay_String title;
        Clay_String message;
        Clay_Color color;

    };

    void LayoutTooltip(
        const Tooltip &tooltip,
        const float positionX,
        const float positionY
    ) {
        CLAY({
            .layout = {
                .padding = CLAY_PADDING_ALL(5),
                .childGap = 10,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            .backgroundColor = tooltip.color,
            .cornerRadius = CLAY_CORNER_RADIUS(5),
            .floating = {
                .attachTo = CLAY_ATTACH_TO_ROOT,
                .offset = { positionX + 5, positionY + 5 },
                .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
            }
        }) {
            if (tooltip.title.length > 0) {
                CLAY_TEXT(tooltip.title, CLAY_TEXT_CONFIG({
                    .textColor = COLOR_WHITE,
                    .fontSize = 16,
                }));
            }

            if (tooltip.message.length > 0) {
                CLAY_TEXT(tooltip.message, CLAY_TEXT_CONFIG({
                    .textColor = COLOR_WHITE,
                    .fontSize = 14,
                }));
            }
        }
    }

    void Clay_HoverTooltip(
        const Clay_String &title,
        const Clay_String &message,
        const Clay_Color &color = COLOR_GRAY
        ) {
        Clay_OnHover(ShowTooltipOnHover, (intptr_t)tooltip);
    }
}

void ShowTooltipOnHover(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    // We don't need these, but they are part of the function signature
    (void)elementId;
    (void)pointerData;

    // Cast the user data back to our Tooltip pointer and render it
    Tooltip* tooltip = (Tooltip*)userData;
    Clay_Tooltip(tooltip);
}

*/