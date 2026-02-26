#include<string>
#include<iostream>
#include<GameState.h>
#include<fstream>
#include<nlohmann/json.hpp>

class WorldLoader {
public:
    GameState load(const std::string& filename) {

        // Trying to open the file
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Could not open file!");
        };


        nlohmann::json j;
        file >> j;

        GameState state;
        state.currentRoom = j["startRoom"];

        // Iterating over the different rooms located in the json map file
        for (const auto& roomJson : j["rooms"]) {
            Room room;
            room.id = roomJson["id"];
            room.description = roomJson["description"];

            for (auto& exit : roomJson["exits"].items()) {
                room.exits[exit.key()] = exit.value();
            };

            if (roomJson.contains("locked")) {
                room.locked = roomJson["locked"];
            };

            if (roomJson.contains("keys")) {
                for (auto& key : roomJson["keys"].items()) {
                    room.requiredKeys[key.key()] = key.value();
                }
            };

            state.rooms[room.id] = room;
        };
    };
};