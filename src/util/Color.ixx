module;

#include <algorithm>

export module Color;

// pack structure to make it trivially copyable for networking
#pragma pack(push, 1)
export struct Color {
    static constexpr Color blend(const Color primary, const Color secondary, float factor) {
        factor = std::clamp(factor, 0.0f, 1.0f);
        const auto r_blend = static_cast<uint8_t>(static_cast<float>(primary.r) * (1.0f - factor) + static_cast<float>(secondary.r) * factor);
        const auto g_blend = static_cast<uint8_t>(static_cast<float>(primary.g) * (1.0f - factor) + static_cast<float>(secondary.g) * factor);
        const auto b_blend = static_cast<uint8_t>(static_cast<float>(primary.b) * (1.0f - factor) + static_cast<float>(secondary.b) * factor);
        return Color(r_blend, g_blend, b_blend);
    }

    explicit constexpr Color(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b) {}

    explicit constexpr Color(const uint32_t color)
        : r((color >> 16) & 0xFF),
          g((color >> 8) & 0xFF),
          b(color & 0xFF) {}

    constexpr Color() = default;

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    [[nodiscard]] Color blendWith(const Color other, const float factor) const {
        return blend(*this, other, factor);
    }

    [[nodiscard]] constexpr uint32_t toInt() const {
        return (static_cast<uint32_t>(this->r) << 16) |
               (static_cast<uint32_t>(this->g) << 8)  |
               (static_cast<uint32_t>(this->b));
    }
};
#pragma pack(pop)

export constexpr auto COLOR_BLACK       = Color(0, 0, 0);
export constexpr auto COLOR_WHITE       = Color(255, 255, 255);
export constexpr auto COLOR_RED         = Color(255, 0, 0);
export constexpr auto COLOR_GREEN       = Color(0, 255, 0);
export constexpr auto COLOR_BLUE        = Color(0, 0, 255);

export constexpr auto COLOR_FRIENDLY    = Color(0x00B2E1);