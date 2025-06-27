module;

#include <cstdint>

export module Camera;

import Reader;
import NetProp;
import Interpolation;

enum class CameraNetProps : uint64_t {
    Width   = 1 << 0,
    Height  = 1 << 1,
    Padding = 1 << 2,
};

inline bool operator&(const unsigned long long lhs, CameraNetProps rhs) {
    return lhs & static_cast<uint64_t>(rhs);
}

export struct Camera {

};