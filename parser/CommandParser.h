#pragma once
#include "Action.h"
#include <string>

class CommandParser {
public: 
    Action parse(const std::string& input) const;

};