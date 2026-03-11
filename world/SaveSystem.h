#pragma once

#include <string>
#include "engine/GameState.h"


class SaveSystem {
public:
    static void saveGame(GameState& state, const std::string& fileName);
    static bool saveExists(const std::string& fileName);
    static void loadSave(GameState& state, const std::string& fileName);
};