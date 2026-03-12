#pragma once
#include <string>
#include <fstream>
#include "parser/Action.h"
#include "parser/ActionResult.h"
#include "engine/GameState.h"


class Diagnostics {
public:
    Diagnostics(const std::string& filename);
    ~Diagnostics();
    void write(const GameState& state);
    //void logState(const Gamestate& state);

private: 
    std::ofstream file;
    int step = 0;
};