#include "SaveSystem.h"
#include "items/item.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <memory>
#include <stdexcept>

using json = nlohmann::json;



static json itemToJson(const std::shared_ptr<Item>& item) {
    json j;
    j["id"] = item->id;
    j["displayName"] = item->displayName;
    j["commandName"] = item->commandName;
    j["description"] = item->description;

    if (auto keyItem = std::dynamic_pointer_cast<KeyItem>(item)) {
        j["type"] = "KeyItem";
        j["unlocksRoomId"] = keyItem->unlocksRoomId;
    } else if (auto readableItem = std::dynamic_pointer_cast<ReadableItem>(item)) {
        j["type"] = "ReadableItem";
        j["text"] = readableItem->text;
    } else if (auto containerItem = std::dynamic_pointer_cast<ContainerItem>(item)) {
        j["type"] = "ContainerItem";
    } else if (auto consumableItem = std::dynamic_pointer_cast<ConsumableItem>(item)) {
        j["type"] = "ConsumableItem";
    } else {
        j["type"] = "Item";
    }

    return j;
}

static std::shared_ptr<Item> itemFromJson(const json& itemJson) {
    std::string type = itemJson.value("type", "Item");

    if (type == "KeyItem") {
        auto item = std::make_shared<KeyItem>();
        item->id = itemJson.value("id", "");
        item->displayName = itemJson.value("displayName", "");
        item->commandName = itemJson.value("commandName", "");
        item->description = itemJson.value("description", "");
        item->unlocksRoomId = itemJson.value("unlocksRoomId", "");
        return item;
    }

    if (type == "ReadableItem") {
        auto item = std::make_shared<ReadableItem>();
        item->id = itemJson.value("id", "");
        item->displayName = itemJson.value("displayName", "");
        item->commandName = itemJson.value("commandName", "");
        item->description = itemJson.value("description", "");
        item->text = itemJson.value("text", "");
        return item;
    }

    if (type == "ContainerItem") {
        auto item = std::make_shared<ContainerItem>();
        item->id = itemJson.value("id", "");
        item->displayName = itemJson.value("displayName", "");
        item->commandName = itemJson.value("commandName", "");
        item->description = itemJson.value("description", "");
        item->code = itemJson.value("code", "");
        item->locked = itemJson.value("locked", true);
        return item;
    }

    if (type == "ConsumableItem") {
        auto item = std::make_shared<ConsumableItem>();
        item->id = itemJson.value("id", "");
        item->displayName = itemJson.value("displayName", "");
        item->commandName = itemJson.value("commandName", "");
        item->description = itemJson.value("description", "");
        item->effect = itemJson.value("effect", "");
        item->consumed = itemJson.value("consumed", false);
        return item;
    }

    auto item = std::make_shared<Item>();
    item->id = itemJson.value("id", "");
    item->displayName = itemJson.value("displayName", "");
    item->commandName = itemJson.value("commandName", "");
    item->description = itemJson.value("description", "");
    return item;
}

bool SaveSystem::saveExists(const std::string& fileName) {
    std::ifstream file(fileName);
    return file.good();
}

void SaveSystem::saveGame(GameState& state, const std::string& fileName) {
    json j;
    j["currentRoom"] = state.currentRoom;

    j["inventory"] = json::array();
    for (const auto& item : state.inventory) {
        j["inventory"].push_back(itemToJson(item));
    }

    j["rooms"] = json::array();
    for (const auto& [roomId, room] : state.rooms) {
        json roomJson;
        roomJson["id"] = room.id;
        roomJson["locked"] = room.locked;

        roomJson["items"] = json::array();
        for (const auto& [itemId, item] : room.items) {
            roomJson["items"].push_back(itemToJson(item));
        }

        j["rooms"].push_back(roomJson);
    }

    std::ofstream file(fileName);
    if (!file) {
        throw std::runtime_error("Could not open save file for writing.");
    }

    file << j.dump(4);
}

void SaveSystem::loadSave(GameState& state, const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file) {
        throw std::runtime_error("Could not open save file.");
    }

    json j;
    file >> j;

    state.currentRoom = j.value("currentRoom", state.currentRoom);

    state.inventory.clear();
    if (j.contains("inventory")) {
        for (const auto& itemJson : j["inventory"]) {
            state.inventory.push_back(itemFromJson(itemJson));
        }
    }

    if (j.contains("rooms")) {
        for (const auto& roomJson : j["rooms"]) {
            std::string roomId = roomJson.value("id", "");
            auto roomIt = state.rooms.find(roomId);

            if (roomIt == state.rooms.end()) {
                continue;
            }

            Room& room = roomIt->second;
            room.locked = roomJson.value("locked", room.locked);

            room.items.clear();
            if(roomJson.contains("items")) {
                for (const auto& itemJson : roomJson["items"]) {
                    auto item = itemFromJson(itemJson);
                    room.items[item->id] = item;
                }
            }
        }
    }
}