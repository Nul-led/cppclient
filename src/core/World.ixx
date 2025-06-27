module;

#include <cstdint>
#include <map>
#include <math.h>
#include <ranges>
#include <string>

export module World;

import Arena;
import Camera;
import Entity;
import Reader;
import Viewport;



export struct World {
    static World *Remote() {
        static World REMOTE_WORLD{};
        return &REMOTE_WORLD;
    }

    uint64_t tick = 0;

    Arena arena{};
    Camera camera{};

    std::map<uint32_t, Entity> entities{};

    uint32_t entityDeletionsCount = 0;
    uint32_t entityUpdatesCount = 0;
    uint32_t serverEntitiesCount = 0;

    explicit World() = default;

    void decode(Reader &reader) {


        // For example, this is where you might set these counters:
        // this->entityUpdatesCount = ...
        // this->entityDeletionsCount = ...
    }

    // Creates an entity with the next available ID.
    Entity& createEntity() {
        if (entities.size() >= UINT32_MAX) {
            throw std::runtime_error("Cannot create new entity: maximum number of entities reached.");
        }

        uint32_t entityId = 0;
        while (entities.contains(entityId)) ++entityId;
        return createEntityAt(entityId);
    }

    // Creates an entity with a specific ID.
    Entity& createEntityAt(uint32_t id) {
        if (entities.contains(id)) {
            throw std::runtime_error("Entity with ID " + std::to_string(id) + " already exists.");
        }
        entities.emplace(id, Entity(id, tick));
        return entities.find(id)->second;
    }

    // Gets an entity by its ID.
    Entity& getEntity(const uint32_t id) {
        return entities.at(id);
    }

    // Gets or creates an entity at ID.
    Entity& getOrCreateEntity(const uint32_t id) {
        if (containsEntity(id)) {
            return getEntity(id);
        }
        return createEntityAt(id);
    }

    // Returns the state of existence of an entity by its ID.
    [[nodiscard]] bool containsEntity(const uint32_t id) const {
        return entities.contains(id);
    }

    // Deletes an entity by its ID.
    // Returns true if the entity was found and deleted, false otherwise.
    bool deleteEntity(const uint32_t id) {
        return entities.erase(id) > 0;
    }

    // Clears all entities.
    void clearEntities() {
        entities.clear();
    }

    // Returns the current number of entities in the world.
    [[nodiscard]] uint32_t getEntityCount() const {
        return static_cast<uint32_t>(entities.size());
    }

    // Returns the number of entities in the server's world if available.
    [[nodiscard]] uint32_t getServerEntityCount() const {
        return serverEntitiesCount;
    }

    // Returns the number of entity updates from the last decode.
    [[nodiscard]] uint32_t getEntityUpdatesCount() const {
        return entityUpdatesCount;
    }

    // Returns the number of entity deletions from the last decode.
    [[nodiscard]] uint32_t getEntityDeletionsCount() const {
        return entityDeletionsCount;
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
        if (entity.absolutesCachedFrame == Viewport::frameCount) return;
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
            if (entity.flags.getRenderValue() & static_cast<uint8_t>(EntityFlags::IsAnimationYAxis)) {
                y += entity.getAnimationOffset(Viewport::renderTime);
            } else {
                x += entity.getAnimationOffset(Viewport::renderTime);
            }
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

    }
};
