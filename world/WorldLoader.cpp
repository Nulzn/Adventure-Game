#include <fstream>
#include <stdexcept>

#include "WorldLoader.h"
#include "engine/GameState.h"
#include "nlohmann/json.hpp"

GameState WorldLoader::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Could not open file!");

    nlohmann::json j;
    file >> j;

    GameState state;
    state.currentRoom = j["startRoom"];

    for (const auto& roomJson : j["rooms"]) {
        Room room;
        room.id = roomJson["id"];
        room.description = roomJson["description"];

        for (auto& ex : roomJson["exits"].items()) {
            room.exits[ex.key()] = ex.value();
        }

        if (roomJson.contains("locked")) room.locked = roomJson["locked"];

        if (roomJson.contains("keys")) {
            for (auto& key : roomJson["keys"].items()) {
                room.requiredKeys[key.key()] = key.value();
            }
        }

        state.rooms[room.id] = room;
    }

    return state;
}