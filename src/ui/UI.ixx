module;

#include <iostream>
#include <map>
#include <vector>
#include <emscripten/em_asm.h>
#include <emscripten/val.h>
#include <__ostream/print.h>

#define CLAY_IMPLEMENTATION
#include "../external/clay.h"

export module UI;

import Canvas;
import Image;
import Inputs;
import Viewport;

constexpr Clay_Color COLOR_LIGHT = {224, 215, 210, 255};
constexpr Clay_Color COLOR_RED = {168, 66, 28, 255};
constexpr Clay_Color COLOR_ORANGE = {225, 138, 50, 255};

export namespace UI {
    std::vector<std::string> fonts = { "Ubuntu" };
    std::map<std::string, Image> images;
    Context2d* ctx;
    Canvas2d* canvas;

    float textStrokeWidth = 0.2f;

    // ReSharper disable once CppPassValueParameterByConstReference
    void handleClayErrors(const Clay_ErrorData data) {
        std::cerr << std::string(data.errorText.chars, data.errorText.length) << std::endl;
    }

    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    // ReSharper disable once CppPassValueParameterByConstReference
    Clay_Dimensions measureText(Clay_StringSlice text, Clay_TextElementConfig *config, void* data) {
        float width;
        EM_ASM(({
            const ctx = Emval.toValue($0);
            ctx.font = $4 + "px " + UTF8ToString($5);
            const metrics = ctx.measureText(UTF8ToString($1, $2));
            HEAPF32[$3 >> 2] = metrics.width;
        }), ctx->ctx.as_handle(), text.chars, text.length, &width, config->fontSize, fonts[config->fontId].c_str());
        return { width, static_cast<float>(config->fontSize) };
    }

    // should always be followed by a restore when done
    void roundedRectClip(const Clay_BoundingBox &bounds, const Clay_CornerRadius &corners) {
        EM_ASM(({
            const ctx = Emval.toValue($0);
            ctx.save();
            ctx.beginPath();
            ctx.roundRect($1, $2, $3, $4, [$5, $6, $7, $8]);
            ctx.clip();
            ctx.closePath();
        }), ctx->ctx.as_handle(), bounds.x, bounds.y, bounds.width, bounds.height, corners.topLeft, corners.topRight, corners.bottomLeft, corners.bottomRight);
    }

    void render(const Clay_RenderCommandArray &commands) {
        for (int i = 0; i < commands.length; ++i) {
            const auto *command = &commands.internalArray[i];
            const auto [x, y, width, height] = command->boundingBox;
            switch (command->commandType) {
                case CLAY_RENDER_COMMAND_TYPE_NONE: {
                    break;
                }
                case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                    const auto [backgroundColor, cornerRadius] = command->renderData.rectangle;
                    ctx->beginPath();
                    ctx->setFillColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
                    if (cornerRadius.topLeft > 0 || cornerRadius.bottomLeft > 0 || cornerRadius.topRight > 0 || cornerRadius.bottomRight > 0) {
                        ctx->roundRect(x, y, width, height, cornerRadius.topLeft, cornerRadius.topRight, cornerRadius.bottomLeft, cornerRadius.bottomRight);
                        ctx->fill();
                    } else {
                        ctx->fillRect(x, y, width, height);
                    }
                    ctx->closePath();
                    break;
                }
                case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                    const auto [stringContents, textColor, fontId, fontSize, letterSpacing, lineHeight] = command->renderData.text;
                    ctx->setFont(std::format("{}px {}", fontSize, fonts[fontId]));
                    ctx->setTextBaseline("middle");
                    ctx->setFillColor(textColor.r, textColor.g, textColor.b, textColor.a);
                    ctx->setLineWidth(static_cast<float>(fontSize) * textStrokeWidth);
                    if (command->userData != nullptr) {
                        const auto strokeColor = static_cast<Clay_Color*>(command->userData);
                        ctx->setStrokeColor(strokeColor->r, strokeColor->g, strokeColor->b, strokeColor->a);
                    } else {
                        ctx->setStrokeColor(0, 0, 0, 255);
                    }
                    const auto text = std::string(stringContents.chars, stringContents.length);
                    ctx->strokeText(text, x, y + height / 2 + 1);
                    ctx->fillText(text, x, y + height / 2 + 1);
                    break;
                }
                case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                    EM_ASM(({
                        const ctx = Emval.toValue($0);
                        ctx.save();
                        ctx.beginPath();
                        ctx.roundRect($1, $2, $3, $4, [$5, $6, $7, $8]);
                        ctx.clip();
                        ctx.closePath();
                    }), ctx->ctx.as_handle(), x, y, width, height);
                    break;
                }
                case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                    ctx->restore();
                    break;
                }
                case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
                    /*
                    const auto [backgroundColor, cornerRadius, imageData] = command->renderData.image;
                    static_cast<std::string>(imageData);
                    if (images.contains(imageData)) {

                    }
                    */

                    break;
                }
                case CLAY_RENDER_COMMAND_TYPE_BORDER: {

                }
                default: ;
            }
        }
    }

    void init() {
        canvas = new Canvas2d(Canvas2d::getOffScreenSurface(1920, 1080));
        ctx = new Context2d(*canvas);

        const uint64_t memory = Clay_MinMemorySize();
        const Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(memory, malloc(memory));

        Clay_Initialize(arena, { 1920.0f, 1080.0f }, { handleClayErrors, nullptr });
        Clay_SetMeasureTextFunction(measureText, nullptr);

        Clay_SetDebugModeEnabled(true);
    }

    void ProgressBar(const Clay_ElementId &id, const float progress, const Clay_Color &backgroundColor, const Clay_Color &progressColor, const float width, const float height, const Clay_String &label = CLAY_STRING(""), const Clay_Color &labelColor = Clay_Color(255, 255, 255, 255)) {
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
                .fontSize = static_cast<uint16_t>(height * 0.75f), // Scale font size with height
            }));
        }
    }

    void layout() {
        canvas->setSize(Viewport::screenWidth, Viewport::screenHeight);
        Clay_SetLayoutDimensions({ static_cast<float>(Viewport::screenWidth), static_cast<float>(Viewport::screenHeight) });
        Clay_SetPointerState({ Inputs::screenMouseX, Inputs::screenMouseY }, Inputs::isLeftMouseButtonDown);
        Clay_UpdateScrollContainers(true, { Inputs::mouseWheelDeltaX, Inputs::mouseWheelDeltaY }, static_cast<float>(Viewport::deltaTime));

        Clay_BeginLayout();

        CLAY({
            .id = CLAY_ID("Root"),
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                    .height = CLAY_SIZING_GROW(),
                },
                .padding = {
                    .top = 100,
                    .left = 100,
                }
            },
            .backgroundColor = COLOR_ORANGE
        }) {
            ProgressBar(
                CLAY_ID("my-progress-bar-1"),
                50.0f / 100.0f,
                (Clay_Color){ 0, 0, 0, 255 },
                (Clay_Color){ 70, 130, 180, 255 },
                500,
                30,
                CLAY_STRING("test")
            );

        }

        render(Clay_EndLayout());
    }
};