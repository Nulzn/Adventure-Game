#include <fstream>
#include <stdexcept>
#include <memory>

#include "WorldLoader.h"
#include "engine/GameState.h"
#include "nlohmann/json.hpp"
#include "items/item.h"

GameState WorldLoader::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Could not open file!");

    nlohmann::json j;
    file >> j;

    GameState state;
    state.currentRoom = j["startRoom"];

    for (const auto& roomJson : j["rooms"]) {
        Room room = parseRoom(roomJson);
        state.rooms[room.id] = room;
    }

    validate(state);
    return state;
}

std::shared_ptr<Item> WorldLoader::parseItem(const nlohmann::json& itemJson) {
    std::string type = itemJson.value("type", "Item");

    if (type == "KeyItem") {
        
        auto item = std::make_shared<KeyItem>();
        item->id             = itemJson.value("id", "");
        item->displayName    = itemJson.value("displayName", "");
        item->commandName    = itemJson.value("commandName", "");
        item->description    = itemJson.value("description", "");
        item->unlocksRoomId  = itemJson.value("unlocksRoomId", "");
        return item;

    } else if (type == "ReadableItem") {

        auto item = std::make_shared<ReadableItem>();
        item->id             = itemJson.value("id", "");
        item->displayName    = itemJson.value("displayName", "");
        item->commandName    = itemJson.value("commandName", "");
        item->description    = itemJson.value("description", "");
        item->text           = itemJson.value("text", "");
        return item;
    } else if (type == "ContainerItem") {

        auto item = std::make_shared<ContainerItem>();
        item->id             = itemJson.value("id", "");
        item->displayName    = itemJson.value("displayName", "");
        item->commandName    = itemJson.value("commandName", "");
        item->description    = itemJson.value("description", "");
        item->code           = itemJson.value("code", "");
        item->locked         = itemJson.value("locked", true);

        if (itemJson.contains("items")) {
            for (const auto& innerItemJson : itemJson["items"]) {
                auto innerItem = parseItem(innerItemJson);
                if (innerItem) {
                    item->contents[innerItem->id] = innerItem;
                }
            }
        }
        return item;

    } else if (type == "ConsumableItem") {
        auto item = std::make_shared<ConsumableItem>();
        item->id             = itemJson.value("id", "");
        item->displayName    = itemJson.value("displayName", "");
        item->commandName    = itemJson.value("commandName", "");
        item->description    = itemJson.value("description", "");
        item->effect         = itemJson.value("effect", "");
        item->consumed       = itemJson.value("consumed", false);
        return item;
    }


    auto item = std::make_shared<Item>();
    item->id             = itemJson.value("id", "");
    item->displayName    = itemJson.value("displayName", "");
    item->commandName    = itemJson.value("commandName", "");
    item->description    = itemJson.value("description", "");
    return item;
}

Room WorldLoader::parseRoom(const nlohmann::json& roomJson) {
    Room room;
    room.id = roomJson.value("id", "");
    room.name = roomJson.value("displayName", "");
    room.description = roomJson.value("description", "");

    for (const auto& itemJson : roomJson["items"]) {
        auto item = parseItem(itemJson);
        room.items[item->id] = item;
    }

    for (auto& ex : roomJson["exits"].items()) {
        room.exits[ex.key()] = ex.value();
    }

    if (roomJson.contains("locked")) room.locked = roomJson.value("locked", true);

    if (roomJson.contains("keys")) {
        for (auto& key : roomJson["keys"].items()) {
            room.requiredKeys[key.key()] = key.value();
        }
    }

    return room;
}

void WorldLoader::validate(const GameState& state) {
    if (state.rooms.find(state.currentRoom) == state.rooms.end()) {
        throw std::runtime_error("startRoom does not exist in rooms.");
    }

    for (const auto& [roomId, room] : state.rooms) {
        for (const auto& [direction, targetRoomId] : room.exits) {
            if (state.rooms.find(targetRoomId) == state.rooms.end()) {
                throw std::runtime_error("Room '" + roomId + "' has exit to missing room '" + targetRoomId + "'.");
            }
        }

        for (const auto& [itemId, itemPtr] : room.items) {
            if (auto keyItem = std::dynamic_pointer_cast<KeyItem>(itemPtr)) {
                if (!keyItem->unlocksRoomId.empty() && state.rooms.find(keyItem->unlocksRoomId) == state.rooms.end()) {
                    throw std::runtime_error("Item '" + keyItem->id + "' unlocks missing room '" + keyItem->unlocksRoomId + "'.");
                }
            }
        }
    }
}