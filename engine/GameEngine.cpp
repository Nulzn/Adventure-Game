#include "GameEngine.h"
#include "GameState.h"
#include "parser/ActionResult.h"
#include "parser/Action.h"
#include "items/item.h"
#include <fstream>
#include <string>
#include <iostream>


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
            auto it = currentRoom.items.find(action.target);
            if (it != currentRoom.items.end()) {
                if (state.inventory.size() >= 6) {
                    result.message = "Your pockets are completely stuffed, fat boy. You need to drop something before you can carry that.";
                } else {
                    std::shared_ptr<Item> item_to_take = it->second;
                    state.inventory.push_back(item_to_take);
                    currentRoom.items.erase(it);
                    result.success = true;
                    result.message = "You picked up " + item_to_take->displayName + ".";
                }
            } else {
                result.message = "The item you seek is not in the room.";
            }
            break;
        }

        case ActionType::DROP: {
            auto it = std::find_if(state.inventory.begin(), state.inventory.end(), [&](const std::shared_ptr<Item>& item){
                    return item->id == action.target; //Den här lilla funktionen retunerar små bokstäver som vi sedan kan jämföra (förhoppningsvis)
            });
            
            if (it != state.inventory.end()) {
                std::shared_ptr<Item> item_to_drop = *it;
                currentRoom.items[item_to_drop->id] = item_to_drop;
                state.inventory.erase(it);
                result.success = true;
                result.message = "You dropped " + item_to_drop->displayName + " in " + currentRoom.name + ".";
            } else {
                result.message = "You do not have " + action.target + " in your inventory.";
            }
            break;
        }

        /*case ActionType::USE: {
            auto it = std::find_if(state.inventory.begin(), state.inventory.end(), [&](const std::shared_ptr<Item>& item){
                    return item->commandName == action.target;
            });
            if (it != state.inventory.end()) {
                ... MASSIVE CODE BLOCK ... BOOM
            } else {
                result.success = false;
                result.message = "You do not have " + action.target + " in your inventory.";
            }
            break;
        } */

        case ActionType::USE_ON: {
            auto it = std::find_if(state.inventory.begin(), state.inventory.end(), [&](const std::shared_ptr<Item>& item){
                    return item->id == action.target; 
            });
            if (it == state.inventory.end()) {
                result.success = false;
                result.message = "You do not have that item.";
                break;
            }
            std::shared_ptr<Item> item_to_use = *it;
            bool foundTarget = false;
            bool unlockedtarget = false;

            for (auto const& [direction, roomId] : currentRoom.exits) {
                Room& targetRoom = state.rooms[roomId];
                if (targetRoom.locked && action.secondtarget == "door") {
                    foundTarget = true;

                    auto key_item = std::dynamic_pointer_cast<KeyItem>(item_to_use);
                    if (!key_item){
                        result.message = "You can not use the " + item_to_use->displayName + " as the key boy.";
                        break;
                    }
                    for (auto const& [jsonkey, required_key_id] : targetRoom.requiredKeys){
                        if (required_key_id == item_to_use->id){
                            targetRoom.locked = false;
                            result.success = true;
                            result.message = "You unlocked the door to the " + targetRoom.name + " you are welcome to walk in now boy.";
                            unlockedtarget = true;
                            break;
                        }
                    }
                } if (unlockedtarget) break;
            } 
            if (!foundTarget) {
                result.success = false;
                result.message = "There is no locked door here boy.";
            } else if (!unlockedtarget){
                result.success = false;
                result.message = "It does not fit the door boy";
            }
            break;
        } 
        case ActionType::LOOK: {
            result.success = true;
            std::string msg = "";
            if (!currentRoom.items.empty()){
                msg += "You see the follow items in the room: \n";
                for (const auto& [id, item] : currentRoom.items){
                    msg += "\n - " + item->displayName;
                }
            } else {
                msg += "There are no items in this room.";
            }
            result.message = msg;
            break;
        }
        case ActionType::INVENTORY: {
            result.success = true;
            std::string msg = "";
            if (state.inventory.size() != 0){
                msg += "This is in your inventory: \n";
                for(const auto& item_ptr : state.inventory){
                    msg += "\n - " + item_ptr->displayName;
                }

            } else {
                msg += "Your inventory is empty. ";

            }
            result.message = msg;
            break;
        }
        case ActionType::INSPECT: {
            auto it = std::find_if(state.inventory.begin(), state.inventory.end(), [&](const std::shared_ptr<Item>& item){
                return item->id == action.target;
            });
            if (it != state.inventory.end()){
                std::shared_ptr<Item> item_to_inspect = *it;
                result.success = true;
                result.message = item_to_inspect->onInspect();
            } else {
                result.message = "You do not see that item. ";
            }
            break;

            
        }
        /*case ActionType::QUIT: {
            result.success = true;
            break;

        }
        case ActionType::HELP:{
            std::cout<<"test\n\n";
            result.success = true;
            result.message = "Commands: \n"
            "  look              - search for items in current room\n"
            "  go <direction>    - move (north/south/east/west)\n"
            "  take <item>       - pick up an item\n"
            "  drop <item>       - drop an item\n"
            "  inspect <item>    - examine an item\n"
            "  use <item> on <target> - use item on something\n"
            "  inventory         - show your inventory\n"
            "  save <filename>   - save the game\n"
            //"  load <filename>   - load a saved game\n"
            "  quit              - quit the game\n"
            "  open <container> <code> - open a container with a code\n"
            "  consume <item>    - consume a consumable\n";
            break;
        }*/
        case ActionType::OPEN: {
            auto it = currentRoom.items.find(action.target);
            if (it != currentRoom.items.end()){
                std::shared_ptr<Item> item_to_open = it->second;
                auto container = std::dynamic_pointer_cast<ContainerItem>(item_to_open);
                if (container && container->locked){
                    if(action.secondtarget == container->code){
                        container->locked = false;
                        std::string foundItems = "";
                        for (auto& [id, item] : container->contents){
                            currentRoom.items[id] = item;
                            foundItems += "\n - " + item->displayName;
                        }
                        container->contents.clear();
                        result.success = true;
                        result.message = "You successfully open the " +item_to_open->displayName + " .\nInside you find: " + foundItems;

                    } else {
                        result.message = "Nice try homeboy. Wrong code.\n";

                    }
                } else {
                    result.message = item_to_open->displayName + " is already open!\n";

                }
                
            } else{
                result.message = action.target + " does not exist in this room. \n";
            }
            break;
        }
        case ActionType::CONSUME: {
            auto it = std::find_if(state.inventory.begin(), state.inventory.end(), [&](const std::shared_ptr<Item>& item){
                return item->id == action.target;
            });
            if (it != state.inventory.end()){
                std::shared_ptr<Item> item_to_eat = *it;
                auto consumable = std::dynamic_pointer_cast<ConsumableItem>(item_to_eat);
                if (consumable){
                    if (consumable->id == "redpill"){
                        state.hasWon = true;
                        result.success = true;
                        result.message = "You have eaten the red pill... welcome to the outside world, now you finally see the TRUTH!!!";
                    }
                    else if (consumable->id == "bluepill"){
                        state.hasWon = true;
                        result.success = true;
                        result.message = "You wake up at a retirement home old as fuck. But extreamly happy about your FAKE life. Bye!";
                    }
                    else {
                        result.success = true;
                        result.message = "Nom nom nom, fat boy.\n";
                    }
                    state.inventory.erase(it);
                } else {
                    result.message = "Can not eat that fat boy.";
                }
            }
            break;
            /*case ActionType::SAVE: {
                
            }*/
        }
        default:
            result.message = "Action not recognized homeboy.\n";
            break;
    } // SWITCH CASE SISTA BRACKET DAFUQ


    return result; 
} //SISTA BRACKET DAFUQ