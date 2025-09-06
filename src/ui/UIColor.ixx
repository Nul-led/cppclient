module;

#include <algorithm>

#include "clay.h"

export module UIColor;

import Color;

namespace UI {
    export constexpr auto COLOR_BLACK = Clay_Color(0, 0, 0, 1);
    export constexpr auto COLOR_WHITE = Clay_Color(255, 255, 255, 1);
    export constexpr auto COLOR_RED = Clay_Color(255, 0, 0, 1);
    export constexpr auto COLOR_GREEN = Clay_Color(0, 255, 0, 1);
    export constexpr auto COLOR_BLUE = Clay_Color(0, 0, 255, 1);
    export constexpr auto COLOR_YELLOW = Clay_Color(255, 255, 0, 1);
    export constexpr auto COLOR_MAGENTA = Clay_Color(255, 0, 255, 1);
    export constexpr auto COLOR_CYAN = Clay_Color(0, 255, 255, 1);
    export constexpr auto COLOR_GRAY = Clay_Color(128, 128, 128, 1);

    export constexpr Clay_Color blend(const Clay_Color &primary, const Clay_Color &secondary, float factor) {
        factor = std::clamp(factor, 0.0f, 1.0f);
        const auto r_blend = static_cast<uint8_t>(static_cast<float>(primary.r) * (1.0f - factor) + static_cast<float>(secondary.r) * factor);
        const auto g_blend = static_cast<uint8_t>(static_cast<float>(primary.g) * (1.0f - factor) + static_cast<float>(secondary.g) * factor);
        const auto b_blend = static_cast<uint8_t>(static_cast<float>(primary.b) * (1.0f - factor) + static_cast<float>(secondary.b) * factor);
        const auto a_blend = static_cast<uint8_t>(static_cast<float>(primary.a) * (1.0f - factor) + static_cast<float>(secondary.a) * factor);
        return Clay_Color(r_blend, g_blend, b_blend, a_blend);
    }

    export Clay_Color toUIColor(const Color &color) {
        return Clay_Color(color.r, color.g, color.b, 1.0f);
    }
}
