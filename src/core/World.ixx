module;

#include <cstdint>
#include <map>
#include <cmath>
#include <ranges>
#include <string>

export module World;

import ArenaData;
import CameraData;
import EntityData;
import DeathData;
import IndicatorData;
import AttributeData;
import ScoreboardData;
import TankData;
import Reader;

export struct World {
    uint64_t tick = 0;

    // singletons
    ArenaData arena{};
    CameraData camera{};
    AttributeData attributes{};
    DeathData death{};
    IndicatorData indicators{};
    ScoreboardData scoreboard{};
    TankData tanks{};

    std::map<uint64_t, EntityData> entities{};

    explicit World() = default;

    // Creates an entity with the next available ID.
    EntityData& createEntity() {
        uint64_t entityId = 0;
        while (entities.contains(entityId)) ++entityId;
        return createEntityAt(entityId);
    }

    // Creates an entity with a specific ID.
    EntityData& createEntityAt(uint64_t id) {
        if (entities.contains(id)) {
            throw std::runtime_error("Entity with ID " + std::to_string(id) + " already exists.");
        }
        entities.emplace(id, EntityData(id, tick));
        return entities.find(id)->second;
    }

    // Gets an entity by its ID.
    EntityData& getEntity(const uint64_t id) {
        return entities.at(id);
    }

    // Gets or creates an entity at ID.
    EntityData& getOrCreateEntity(const uint64_t id) {
        if (containsEntity(id)) {
            return getEntity(id);
        }
        return createEntityAt(id);
    }

    // Returns the state of existence of an entity by its ID.
    [[nodiscard]] bool containsEntity(const uint64_t id) const {
        return entities.contains(id);
    }

    // Deletes an entity by its ID.
    // Returns true if the entity was found and deleted, false otherwise.
    bool deleteEntity(const uint64_t id) {
        return entities.erase(id) > 0;
    }

    // Clears all entities.
    void clearEntities() {
        entities.clear();
    }

    // Returns the current number of entities in the world.
    [[nodiscard]] size_t getEntityCount() const {
        return entities.size();
    }

    // Returns the current tick
    [[nodiscard]] uint64_t getTick() const {
        return tick;
    }

    void calculateEntityAbsolutes() {
        for (auto &val: entities | std::views::values) {
            val.absolutes.calculateAbsolutes();
        }
    }
};
