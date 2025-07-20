module;

#include <cstdint>

export module CameraData;

import Reader;
import NetProp;
import Interpolation;

enum class CameraNetProps : uint64_t {
    X           = 1 << 0,
    Y           = 1 << 1,
    Scale       = 1 << 2,
    Rotation    = 1 << 3,
    Anchor      = 1 << 4,
    AnchorSpeed = 1 << 5,
    Flags       = 1 << 6,
};

inline bool operator&(const unsigned long long lhs, CameraNetProps rhs) {
    return lhs & static_cast<uint64_t>(rhs);
}

export struct CameraData {


};