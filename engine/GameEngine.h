#pragma once

#include "GameState.h"
#include "parser/ActionResult.h"
#include "parser/Action.h"

class GameEngine{
    private:
    GameState state;
    public:
    ActionResult process_action(const Action& action, GameState& state);
};