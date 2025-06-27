module;

#include <cstdint>

export module Arena;

import Reader;
import NetProp;
import Interpolation;

enum class ArenaNetProps : uint64_t {
    Width   = 1 << 0,
    Height  = 1 << 1,
    Padding = 1 << 2,
};

inline bool operator&(const unsigned long long lhs, ArenaNetProps rhs) {
    return lhs & static_cast<uint64_t>(rhs);
}

export struct Arena {
    // todo easing?
    NetPropIntegral<uint32_t, EasingMode::Linear, IntegralFormat::ULeb128> width{ 22300 };
    NetPropIntegral<uint32_t, EasingMode::Linear, IntegralFormat::ULeb128> height{ 22300 };
    NetPropIntegral<uint32_t, EasingMode::Linear, IntegralFormat::ULeb128> padding{ 200 };

    void decode(Reader &reader) {
        const auto state = reader.uleb128<uint64_t>();

        if (state & ArenaNetProps::Width) width.decode(reader);
        if (state & ArenaNetProps::Height) height.decode(reader);
        if (state & ArenaNetProps::Padding) padding.decode(reader);
    }
};
