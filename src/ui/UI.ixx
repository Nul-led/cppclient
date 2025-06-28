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
                    Clay_RenderCommand::
                }
                default: ;
            }
        }
    }

    void rect(Clay_RectangleRenderData data) {

    }

    void border(Clay_BorderRenderData data) {


    }

    void text() {

    }

    void image() {

    }

    void scissorStart(const double x, const double y, const double width, const double height) const {
        ctx.save();
        ctx.beginPath();
        ctx.rect(x, y, width, height);
        ctx.clip();
        ctx.closePath();
    }

    void scissorEnd() const {
        ctx.restore();
    }
};