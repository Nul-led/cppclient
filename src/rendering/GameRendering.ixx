module;

#include <cstdint>
#include <math.h>
#include <string>

export module GameRendering;

import Canvas;
import Color;

struct EntityRenderData {};

namespace GameRendering {
#ifdef DEBUG
    double renderStart = 0.0f;
    double renderEnd = 0.0f;
#endif


    bool enableRawHealthValues = false;
    bool enableRenderBackgroundColor = true;
    bool enableRenderGrid = true;
    bool enableRenderBorders = true;

    export void renderBackground() {}

    export void renderEntity(
        const Context2d& ctx,
        const float x,
        const float y,
        const float rotation,
        const uint8_t bbType,
        const float bbScale1,
        const float bbScale2,
        const Color& color,
        const Color& strokeColor,
        const float strokeWidth,
        const uint8_t opacity,
        const bool isTrapezoid,
        const bool isStar
    ) {
        ctx.save();

        // optimize (precalc)
        ctx.translate(x, y);
        ctx.rotate(rotation);

        ctx.setGlobalAlpha(static_cast<float>(opacity) / 255.0f);
        ctx.setFillColor(color.r, color.g, color.b, 1.0f);
        ctx.setStrokeColor(strokeColor.r, strokeColor.g, strokeColor.b, 1.0f);
        ctx.setLineJoin("round");
        ctx.setLineWidth(strokeWidth);

        ctx.beginPath();

        switch (bbType) {
        case 0: {
            break;
        }
        case 1: {
            ctx.arc(0, 0, bbScale1, 0, M_PI * 2.0f, false);
            break;
        }
        case 2: {
            const auto hw = bbScale1 / 2.0f;
            const auto hh = bbScale2 / 2.0f;
            if (isTrapezoid) {
                ctx.moveTo(-hw, -hh);
                ctx.lineTo(hw, -hh * 1.75f);
                ctx.lineTo(hw, hh * 1.75f);
                ctx.lineTo(-hw, hh);
            }
            else {
                ctx.rect(-hw, -hh, bbScale1, bbScale2);
            }
            break;
        }
        default: {
            const auto vertices = bbType * (isStar ? 2 : 1);
            for (int i = 0; i < vertices; ++i) {
                auto offsetRotation = M_PI * 2.0f * i / vertices;
                if (vertices == 4) offsetRotation += M_PI_4;
                const auto heightFactor = (isStar && (i % 2) == 0) ? 0.4f : 1.0f;
                const auto vx = cos(offsetRotation) * bbScale1 * heightFactor;
                const auto vy = sin(offsetRotation) * bbScale1 * heightFactor;
                if (i == 0) ctx.moveTo(vx, vy);
                else ctx.lineTo(vx, vy);
            }
        }
        }
        ctx.closePath();

        ctx.fill();
        ctx.stroke();

        ctx.restore();
    }

    export void renderHealth(
        const Context2d& ctx,
        float x,
        float y,
        uint64_t value,
        uint64_t max
    ) {
        // blend color from green to red (value/max%)
    }

    export void renderScore(
        const Context2d& ctx,
        float x,
        float y,
        uint64_t value,
        const Color& color
    ) {}

    export void renderName(
        const Context2d& ctx,
        float x,
        float y,
        const std::string& value,
        const Color& color
    ) {}
}
