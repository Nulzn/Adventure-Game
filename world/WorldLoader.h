#pragma once

#include <string>
#include "engine/GameState.h"
#include "nlohmann/json.hpp"

class WorldLoader {
public:
    static GameState load(const std::string& filename);
private:
    static Room parseRoom(const nlohmann::json& roomJson);
    static std::shared_ptr<Item> parseItem(const nlohmann::json& itemJson);
    static void validate(const GameState& state);
    static void parseInventory(GameState& state, const nlohmann::json& inventoryJson);
};