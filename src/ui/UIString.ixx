module;

#include <format>
#include <string>

#include "clay.h"

export module UIString;

// lifetimes must match!
export Clay_String toClayString(const std::string &str) {
    return Clay_String(false, static_cast<int32_t>(str.length()), str.c_str());
}

export void CLAY_STR_TEXT(
    const std::string &str,
    const uint16_t fontSize = 16,
    const Clay_Color &textColor = Clay_Color(255, 255, 255, 1.0f),
    const Clay_TextElementConfigWrapMode wrapMode = CLAY_TEXT_WRAP_NONE,
    const Clay_TextAlignment textAlignment = CLAY_TEXT_ALIGN_CENTER
) {
    CLAY_TEXT(toClayString(str), CLAY_TEXT_CONFIG({
                  .fontId = 0,
                  .fontSize = fontSize,
                  .textColor = textColor,
                  .wrapMode = wrapMode,
                  .textAlignment = textAlignment,
                  }));
}

export std::string formatSeconds(uint64_t totalSeconds) {
    constexpr uint64_t SECONDS_IN_MINUTE = 60;
    constexpr uint64_t SECONDS_IN_HOUR = 60 * SECONDS_IN_MINUTE;
    constexpr uint64_t SECONDS_IN_DAY = 24 * SECONDS_IN_HOUR;

    uint64_t days = totalSeconds / SECONDS_IN_DAY;
    totalSeconds %= SECONDS_IN_DAY;

    uint64_t hours = totalSeconds / SECONDS_IN_HOUR;
    totalSeconds %= SECONDS_IN_HOUR;

    uint64_t minutes = totalSeconds / SECONDS_IN_MINUTE;
    uint64_t seconds = totalSeconds % SECONDS_IN_MINUTE;

    std::string result_str;
    result_str.reserve(30);

    bool needs_space = false;

    if (days > 0) {
        result_str += std::format("{}d", days);
        needs_space = true;
    }

    if (hours > 0) {
        if (needs_space) {
            result_str += " ";
        }
        result_str += std::format("{:02}h", hours);
        needs_space = true;
    }

    if (minutes > 0) {
        if (needs_space) {
            result_str += " ";
        }
        result_str += std::format("{:02}m", minutes);
        needs_space = true;
    }

    if (seconds > 0) {
        if (needs_space) {
            result_str += " ";
        }
        result_str += std::format("{:02}s", seconds);
    }

    if (result_str.empty()) {
        return "0s";
    }

    return result_str;
}
