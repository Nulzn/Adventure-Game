#pragma once

#include <string>
#include "GameState.h"

class WorldLoader
{
public:
    static GameState load(const std::string& filename);
};