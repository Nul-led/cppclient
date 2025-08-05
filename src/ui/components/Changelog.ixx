module;

#include <string>

#include "clay.h"

export module Changelog;

import UIString;
import UIColor;

namespace UI {
    std::string header("Changelog");
    std::string lines("test\ntest2\ntest123349539045390485longlol\nand another one\nthis goes on forever");

    export void Changelog() {
        const Clay_ElementDeclaration panel = {
            .id = CLAY_ID("Changelog"),
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

        const Clay_ElementDeclaration headerContainer = {
            .id = CLAY_ID("Changelog::Header"),
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                },
                .childAlignment = {
                    .x = CLAY_ALIGN_X_CENTER
                }
            }
        };

        const Clay_ElementDeclaration linesContainer = {
            .id = CLAY_ID("Changelog::Lines"),
        };

        CLAY(panel) {
            CLAY(headerContainer) {
                CLAY_STR_TEXT(header, 28);
            }
            CLAY(linesContainer) {
                CLAY_STR_TEXT(lines, 20, COLOR_WHITE, CLAY_TEXT_WRAP_NEWLINES, CLAY_TEXT_ALIGN_LEFT);
            }
        }
    }
}