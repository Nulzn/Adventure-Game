#include "SaveSystem.h"
#include <fstream>
#include "nlohmann/json.hpp"
#include "GameState.h"

using json = nlohmann::json;

void save(const GameState& state, const std::string& filename) {
    json j;

    j["currentRoom"] = state.currentRoom;

    j["inventory"] = state.inventory;

    for (const auto& [roomId, room] : state.rooms) {
        j["rooms"][roomId]["locked"] = room.locked;
    }

    std::ofstream file(filename);

    if (!file) {
        throw std::runtime_error("Could not create save file.");
    }

    file << j.dump(4);
}