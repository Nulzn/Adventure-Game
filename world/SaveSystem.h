#pragma once
#include <string>
#include "GameState.h"

class SaveSystem
{
public:
    static void save(const std::string& filename);
};