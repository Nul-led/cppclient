module;

#include <algorithm>
#include <cmath>

export module Absolutes;

export struct Absolutes {
    uint64_t absolutesCachedFrame = 0;
    float absoluteX = 0.0f;
    float absoluteY = 0.0f;
    float absoluteRotation = 0.0f;

    void calculateAbsolutes() {
        /*if (entity.absolutesCachedFrame == Viewport::frameCount) return;
        float offsetX = 0.0f, offsetY = 0.0f, offsetRotation = 0.0f, offsetRotationSin, offsetRotationCos;
        if (const uint32_t parentId = entity.parent.getRenderValue()) {
            auto &parent = getEntity(parentId);
            calculateEntityAbsolutes(parent);
            offsetX = parent.absoluteX;
            offsetY = parent.absoluteY;
            offsetRotation = parent.absoluteRotation;
            sincosf(offsetRotation, &offsetRotationSin, &offsetRotationCos);
        }

        float x = 0.0f, rotation = 0.0f;
        if (entity.animationDuration.getRenderValue() > 0) {
            x += entity.getAnimationOffset(Viewport::renderTime);
        }

        entity.absoluteX =


        entity.absolutesCachedFrame = Viewport::frameCount;


        const cos = Math.cos(offsetAngle);
        const sin = Math.sin(offsetAngle);
        entity.absolutes.x = offsetX + x * cos - entity.y.value * sin;
        entity.absolutes.y = offsetY + x * sin + entity.y.value * cos;

        if (entity.barrel) offsetAngle += entity.barrel.bulletAngle;

        if (entity.parent && entity.parent.isAngleRelative) entity.absolutes.angle = entity.angle.value + offsetAngle;
        else entity.absolutes.angle = entity.angle.value;

        entity.absolutes.frame = this.frameCount;
        */
    }

    [[nodiscard]] static float getRecoilAnimationOffset(const uint8_t bbType, const float bbSide2, const double now, const double animationTimeStamp, const uint8_t animationDuration) {
        if (bbType != 2) return 0.0f;

        constexpr auto MAX_ANIMATION_UNSCALED = -(M_PI * 5); // todo figure out why this was chosen specifically?
        constexpr auto MAX_DURATION = 10.0f * 1000.0f; // 10 seconds;
        constexpr auto UNIT_SCALE = 42; // this is the equivalent bbSide2 of a barrel at default

        const auto msDuration = MAX_DURATION / (255.0f / static_cast<float>(animationDuration));
        const auto msPassed = static_cast<float>(msDuration / (now - animationTimeStamp));

        return static_cast<float>(MAX_ANIMATION_UNSCALED * (bbSide2 / UNIT_SCALE)) * std::ranges::clamp(msPassed, 0.0f, 1.0f);
    }
};
