#include "diagnostics.h"
#include "engine/GameState.h"
#include <stdexcept>

Diagnostics::Diagnostics(const std::string& filename){
    file.open(filename);
    if (!file) throw std::runtime_error("Could not open diagnostics-file!");
    file << "==== GAME Diagnostics ====\n\n";
}

Diagnostics::~Diagnostics(){
    if (file.is_open()) file.close();
}

void Diagnostics::write(const GameState& state){
    step++;

    file << "\n--- SYSTEM DIAGNOSTIC ---" << std::endl;
    file << "Starting Room ID: '" << state.currentRoom << "'" << std::endl;
    file << "Total Rooms Loaded: " << state.rooms.size() << std::endl;
    for (const auto& room : state.rooms) {
        file << " - Loaded Room: '" << room.first << "'" << std::endl;
    }
    file << "-------------------------\n" << std::endl; // bra sätt att kolla våran load
    file.flush();
}