module;

#include <algorithm>
#include <cmath>

export module Interpolation;

export enum class EasingMode {
    None,
    Linear,
    EaseInSine,
    EaseOutSine,
    EaseInOutSine,
    EaseInQuad,
    EaseOutQuad,
    EaseInOutQuad,
    EaseInCubic,
    EaseOutCubic,
    EaseInOutCubic,
    EaseInQuart,
    EaseOutQuart,
    EaseInOutQuart,
    EaseInQuint,
    EaseOutQuint,
    EaseInOutQuint,
    EaseInExpo,
    EaseOutExpo,
    EaseInOutExpo,
    EaseInCirc,
    EaseOutCirc,
    EaseInOutCirc,
    EaseInBack,
    EaseOutBack,
    EaseInOutBack,
    EaseInElastic,
    EaseOutElastic,
    EaseInOutElastic,
    EaseInBounce,
    EaseOutBounce,
    EaseInOutBounce,
};

namespace EasingFunctions {
    // --- Linear ---
    inline double linear(const double t) { return t; }

    // --- Sine ---
    inline double easeInSine(const double t) { return 1.0f - std::cos(t * M_PI / 2.0f); }
    inline double easeOutSine(const double t) { return std::sin(t * M_PI / 2.0f); }
    inline double easeInOutSine(const double t) { return -(std::cos(M_PI * t) - 1.0f) / 2.0f; }

    // --- Quadratic ---
    inline double easeInQuad(const double t) { return t * t; }
    inline double easeOutQuad(const double t) { return 1.0f - (1.0f - t) * (1.0f - t); }
    inline double easeInOutQuad(const double t) { return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f; }

    // --- Cubic ---
    inline double easeInCubic(const double t) { return t * t * t; }
    inline double easeOutCubic(const double t) { return 1.0f - std::pow(1.0f - t, 3.0f); }
    inline double easeInOutCubic(const double t) { return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f; }

    // --- Quartic ---
    inline double easeInQuart(const double t) { return t * t * t * t; }
    inline double easeOutQuart(const double t) { return 1.0f - std::pow(1.0f - t, 4.0f); }
    inline double easeInOutQuart(const double t) { return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f; }

    // --- Quintic ---
    inline double easeInQuint(const double t) { return t * t * t * t * t; }
    inline double easeOutQuint(const double t) { return 1.0f - std::pow(1.0f - t, 5.0f); }
    inline double easeInOutQuint(const double t) { return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f; }

    // --- Exponential ---
    inline double easeInExpo(const double t) { return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f); }
    inline double easeOutExpo(const double t) { return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t); }
    inline double easeInOutExpo(const double t) {
        if (t == 0.0f || t == 1.0f) return t;
        return t < 0.5f ? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f
                        : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
    }

    // --- Circular ---
    inline double easeInCirc(const double t) { return 1.0f - std::sqrt(1.0f - std::pow(t, 2.0f)); }
    inline double easeOutCirc(const double t) { return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f)); }
    inline double easeInOutCirc(const double t) {
        return t < 0.5f ? (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f
                        : (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
    }

    // --- Back ---
    constexpr double c1 = 1.70158f;
    constexpr double c2 = c1 * 1.525f;
    constexpr double c3 = c1 + 1.0f;
    inline double easeInBack(const double t) { return c3 * t * t * t - c1 * t * t; }
    inline double easeOutBack(const double t) { return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f); }
    inline double easeInOutBack(const double t) {
        return t < 0.5f ? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
                        : (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
    }

    // --- Elastic ---
    constexpr double c4 = (2.0f * M_PI) / 3.0f;
    constexpr double c5 = (2.0f * M_PI) / 4.5f;
    inline double easeInElastic(const double t) {
        if (t == 0.0f || t == 1.0f) return t;
        return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
    }
    inline double easeOutElastic(const double t) {
        if (t == 0.0f || t == 1.0f) return t;
        return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
    }
    inline double easeInOutElastic(const double t) {
        if (t == 0.0f || t == 1.0f) return t;
        return t < 0.5f ? -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f
                        : (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
    }

    // --- Bounce ---
    inline double easeOutBounce(double t) {
        constexpr double n1 = 7.5625f;
        if (constexpr double d1 = 2.75f; t < 1.0f / d1) {
            return n1 * t * t;
        } else if (t < 2.0f / d1) {
            t -= 1.5f / d1;
            return n1 * t * t + 0.75f;
        } else if (t < 2.5f / d1) {
            t -= 2.25f / d1;
            return n1 * t * t + 0.9375f;
        } else {
            t -= 2.625f / d1;
            return n1 * t * t + 0.984375f;
        }
    }
    inline double easeInBounce(const double t) { return 1.0f - easeOutBounce(1.0f - t); }
    inline double easeInOutBounce(const double t) {
        return t < 0.5f ? (1.0f - easeOutBounce(1.0f - 2.0f * t)) / 2.0f
                        : (1.0f + easeOutBounce(2.0f * t - 1.0f)) / 2.0f;
    }
}

template<EasingMode Easing>
double getEasedProgress(double progress) {
    progress = std::clamp<double>(progress, 0.0f, 1.0f);
    if constexpr (Easing == EasingMode::None) {
        progress = 1.0f;
    } else if constexpr (Easing == EasingMode::Linear) {
        progress = EasingFunctions::linear(progress);
    } else if constexpr (Easing == EasingMode::EaseInSine) {
        progress = EasingFunctions::easeInSine(progress);
    } else if constexpr (Easing == EasingMode::EaseOutSine) {
        progress = EasingFunctions::easeOutSine(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutSine) {
        progress = EasingFunctions::easeInOutSine(progress);
    } else if constexpr (Easing == EasingMode::EaseInQuad) {
        progress = EasingFunctions::easeInQuad(progress);
    } else if constexpr (Easing == EasingMode::EaseOutQuad) {
        progress = EasingFunctions::easeOutQuad(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutQuad) {
        progress = EasingFunctions::easeInOutQuad(progress);
    } else if constexpr (Easing == EasingMode::EaseInCubic) {
        progress = EasingFunctions::easeInCubic(progress);
    } else if constexpr (Easing == EasingMode::EaseOutCubic) {
        progress = EasingFunctions::easeOutCubic(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutCubic) {
        progress = EasingFunctions::easeInOutCubic(progress);
    } else if constexpr (Easing == EasingMode::EaseInQuart) {
        progress = EasingFunctions::easeInQuart(progress);
    } else if constexpr (Easing == EasingMode::EaseOutQuart) {
        progress = EasingFunctions::easeOutQuart(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutQuart) {
        progress = EasingFunctions::easeInOutQuart(progress);
    } else if constexpr (Easing == EasingMode::EaseInQuint) {
        progress = EasingFunctions::easeInQuint(progress);
    } else if constexpr (Easing == EasingMode::EaseOutQuint) {
        progress = EasingFunctions::easeOutQuint(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutQuint) {
        progress = EasingFunctions::easeInOutQuint(progress);
    } else if constexpr (Easing == EasingMode::EaseInExpo) {
        progress = EasingFunctions::easeInExpo(progress);
    } else if constexpr (Easing == EasingMode::EaseOutExpo) {
        progress = EasingFunctions::easeOutExpo(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutExpo) {
        progress = EasingFunctions::easeInOutExpo(progress);
    } else if constexpr (Easing == EasingMode::EaseInCirc) {
        progress = EasingFunctions::easeInCirc(progress);
    } else if constexpr (Easing == EasingMode::EaseOutCirc) {
        progress = EasingFunctions::easeOutCirc(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutCirc) {
        progress = EasingFunctions::easeInOutCirc(progress);
    } else if constexpr (Easing == EasingMode::EaseInBack) {
        progress = EasingFunctions::easeInBack(progress);
    } else if constexpr (Easing == EasingMode::EaseOutBack) {
        progress = EasingFunctions::easeOutBack(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutBack) {
        progress = EasingFunctions::easeInOutBack(progress);
    } else if constexpr (Easing == EasingMode::EaseInElastic) {
        progress = EasingFunctions::easeInElastic(progress);
    } else if constexpr (Easing == EasingMode::EaseOutElastic) {
        progress = EasingFunctions::easeOutElastic(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutElastic) {
        progress = EasingFunctions::easeInOutElastic(progress);
    } else if constexpr (Easing == EasingMode::EaseInBounce) {
        progress = EasingFunctions::easeInBounce(progress);
    } else if constexpr (Easing == EasingMode::EaseOutBounce) {
        progress = EasingFunctions::easeOutBounce(progress);
    } else if constexpr (Easing == EasingMode::EaseInOutBounce) {
        progress = EasingFunctions::easeInOutBounce(progress);
    }
    return progress;
}

export template<typename T, EasingMode Easing = EasingMode::Linear>
T interpolate(T start, T end, double progress) {
    progress = getEasedProgress<Easing>(progress);
    return static_cast<T>((1.0f - progress) * start + progress * end);
}

// Used to interpolate cyclic values that are confined to a specific interval [0; period] (such as angles, radians, hue, ...)
export template<typename T, EasingMode Easing = EasingMode::Linear>
T interpolateCyclic(T start, T end, double progress, T period) {
    progress = getEasedProgress<Easing>(progress);
    T delta = end - start;
    const T halfPeriod = period * 0.5f;

    if (delta > halfPeriod) {
        delta -= period;
    } else if (delta < -halfPeriod) {
        delta += period;
    }

    T result = start + delta * progress;

    if constexpr(std::is_floating_point_v<T>) {
        result = std::fmod(result, period);
    } else {
        result %= period;
    }

    if (result < 0) {
        result += period;
    }

    return result;
}