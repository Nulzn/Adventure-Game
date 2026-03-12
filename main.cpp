#include<iostream>
#include<string>

#include "engine/GameEngine.h"
#include "engine/GameState.h"
#include "world/WorldLoader.h"
#include "parser/CommandParser.h"
#include "narration/BasicNarrator.h"
#include "narration/logger.h"
#include "narration/diagnostics.h"
#include "world/SaveSystem.h"

int main() {
    WorldLoader loader;
    CommandParser parser;
    BasicNarrator narrator;
    GameEngine engine;

    Logger logger("data/game_log.txt");

    Diagnostics diagnostics("data/system_DOIDOIDOI.txt");

    GameState state = loader.load("data/baseGame.json");
    diagnostics.write(state);
    bool gameRunning = true;
    std::string userInput;

    //std::cout << state.rooms[state.currentRoom].description << std::endl;
    
    std::cout <<"Hello fucker welcome to the matrix. You have been placed here to try and escape it, dun dun duuuun.\n";
    Room& currentRoom = state.rooms[state.currentRoom];
    std::cout << "You're standing in the " << currentRoom.name << std::endl;
    std::cout <<currentRoom.description << std::endl;
    //Lite välkomstfraser oså



    while (gameRunning) {

        std::getline(std::cin, userInput);
        //std::cout << userInput << std::endl;
        // Det är måndag, hade jag haft en dynamit nu hade jag
        // stoppat den i käften. Tänt på. Pang. Hade ni fått städa
        // den här baracken i flera veckor.

        // IT'S THURSDAY, HAVE I HAD A DYNAMITE NOW HAD I PUT IT IN THE MOUTH
        // LIGHT IT, BOOM. HAD TO CLEAN THE WHOLE BARACK IN MANY WEEKS.

        Action action = parser.parse(userInput); // Gets the action message


        if (action.type == ActionType::QUIT) {
            logger.log(userInput, action, {false, "Player Quit"});
            std::string narration = narrator.describeActionResult(state, action, {true});
            std::cout << narration << std::endl;
            gameRunning = false;
            continue;
        }

        if (action.type == ActionType::HELP) {
            std::cout << narrator.helpText() << std::endl;
            logger.log(userInput, action, {false, "Help displayed"});
            continue;
        }

        if (action.type == ActionType::LOAD) {
            loader.load("data/world.json");
            try {
                SaveSystem::loadSave(state, "data/save.json");
            } catch (const std::exception& e) {
                throw std::runtime_error("Could not save file.");
            }
            continue;
        }

        if (action.type == ActionType::SAVE) {
            try {
                SaveSystem::saveGame(state, "data/save.json");
            } catch (const std::exception& e) {
                throw std::runtime_error("Could not save file.");
            }
            continue;
        }
        
        //Skicka allt annat till engine
        ActionResult result = engine.process_action(action, state);
        logger.log(userInput, action, result);
        
        std::string narration = narrator.describeActionResult(state, action, result);
        std::cout << narration << std::endl;
        
        //Kolla wincon
        if (state.hasWon == true){
            gameRunning = false;
        }
    }

    return 0;
}