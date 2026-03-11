#pragma once
#include <string>
#include <fstream>
#include "parser/Action.h"
#include "parser/ActionResult.h"


class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();
    void log(const std::string& rawInput, const Action& action, const ActionResult& result);

private: 
    std::ofstream file;
    int step = 0;
};