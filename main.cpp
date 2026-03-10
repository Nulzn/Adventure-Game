#include<iostream>
#include<string>
#include<engine/GameEngine.h>
#include<engine/GameState.h>
#include<world/WorldLoader.h>
#include<parser/CommandParser.h>
#include<narration/BasicNarrator.h>

int main() {
    WorldLoader loader;
    CommandParser parser;
    BasicNarrator narrator;
    GameEngine engine;


    GameState state = loader.load("data/world.json");

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

        if (action.type == ActionType::QUIT) {
            break;
        }

        ActionResult result = engine.process_action(action, state);

        std::string narration = narrator.describeActionResult(state, action, result);

        std::cout << narration << std::endl;
        
        //std::cout << narrator.describeActionResult(state, result, processedAction) << std::endl;
    }

    return 0;
}