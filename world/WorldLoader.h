#pragma once

#include <string>
#include "engine/GameState.h"

class WorldLoader
{
public:
    static GameState load(const std::string& filename);
};