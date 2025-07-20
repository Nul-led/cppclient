module;

#include <vector>

export module TankData;

using TankId = uint64_t;

export struct ChildDefinition {
    float x;
    float y;

};

export struct TankDefinition {
    TankId id;
    std::string name;

};

export struct TankData {
    std::vector<TankId> upgradeIds;
    std::vector<TankDefinition> definitions;
};

