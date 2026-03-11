#include "logger.h"
#include <stdexcept>

Logger::Logger(const std::string& filename){
    file.open(filename);
    if (!file) throw std::runtime_error("Could not open log-file!");
    file << "==== GAME LOG ====\n\n";
}

Logger::~Logger(){
    if (file.is_open()) file.close();
}

void Logger::log(const std::string& rawInput, const Action& action, const ActionResult& result){
    step++;
    file << "Step:      " << step << "\n";
    file << "Input:     " << rawInput << "\n";
    file << "Parsed:    " << (action.type != ActionType::INVALID ? "YES" : "NO") << "\n";
    file << "Success:   " << (result.success ? "YES" : "NO") << "\n";
    file << "Message:   " << (result.message) << "\n";
    file << "--------\n";
    file.flush();
}