module;

#define CLAY_IMPLEMENTATION
#include "../external/clay.h"

export module UI;

import Canvas;

struct UI {
    Context2d ctx;

    void render(const Clay_RenderCommandArray &commands) {
        for (int i = 0; i < commands.length; ++i) {
            switch (const auto *command = &commands.internalArray[i]; command->commandType) {
                case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {

                }
                default: ;
            }
        }
    }

    void rect(Clay_RectangleRenderData data) {

    }

    void border() {

    }

    void text() {

    }

    void image() {

    }

    void scissorStart(const double x, const double y, const double width, const double height) const {

    }
};