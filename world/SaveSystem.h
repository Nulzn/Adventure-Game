#pragma once
#include <string>
#include "engine/GameState.h"

class SaveSystem
{
public:
    static void save(const GameState& state, const std::string& filename);
};