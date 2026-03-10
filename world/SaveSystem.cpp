#include <fstream>
#include "SaveSystem.h"
#include "nlohmann/json.hpp"
#include "engine/GameState.h"

using json = nlohmann::json;

void save(const GameState& state, const std::string& filename) {
    json j;
    nlohmann::json inventoryJson = nlohmann::json::array();

    j["currentRoom"] = state.currentRoom;

    for (const auto& item : state.inventory) {
        inventoryJson.push_back(item->id);
    };
    j["inventory"] = inventoryJson;

    for (const auto& [roomId, room] : state.rooms) {
        j["rooms"][roomId]["locked"] = room.locked;
    }

    std::ofstream file(filename);

    if (!file) {
        throw std::runtime_error("Could not create save file.");
    }

    file << j.dump(4);
}