module;

#include "clay.h"

export module Menu;

import Changelog;
import UIColor;

namespace UI {
    export void MenuView() {
        CLAY({
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW() },
                .padding = { .top = 20 },
                .childAlignment = { .y = CLAY_ALIGN_Y_TOP }
            }
        }) {
            CLAY({
                .layout = {
                    .sizing = { .width = CLAY_SIZING_GROW() },
                    .padding = { .left = 20 }
                }
            }) {
                Changelog();
            }

            CLAY({
                .layout = {
                    .sizing = { .width = CLAY_SIZING_GROW() },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER }
                }
            }) {
                // GameModes();
                CLAY({
                    .layout = {
                        .sizing = { .width = CLAY_SIZING_FIXED(400), .height = CLAY_SIZING_FIXED(100) },
                    },
                    .backgroundColor = COLOR_BLUE,
                }) {}
            }

            CLAY({ .layout = { .sizing = { .width = CLAY_SIZING_GROW() } } }) {}
        }

        CLAY({
            .layout = {
                .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
            }
        }) {
            CLAY({
                .layout = {
                    .sizing = { .width = CLAY_SIZING_FIXED(500), .height = CLAY_SIZING_FIXED(300) },
                },
                .backgroundColor = COLOR_BLUE,
            }) {}
            //NameInput();
        }

        CLAY({
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW() },
                .padding = { .bottom = 20 },
                .childAlignment = { .y = CLAY_ALIGN_Y_BOTTOM }
            }
        }) {
            CLAY({
                .layout = {
                    .sizing = { .width = CLAY_SIZING_GROW() },
                    .padding = { .left = 20 }
                }
            }) {
                CLAY({
                    .layout = {
                        .sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_FIXED(150) },
                    },
                    .backgroundColor = COLOR_BLUE,
                }) {}
                //Socials();
            }

            CLAY({ .layout = { .sizing = { .width = CLAY_SIZING_GROW() } } }) {}

            CLAY({
                .layout = {
                    .sizing = { .width = CLAY_SIZING_GROW() },
                    .childAlignment = { .x = CLAY_ALIGN_X_RIGHT },
                    .padding = { .right = 20 }
                }
            }) {
                CLAY({
                    .layout = {
                        .sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_FIXED(150) },
                    },
                    .backgroundColor = COLOR_BLUE,
                }) {}
                //Actions();
            }
        }
    }
}