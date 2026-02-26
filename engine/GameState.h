#pragma once
#include <string>
#include <map>
#include <vector>
//Rum, föremål och spelarens tillstånd
struct Item {
public:
    std::string id;          //unikt id
    std::string displayname; //namn i text
    std::string commandName; //enstaka namn
    std::string description; //Beskrivning
    virtual ~Item() = default;
};

struct ReadableItem : public Item {
public:
    std::string text; //texten som utläses
};

struct Room {
public:
    std::string id;
    std::string name;
    std::string description;
    bool locked;
    std::map<std::string, std::string> requiredKeys;

    // Map of all the 
    std::map<std::string, std::string> exits;
    std::vector<std::string> itemIds;
    virtual ~Room() = default;
};

struct GameState {
    std::string currentRoom;
    std::map<std::string, Room> rooms;

    std::vector<std::string> inventory;

    bool doorUnlocked = false;
    bool hasWon = false;
};