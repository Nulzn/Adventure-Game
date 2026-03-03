#include "GameEngine.h"
#include "GameState.h"
#include "parser/ActionResult.h"
#include "parser/Action.h"
//#include "[folderPath]/[fileName].(cpp, hpp, json, h)"
//#include "EXAMPLE: world/SaveSystem.h or world/SaveSystem.cpp"
#include <fstream>
#include <string>
#include <algorithm>
        //GO, TAKE, DROP, USE_ON, USE, QUIT, INSPECT

ActionResult GameEngine::process_action(const Action& action, GameState& state) {
    ActionResult result;
    result.success = false;
    
    Room& currentRoom = state.rooms[state.currentRoom];
    
    switch(action.type) {
        case ActionType::GO: {
            if (currentRoom.exits.count(action.target)) {
                std::string nextRoomId = currentRoom.exits[action.target];
                Room& nextRoom = state.rooms[nextRoomId];
                if (nextRoom.locked) {
                    result.message = "The door to the " + nextRoom.name + " is locked. You will need a key to unlock it!";
                } else {
                    state.currentRoom = nextRoomId;
                    result.success = true;
                    result.message = "You walked to the" + nextRoomId;
                }
            } else {
                result.message = "You are not allowed to walk there.";
            }
            break;
        }

        case ActionType::TAKE: {
            auto it = std::find(currentRoom.itemIds.begin(), currentRoom.itemIds.end(), action.target);
            if (it != currentRoom.itemIds.end()) {
                if (state.inventory.size() >= 6) {
                    result.message = "Your inventory is full.";
                } else {
                    state.inventory.push_back(action.target);
                    currentRoom.itemIds.erase(it);
                    result.success = true;
                    result.message = "You picked up " + action.target;
                }
            } else {
                result.message = "The item you seek is not in the room.";
            }
            break;
        }

        case ActionType::DROP: {
            auto it = std::find(state.inventory.begin(), state.inventory.end(), action.target);
            if (it != state.inventory.end()) {
                currentRoom.itemIds.push_back(action.target);
                state.inventory.erase(it);
                result.success = true;
                result.message = "You dropped " + action.target + " in " + currentRoom.name + ".";
            } else {
                result.message = "You do not have " + action.target + " in your inventory.";
            }
            break;
        }

        case ActionType::USE: {
            auto it = std::find(state.inventory.begin(), state.inventory.end(), action.target);
            if (it != state.inventory.end()) {
                // Logic here
            } else {
                result.success = false;
                result.message = "You do not have " + action.target + " in your inventory.";
            }
            break;
        } 

        case ActionType::USE_ON: {
            auto it = std::find(state.inventory.begin(), state.inventory.end(), action.target);
            if (it == state.inventory.end()) {
                result.success = false;
                result.message = "You do not have that item.";
                break;
            }
            
            bool foundTarget = false;
            for (auto const& [direction, roomId] : currentRoom.exits) {
                Room& targetRoom = state.rooms[roomId];
                if (targetRoom.locked && action.secondtarget == "door") {
                    if (targetRoom.requiredKeys.count(action.target)) {
                        targetRoom.locked = false;
                        result.success = true;
                        result.message = "You open the door to " + targetRoom.name + " you can now enter.";
                        foundTarget = true;
                        break;
                    }
                }
            } 
            if (!foundTarget) {
                result.success = false;
                result.message = "That item does not fit the door.";
            }
            break;
        } 

        default:
            result.message = "Action not recognized homeboy.";
            break;
    } // This MUST close the switch

    return result; 
} // This MUST be the final bracket in the file