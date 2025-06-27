module;

#include <string>
#include <type_traits>

export module NetProp;

import Interpolation;
import Viewport;
import Reader;

export enum class StringFormat {
    Short,
    Variable,
    NullTerminated,
};

export enum class IntegralFormat {
    Trivial,
    ULeb128,
    SLeb128,
    ZLeb128,
};

export template<typename T>
struct NetProp {
    T currentServerValue{};
    T previousServerValue{};
    double currentTimeStamp = 0.0;
    double previousTimeStamp = 0.0;
    bool hasBeenUpdated = false;

    explicit NetProp(T defaultValue)
        : currentServerValue(std::move(defaultValue)), previousServerValue(currentServerValue) {
    }

    virtual ~NetProp() = default;

    void updateValue(T newValue) {
        previousServerValue = currentServerValue;
        previousTimeStamp = currentTimeStamp;

        currentServerValue = std::move(newValue);
        currentTimeStamp = Viewport::now;

        hasBeenUpdated = true;
    }

    [[nodiscard]] virtual T getRenderValue() const {
        return currentServerValue;
    }

    virtual void decode(Reader &reader) {
        updateValue(reader.trivial<T>());
    }
};

export template<StringFormat Format>
struct NetPropString final : NetProp<std::string> {
    using NetProp::NetProp;

    void decode(Reader &reader) override {
        if constexpr (Format == StringFormat::NullTerminated) {
            updateValue(reader.ntString());
        } else if constexpr (Format == StringFormat::Short) {
            updateValue(reader.shortString());
        } else if constexpr (Format == StringFormat::Variable) {
            updateValue(reader.varString());
        }
    }
};

export template<typename T, EasingMode InterpolationMode = EasingMode::None>
    requires std::is_arithmetic_v<T>
struct NetPropArithmetic : NetProp<T> {
    bool isCyclic = false;
    T period{};

    explicit NetPropArithmetic(T defaultValue) : NetProp<T>(std::move(defaultValue)) {}
    explicit NetPropArithmetic(T defaultValue, T period) : NetProp<T>(std::move(defaultValue)), isCyclic(true), period(period) {}

    T getRenderValue() const override {
        if (InterpolationMode == EasingMode::None || !this->hasBeenUpdated) {
            return this->currentServerValue;
        }

        // If not enough history or timestamps are identical, no interpolation possible/needed.
        if (this->previousTimeStamp >= this->currentTimeStamp) {
            return this->currentServerValue;
        }

        double renderTime = Viewport::renderTime;

        // Clamp renderTime to the interpolation interval
        if (renderTime <= this->previousTimeStamp) {
            return this->previousServerValue;
        }

        if (renderTime >= this->currentTimeStamp) {
            return this->currentServerValue;
        }

        float alpha = (renderTime - this->previousTimeStamp) / (this->currentTimeStamp - this->previousTimeStamp);

        if (isCyclic) {
            return static_cast<T>(interpolateCyclic<double, InterpolationMode>(static_cast<double>(this->previousServerValue), static_cast<double>(this->currentServerValue), alpha, period));
        }

        return static_cast<T>(interpolate<double, InterpolationMode>(static_cast<double>(this->previousServerValue), static_cast<double>(this->currentServerValue), alpha));
    }
};

export template<typename T, EasingMode InterpolationMode = EasingMode::None, IntegralFormat Format =
        IntegralFormat::Trivial>
    requires std::is_integral_v<T>
struct NetPropIntegral final : NetPropArithmetic<T, InterpolationMode> {
    static_assert(Format != IntegralFormat::ULeb128 || std::is_unsigned_v<T>,
                  "IntegralFormat::ULeb128 requires an unsigned integral type");
    static_assert(Format != IntegralFormat::SLeb128 || std::is_signed_v<T>,
                  "IntegralFormat::SLeb128 requires a signed integral type");
    static_assert(Format != IntegralFormat::ZLeb128 || std::is_signed_v<T>,
                  "IntegralFormat::ZLeb128 requires a signed integral type");

    using NetPropArithmetic<T, InterpolationMode>::NetPropArithmetic;

    void decode(Reader &reader) override {
        if constexpr(Format == IntegralFormat::ULeb128) {
            this->updateValue(reader.uleb128<T>());
        } else if constexpr(Format == IntegralFormat::SLeb128) {
            this->updateValue(reader.sleb128<T>());
        } else if constexpr(Format == IntegralFormat::ZLeb128) {
            this->updateValue(reader.zleb128<T>());
        } else if constexpr(Format == IntegralFormat::Trivial) {
            this->updateValue(reader.trivial<T>());
        }
    }
};

