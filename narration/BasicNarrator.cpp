#include "BasicNarrator.h"
#include "engine/GameState.h"
#include "parser/ActionResult.h"
#include "parser/Action.h"
#include <sstream>

std::string BasicNarrator::describeCurrentRoom(const GameState& state) const {
    const auto& room = state.rooms.at(state.currentRoom);

    std::string text = room.name + "\n";
    text += room.description + "\n";
    text += "Exits: ";

    for (const auto& exit : room.exits){
        text += exit.first + ", ";
    }
    return text;
}

std::string BasicNarrator::describeActionResult(
    const GameState& state,
    const Action& action,
    const ActionResult& result
) const {
    if (!result.success){
        return result.message;
    }

    switch (action.type) {
        case ActionType::QUIT:
            return "Goodbye!";
        case ActionType::GO:
            return describeCurrentRoom(state);
        
        default:
            return result.message;
    }
};

std::string BasicNarrator::helpText(ActionResult& result) const {
    result.message =  "Commands:\n"
            "  look              - search for items in current room\n"
            "  go <direction>    - move (north/south/east/west)\n"
            "  take <item>       - pick up an item\n"
            "  drop <item>       - drop an item\n"
            "  inspect <item>    - examine an item\n"
            "  use <item>        - use an item\n"
            "  use <item> on <target> - use item on something\n"
            "  inventory         - show your inventory\n"
            "  save <filename>   - save the game\n"
            "  load <filename>   - load a saved game\n"
            "  quit              - quit the game\n";

    return result.message;   
}