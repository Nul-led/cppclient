module;

#include <cstdint>
#include <map>
#include <cmath>
#include <ranges>
#include <string>

export module World;

import Arena;
import Camera;
import Entity;
import Reader;

export struct World {
    uint64_t tick = 0;
    Arena arena{};
    Camera camera{};
    std::map<uint16_t, Entity> entities{};

    explicit World() = default;

    // Creates an entity with the next available ID.
    Entity& createEntity() {
        if (entities.size() >= UINT32_MAX) {
            throw std::runtime_error("Cannot create new entity: maximum number of entities reached.");
        }

        uint16_t entityId = 0;
        while (entities.contains(entityId)) ++entityId;
        return createEntityAt(entityId);
    }

    // Creates an entity with a specific ID.
    Entity& createEntityAt(uint16_t id) {
        if (entities.contains(id)) {
            throw std::runtime_error("Entity with ID " + std::to_string(id) + " already exists.");
        }
        entities.emplace(id, Entity(id, tick));
        return entities.find(id)->second;
    }

    // Gets an entity by its ID.
    Entity& getEntity(const uint16_t id) {
        return entities.at(id);
    }

    // Gets or creates an entity at ID.
    Entity& getOrCreateEntity(const uint16_t id) {
        if (containsEntity(id)) {
            return getEntity(id);
        }
        return createEntityAt(id);
    }

    // Returns the state of existence of an entity by its ID.
    [[nodiscard]] bool containsEntity(const uint16_t id) const {
        return entities.contains(id);
    }

    // Deletes an entity by its ID.
    // Returns true if the entity was found and deleted, false otherwise.
    bool deleteEntity(const uint16_t id) {
        return entities.erase(id) > 0;
    }

    // Clears all entities.
    void clearEntities() {
        entities.clear();
    }

    // Returns the current number of entities in the world.
    [[nodiscard]] uint16_t getEntityCount() const {
        return static_cast<uint16_t>(entities.size());
    }

    // Returns the current tick
    [[nodiscard]] uint64_t getTick() const {
        return tick;
    }

    void calculateAbsolutes() {
        for (auto &val: entities | std::views::values) {
            calculateEntityAbsolutes(val);
        }
    }

    void calculateEntityAbsolutes(Entity& entity) {
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
};
