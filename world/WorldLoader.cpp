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
        room.name = roomJson["displayName"];
        room.description = roomJson["description"];
        //room.itemIds = roomJson["items_ids"];

        for (const auto& itemJson : roomJson["items"]) {
            std::string type = itemJson["type"];

            if (type == "KeyItem") {
                auto item = std::make_shared<KeyItem>();
                item->id             = itemJson["id"];
                item->displayName    = itemJson["displayName"];
                item->commandName    = itemJson["commandName"];
                item->description    = itemJson["description"];
                item->unlocksRoomId  = itemJson["unlocksRoomId"];
                room.items[item->id] = item;

            } else if (type == "ReadableItem") {
                auto item = std::make_shared<ReadableItem>();
                item->id             = itemJson["id"];
                item->displayName    = itemJson["displayName"];
                item->commandName    = itemJson["commandName"];
                item->description    = itemJson["description"];
                item->text           = itemJson["text"];
                room.items[item->id] = item;
            }
        }

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