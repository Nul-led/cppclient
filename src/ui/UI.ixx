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

import TankData;
import Canvas;
import Image;
import Inputs;
import Viewport;

import Progressbar;
import Button;

import Attributes;
import Upgrades;
import Statistics;

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
                   const str = UTF8ToString($1, $2);
                   ctx.font = $3 + "px " + UTF8ToString($4);
                   const metrics = ctx.measureText(str);
                   HEAPF32[$5 >> 2] = metrics.width;
                   }), ctx->ctx.as_handle(), text.chars, text.length, config->fontSize, fonts[config->fontId].c_str(), &width);
        return { width, static_cast<float>(config->fontSize) * (1 + textStrokeWidth)  };
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
                    ctx->setFont(std::format("{}px {}", static_cast<float>(fontSize), fonts[fontId]));
                    ctx->setTextBaseline("middle");
                    ctx->setFillColor(textColor.r, textColor.g, textColor.b, textColor.a);
                    const auto lineWidth = static_cast<float>(fontSize) * textStrokeWidth;
                    ctx->setLineWidth(lineWidth);
                    if (command->userData != nullptr) {
                        const auto strokeColor = static_cast<Clay_Color*>(command->userData);
                        ctx->setStrokeColor(strokeColor->r, strokeColor->g, strokeColor->b, strokeColor->a);
                    } else {
                        ctx->setStrokeColor(0, 0, 0, 255);
                    }
                    const auto text = std::string(stringContents.chars, stringContents.length);
                    ctx->strokeText(text, x, y + height / 2);
                    ctx->fillText(text, x, y + height / 2);
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
                    const auto [color, cornerRadius, borderWidth] = command->renderData.border;
                    const auto halfLWT = static_cast<float>(borderWidth.top) / 2.0f;
                    const auto halfLWR = static_cast<float>(borderWidth.right) / 2.0f;
                    const auto halfLWB = static_cast<float>(borderWidth.bottom) / 2.0f;
                    const auto halfLWL = static_cast<float>(borderWidth.left) / 2.0f;

                    ctx->setStrokeColor(color.r, color.g, color.b, color.a);

                     // Top border
                    if (borderWidth.top > 0) {
                        ctx->beginPath();
                        ctx->setLineWidth(borderWidth.top);
                        // Draw line from end of top-left corner to start of top-right corner
                        ctx->moveTo(x + cornerRadius.topLeft, y + halfLWT);
                        ctx->lineTo(x + width - cornerRadius.topRight, y + halfLWT);
                        ctx->stroke();
                    }

                    // Right border
                    if (borderWidth.right > 0) {
                        ctx->beginPath();
                        ctx->setLineWidth(borderWidth.right);
                        // Draw line from end of top-right corner to start of bottom-right corner
                        ctx->moveTo(x + width - halfLWR, y + cornerRadius.topRight);
                        ctx->lineTo(x + width - halfLWR, y + height - cornerRadius.bottomRight);
                        ctx->stroke();
                    }


                    // Bottom border
                    if (borderWidth.bottom > 0) {
                        ctx->beginPath();
                        ctx->setLineWidth(borderWidth.bottom);
                        // Draw line from end of bottom-right corner to start of bottom-left corner
                        ctx->moveTo(x + width - cornerRadius.bottomRight, y + height - halfLWB);
                        ctx->lineTo(x + cornerRadius.bottomLeft, y + height - halfLWB);
                        ctx->stroke();
                    }

                    // Left border
                    if (borderWidth.left > 0) {
                        ctx->beginPath();
                        ctx->setLineWidth(borderWidth.left);
                        // Draw line from end of bottom-left corner to start of top-left corner
                        ctx->moveTo(x + halfLWL, y + height - cornerRadius.bottomLeft);
                        ctx->lineTo(x + halfLWL, y + cornerRadius.topLeft);
                        ctx->stroke();
                    }

                    // Top-left corner
                    if (cornerRadius.topLeft > 0) {
                        // Use the width of the clockwise-next side (top)
                        if (borderWidth.top > 0 || borderWidth.left > 0) {
                            ctx->beginPath();
                            ctx->setLineWidth(borderWidth.top > 0 ? borderWidth.top : borderWidth.left);
                            ctx->moveTo(x + halfLWL, y + cornerRadius.topLeft);
                            ctx->arcTo(x + halfLWL, y + halfLWT, x + cornerRadius.topLeft, y + halfLWT, cornerRadius.topLeft);
                            ctx->stroke();
                        }
                    }

                    // Top-right corner
                    if (cornerRadius.topRight > 0) {
                        // Use the width of the clockwise-next side (right)
                        if (borderWidth.right > 0 || borderWidth.top > 0) {
                            ctx->beginPath();
                            ctx->setLineWidth(borderWidth.right > 0 ? borderWidth.right : borderWidth.top);
                            ctx->moveTo(x + width - cornerRadius.topRight, y + halfLWT);
                            ctx->arcTo(x + width - halfLWR, y + halfLWT, x + width - halfLWR, y + cornerRadius.topRight, cornerRadius.topRight);
                            ctx->stroke();
                        }
                    }

                    // Bottom-right corner
                    if (cornerRadius.bottomRight > 0) {
                        // Use the width of the clockwise-next side (bottom)
                        if (borderWidth.bottom > 0 || borderWidth.right > 0) {
                            ctx->beginPath();
                            ctx->setLineWidth(borderWidth.bottom > 0 ? borderWidth.bottom : borderWidth.right);
                            ctx->moveTo(x + width - halfLWR, y + height - cornerRadius.bottomRight);
                            ctx->arcTo(x + width - halfLWR, y + height - halfLWB, x + width - cornerRadius.bottomRight, y + height - halfLWB, cornerRadius.bottomRight);
                            ctx->stroke();
                        }
                    }

                    // Bottom-left corner
                    if (cornerRadius.bottomLeft > 0) {
                        // Use the width of the clockwise-next side (left)
                        if (borderWidth.left > 0 || borderWidth.bottom > 0) {
                            ctx->beginPath();
                            ctx->setLineWidth(borderWidth.left > 0 ? borderWidth.left : borderWidth.bottom);
                            ctx->moveTo(x + cornerRadius.bottomLeft, y + height - halfLWB);
                            ctx->arcTo(x + halfLWL, y + height - halfLWB, x + halfLWL, y + height - cornerRadius.bottomLeft, cornerRadius.bottomLeft);
                            ctx->stroke();
                        }
                    }
                    break;
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

    const auto testChars = "test";
    float testFontLoadWidth = 0;

    void layout() {
        canvas->setSize(Viewport::screenWidth, Viewport::screenHeight);

        {
            const auto width = measureText(Clay_StringSlice(4, testChars, testChars), CLAY_TEXT_CONFIG({
                .fontId = 0,
                .fontSize = 16,
            }), nullptr).width;

            if (width != testFontLoadWidth) {
                std::cout << "Detected Font Load" << std::endl;
                Clay_ResetMeasureTextCache();
                testFontLoadWidth = width;
            }
        }

        Clay_SetLayoutDimensions({ static_cast<float>(Viewport::screenWidth), static_cast<float>(Viewport::screenHeight) });
        Clay_SetPointerState({ Inputs::screenMouseX, Inputs::screenMouseY }, Inputs::isLeftMouseButtonDown);
        Clay_UpdateScrollContainers(true, { Inputs::mouseWheelDeltaX, Inputs::mouseWheelDeltaY }, static_cast<float>(Viewport::deltaTime));

        Clay_BeginLayout();

        Clay_SetCullingEnabled(false);

        CLAY({
            .id = CLAY_ID("Root"),
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                    .height = CLAY_SIZING_GROW(),
                },
                .padding = {
                    .left = 100,
                    .top = 100,
                },
                .layoutDirection = CLAY_TOP_TO_BOTTOM
            },
            .backgroundColor = COLOR_LIGHT
        }) {

            Statistics(
                100,
                45,
                5,
                1000000,
                CLAY_STRING("test"),
                0
            );

        }

        render(Clay_EndLayout());
    }
};