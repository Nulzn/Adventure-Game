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
    if (action.type == ActionType::QUIT){
        return "Goodbye!";
    }
    if (action.type == ActionType::GO){
        return describeCurrentRoom(state);
    }
    return "";
};

std::string BasicNarrator::helpText(ActionResult& result) const {
    result.message = "HELP IS ON THE WAY!";
    return result.message;   
}