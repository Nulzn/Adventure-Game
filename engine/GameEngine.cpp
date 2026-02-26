#include "GameEngine.h"
#include "ActionResult.h"
#include "Action.h"
#include "GameState.h"
#include <string>
#include <algorithm>

ActionResult GameEngine::processaction(const Action& action, const GameState& state) {
    ActionResult result;
    result.success = false;
    //för att hitta nuvarande rum vi är i
    Room& currentRoom = state.rooms[state.currentRoom];
    
    
    switch(action.type){
        case ActionType::GO:
        if (currentRoom.exits.count(action.target)){ //north south east west 

            state.currentRoom.id = state.currentRoom.exits[action.target];
            result.success = true;
            result.message = "You walked to the" + state.rooms[state.currentRoomId].name + ".";
        }
        else {
            result.message = "You are not allowed to walk there.";
        }
        
        break;
        //GO, TAKE, DROP, EXIT, USE_ON, USE, QUIT, INSPECT
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
                result.message = "You do not have " + action.target + " in your inventory."
            }

            }

        } //take key 
    
    }
}
