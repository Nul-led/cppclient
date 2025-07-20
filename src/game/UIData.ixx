module;

#include <cstdint>

export module UIData;

enum class UINetProps : uint64_t {

};

export inline bool operator&(const uint64_t lhs, const UINetProps rhs) {
    return lhs & static_cast<uint64_t>(rhs);
}

export enum class EntityFlags : uint8_t {
    DisableSpawning = 1 << 0,
    DisableScores   = 1 << 1,
    DisableNames    = 1 << 2,
    DisableHealth   = 1 << 3,
};