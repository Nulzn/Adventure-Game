#include "CommandParser.h"
#include <sstream>
#include <algorithm>
#include <cctype>

Action CommandParser::parse(const std::string& input) const {
    //Dela upp input i tokens och gör allt till lowercase
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string word;

    while (iss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){return tolower(c);});
        tokens.push_back(word);
    }

    //Tom input
    if (tokens.empty()){
        return {ActionType::INVALID, "", ""};
    }

    const std::string& cmd = tokens[0];
    const size_t count = tokens.size();

    //1-token command
    if (count == 1){
        if (cmd == "look") return {ActionType::LOOK, "", ""};
        if (cmd == "inventory") return {ActionType::INVENTORY, "", ""};
        
        //Command som kräver argument
        if (cmd == "go")    return {ActionType::INVALID, "", ""};
    }

    //2-token command
    if (count == 2){
        if (cmd == "go")    return {ActionType::GO, tokens[1], ""};
        if (cmd == "take")  return {ActionType::TAKE, tokens[1], ""};
        if (cmd == "drop")  return {ActionType::DROP, tokens[1], ""};
        if (cmd == "inspect") return {ActionType::INSPECT, tokens[1], ""};
        if (cmd == "use")   return {ActionType::USE, tokens[1], ""};
    }

    //4-token command
    if (count == 4 && cmd == "use" && tokens[2] == "on"){
        return {ActionType::USE_ON, tokens[1], tokens[3]};
    }

    //Allt annat är ogiltigt
    return {ActionType::INVALID, "", ""};
}