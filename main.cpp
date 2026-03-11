#include<iostream>
#include<string>

#include "engine/GameEngine.h"
#include "engine/GameState.h"
#include "world/WorldLoader.h"
#include "parser/CommandParser.h"
#include "narration/BasicNarrator.h"
#include "narration/logger.h"
#include "world/SaveSystem.h"

int main() {
    WorldLoader loader;
    CommandParser parser;
    BasicNarrator narrator;
    GameEngine engine;

    Logger logger("data/game_log.txt");


    GameState state;
    /*if (SaveSystem::saveExists("data/save.json")) {
        state = loader.load("data/world.json");
        SaveSystem::loadSave(state, "data/save.json");
    } else {
        state = loader.load("data/world.json");
    }*/

   state = loader.load("data/world.json");

    bool gameRunning = true;
    std::string userInput;

    //std::cout << state.rooms[state.currentRoom].description << std::endl;

    std::cout << narrator.describeCurrentRoom(state) << std::endl;

    while (gameRunning) {

        std::getline(std::cin, userInput);
        //std::cout << userInput << std::endl;
        // Det är måndag, hade jag haft en dynamit nu hade jag
        // stoppat den i käften. Tänt på. Pang. Hade ni fått städa
        // den här baracken i flera veckor.

        Action action = parser.parse(userInput); // Gets the action message
        ActionResult result = engine.process_action(action, state);
        logger.log(userInput, action, result);

        if (action.type == ActionType::QUIT) {
            std::string narration = narrator.describeActionResult(state, action, result);
            std::cout << narration << std::endl;
            SaveSystem::saveGame(state, "data/save.json");
            logger.log(userInput, action, {false, "Player Quit"});
            break;
        }
        if (action.type == ActionType::HELP){
            std::cout << narrator.helpText(result) << std::endl;
            logger.log(userInput, action, {false, "Help displayed"});
            continue;
        }

        std::string narration = narrator.describeActionResult(state, action, result);
        std::cout << narration << std::endl;
    }

    return 0;
}