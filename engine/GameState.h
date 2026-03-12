#pragma once
#include <string>
#include <map>
#include <vector>
#include<items/item.h>
//Rum och spelarens tillstånd


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