#include<iostream>
#include<GameEngine.h>
#include<GameState.h>
#include<WorldLoader.h>
#include<CommandParser.h>
#include<BasicNarrator.h>

int main() {
    GameState state = WorldLoader::load("world.json");

    std::cout << state.rooms[state.currentRoom].description << std::endl;

    return 0;
}