#include "GameEngine.h"
#include "ActionResult.h"
#include "Action.h"
#include "GameState.h"
//#include "d"
#include <fstream>
#include <string>
#include <algorithm>
        //GO, TAKE, DROP, USE_ON, USE, QUIT, INSPECT

ActionResult GameEngine::process_action(const Action& action, GameState& state) {// ska det verkligen vara const
    ActionResult result;
    result.success = false;
    //för att hitta nuvarande rum vi är i
    Room& currentRoom = state.rooms[state.currentRoom];
    
    
    switch(action.type){
        case ActionType::GO: //la till en check ifall rummet är låst eller inte innan spelare går.
        if (currentRoom.exits.count(action.target)){ //north south east west 
            std::string nextRoomId = state.currentRoom.exits[action.target];
            Room& nextRoom = state.rooms[state.nextRoomId];
            if (nextRoom.locked){
                result.success = false;
                result.message = "The door to the " + nextRoom.name + " is locked. You will need a key to unlock it!";
                break;
            } else{

                state.currentRoom.id = state.currentRoom.exits[action.target];
                result.success = true;
                result.message = "You walked to the" + state.rooms[state.currentRoomId].name + ".";
            }
        } else {
            result.success = false;
            result.message = "You are not allowed to walk there.";
        }
        
        break;
        case  ActionType::TAKE:{

                auto it = std::find(currentRoom.itemIds.begin(), currentRoom.itemIds.end(), action.target);
            if (it != currentRoom.itemIds.end()){
                if (state.inventory.size() >= 6){
                    result.success = false;
                    result.message = "Your inventory is full."
                } else {
                    state.inventory.push_back(action.target);
                    currentRoom.itemIds.erase(it);
                    result.success = true;
                    result.message = "You picked up " + action.target + ".";
                }
            }   else {
                result.success = false;
                result.message = "The item you seek is not in the room.";
        }
        break;

        }
        case ActionType::DROP:{
                auto it = std::find(state.inventory.begin(),state.inventory.end(),action.target);
            if (it != state.inventory.end()){
                currentRoom.itemIds.push_back(action.target);
                state.inventory.erase(it);
                result.success = true;
                result.message = "You dropped " + action.target + " in " +state.rooms[state.currentRoomId].name + ".";
            } else {
                result.success = false;
                result.message = "You do not have " + action.target + " in your inventory.";
            }
        break;

            }
        case ActionType::USE:{
            auto it = std::find(state.inventory.begin(),state.inventory.end(),action.target);
            if (it != state.inventory.end()){
                ...
            } else {
                result.success = false;
                result.message = "You do not have " + action.target + " in your inventory.";
            }
        break;
        
        case ActionType::USE_ON:{
            auto it = std::find(state.inventory.begin(),state.inventory.end(),action.target);
            if (it == state.inventory.end()){
                result.success = false;
                result.message = "You do not have that item. ";
                break;
            }
            
            bool foundTarget = false;
            for (auto const& [direction, roomId] : currentRoom.exits) {
                Room& targetRoom = state.rooms[roomId]; //loopar igenom alla dörrarna nära dig
                if (targetRoom.locked && action.secondtarget == "door"){ // kollar ifall du vill låsa upp.
                    if (targetRoom.requiredKeys.count(action.target)){ //låser upp
                        targetRoom.locked = false;
                        result.success = true;
                        result.message = "You open the door to " + targetRoom.name + " you can now enter.";
                        foundTarget = true;
                        break;
                    }
                }
        } if (!foundTarget) {
            result.success = false;
            result.message = "That item does not fit the door. ";
        }
        break;
        } //take key 
    
    }
}
