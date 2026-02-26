#include "CommandParser.h"
#include <sstream>

Action CommandParser::parse(const std::string& input)const {

    std :: istringstream iss(input);
    std :: string word1, word2;

    iss >> word1 >> word2;

    if (word1 == "look" && word2.empty()){
        return {ActionType::INSPECT, ""};
    }

    if (word1 == "go" && !word2.empty()){
        return {ActionType::GO, word2};
    }

    if (word1 == "quit" && word2.empty()){
        return {ActionType::QUIT, ""};
    }

    return {ActionType::INVALID, ""};
}