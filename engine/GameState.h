#pragma once
#include <string>
#include <map>
#include <vector>
#include<items/item.h>
//Rum, föremål och spelarens tillstånd
/*struct Item {
public:
    std::string id;          //unikt id
    std::string displayname; //namn i text
    std::string commandName; //enstaka namn
    std::string description; //Beskrivning
    virtual ~Item() = default;
};

struct KeyItem : public Item {
public:
    std::string unlocksRoomId;
};

struct ReadableItem : public Item {
public:
    std::string text; //texten som utläses
};
*/

struct Room {
public:
    std::string id;
    std::string name;
    std::string description;
    bool locked = false;
    std::map<std::string, std::string> requiredKeys;

    // Map of all the 
    std::map<std::string, std::string> exits;
    std::map<std::string, std::shared_ptr<Item>> items;
    virtual ~Room() = default;
};

struct GameState {
    std::string currentRoom;
    std::map<std::string, Room> rooms;
    std::vector<std::shared_ptr<Item>> inventory;//Shared pointer är broken för att faktiskt "flytta" på våra items :D

    bool doorUnlocked = false;
    bool hasWon = false;
};