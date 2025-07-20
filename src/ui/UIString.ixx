module;

#include <format>
#include <string>

#include "clay.h"

export module UIString;

// lifetimes must match!
export Clay_String toUIString(const std::string& str) {
    return Clay_String(false, static_cast<int32_t>(str.length()), str.c_str());
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