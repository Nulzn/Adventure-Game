#include<iostream>
#include<engine/GameEngine.h>
#include<engine/GameState.h>
#include<world/WorldLoader.h>
#include<parser/CommandParser.h>
#include<narration/BasicNarrator.h>

int main() {
    WorldLoader loader;
    GameState state = loader.load("data/world.json");

    std::cout << state.rooms[state.currentRoom].description << std::endl;

    return 0;
}