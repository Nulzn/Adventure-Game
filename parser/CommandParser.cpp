#include "CommandParser.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <sstream>

Action CommandParser::parse(const std::string& input)const {

    std :: istringstream iss(input);
    std :: string word1, word2;

    iss >> word1 >> word2;

    if (boost::algorithm::to_lower(word1) == "look" && word2.empty()){
        return {ActionType::INSPECT, ""};
    }

    if (boost::algorithm::to_lower(word1) == "go" && !word2.empty()){
        return {ActionType::GO, word2};
    }

    if (boost::algorithm::to_lower(word1) == "quit" && word2.empty()){
        return {ActionType::QUIT, ""};
    }

    return {ActionType::INVALID, ""};
}