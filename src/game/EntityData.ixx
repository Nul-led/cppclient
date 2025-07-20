module;

#include <algorithm>
#include <cstdint>
#include <cmath>

export module EntityData;

import NetProp;
import Reader;
import Color;
import Interpolation;
import Canvas;
import Absolutes;

enum class EntityNetProps : uint64_t {
    // byte 1
    X                   = 1 << 0,
    Y                   = 1 << 1,
    Rotation            = 1 << 2,
    Health              = 1 << 3,
    BBSide1             = 1 << 4,
    Score               = 1 << 5,
    AnimationDuration   = 1 << 6,
    // byte 2
    MaxHealth            = 1 << 7,
    Parent               = 1 << 8,
    BBType               = 1 << 9,
    BBSide2              = 1 << 10,
    Color                = 1 << 11,
    Z                    = 1 << 12,
    Flags                = 1 << 13,
    // byte 3
    Opacity              = 1 << 14,
    Name                 = 1 << 15,
    NameColor            = 1 << 16,
    ScoreColor           = 1 << 17,
    StrokeWidth          = 1 << 18,
    RotationOffset       = 1 << 19,
};

export inline bool operator&(const uint64_t lhs, const EntityNetProps rhs) {
    return lhs & static_cast<uint64_t>(rhs);
}

export enum class EntityFlags : uint8_t {
    IsRotationAbsolute      = 1 << 0, // is the rotation is absolute / not relative to the parents
    IsTrapezoid             = 1 << 1, // render a rectangle as a isosceles trapezoid
    IsStar                  = 1 << 2, // render a regular polygon as a star / regular polygram
    IsInvulnerable          = 1 << 3, // render the invulnerability filter flash on the entity
    IsAboveParent           = 1 << 4, // render above parent instead of below, overrides Z coordinate
    IsOnMinimap             = 1 << 5, // render entity to the minimap
    DoShapeRotation         = 1 << 6, // simulates passive shape rotation on the client, also sets random start rotation when this is toggled during spawn tick
};

inline uint8_t operator&(const uint8_t lhs, const EntityNetProps rhs) {
    return lhs & static_cast<uint8_t>(rhs);
}

export struct EntityData {
    uint64_t id;
    uint64_t spawnTick;

    explicit EntityData(const uint64_t id, const uint64_t spawnTick) : id(id), spawnTick(spawnTick) {}

    // state byte 1
    NetPropArithmetic<float, EasingMode::Linear> x{ 0.0f };
    NetPropArithmetic<float, EasingMode::Linear> y{ 0.0f };
    NetPropArithmetic<float, EasingMode::Linear> rotation{ 0.0f, M_PI * 2.0f };
    NetPropIntegral<uint64_t, EasingMode::Linear, IntegralFormat::ULeb128> health{ 0 }; // TODO might look nice with some out/inout easing
    NetPropArithmetic<float, EasingMode::Linear> bbSide1{ 50.0f }; // TODO add easing?, also is this default statistically a good idea?
    NetPropIntegral<uint64_t, EasingMode::Linear, IntegralFormat::ULeb128> score{ 0 };
    NetProp<uint8_t> animationDuration{ 0 };

    // state byte 2
    NetPropIntegral<uint64_t, EasingMode::Linear, IntegralFormat::ULeb128> maxHealth{ 0 };
    NetPropIntegral<uint64_t, EasingMode::None, IntegralFormat::ULeb128> parent{ 0 };
    NetProp<uint8_t> bbType{ 1 };
    NetPropArithmetic<float, EasingMode::Linear> bbSide2{ 0.0f };
    NetProp<Color> color{ COLOR_FRIENDLY };
    NetPropIntegral<uint64_t, EasingMode::None, IntegralFormat::ULeb128> z{ 1 };
    NetProp<uint8_t> flags{ 0 };

    // state byte 3
    NetPropArithmetic<uint8_t, EasingMode::Linear> opacity{ 255 };
    NetPropString<StringFormat::NullTerminated> name{ "" };
    NetProp<Color> nameColor{ COLOR_WHITE };
    NetProp<Color> scoreColor{ COLOR_WHITE };
    NetPropArithmetic<uint8_t, EasingMode::Linear> strokeWidth{ 3 };
    NetPropArithmetic<float, EasingMode::Linear> rotationOffset{ 0.0f };

    Absolutes absolutes{};

    void decode(Reader &reader) {
        const auto state = reader.uleb128<uint64_t>();

        if (state & EntityNetProps::X) x.decode(reader);
        if (state & EntityNetProps::Y) y.decode(reader);
        if (state & EntityNetProps::Rotation) rotation.decode(reader);
        if (state & EntityNetProps::Health) health.decode(reader);
        if (state & EntityNetProps::BBSide1) bbSide1.decode(reader);
        if (state & EntityNetProps::Score) score.decode(reader);
        if (state & EntityNetProps::AnimationDuration) animationDuration.decode(reader);

        if (state & EntityNetProps::MaxHealth) maxHealth.decode(reader);
        if (state & EntityNetProps::Parent) parent.decode(reader);
        if (state & EntityNetProps::BBType) bbType.decode(reader);
        if (state & EntityNetProps::BBSide2) bbSide2.decode(reader);
        if (state & EntityNetProps::Color) color.decode(reader);
        if (state & EntityNetProps::Z) z.decode(reader);
        if (state & EntityNetProps::Flags) flags.decode(reader);

        if (state & EntityNetProps::Opacity) opacity.decode(reader);
        if (state & EntityNetProps::Name) name.decode(reader);
        if (state & EntityNetProps::NameColor) nameColor.decode(reader);
        if (state & EntityNetProps::ScoreColor) scoreColor.decode(reader);
        if (state & EntityNetProps::StrokeWidth) strokeWidth.decode(reader);
        if (state & EntityNetProps::RotationOffset) rotationOffset.decode(reader);
    }
};
