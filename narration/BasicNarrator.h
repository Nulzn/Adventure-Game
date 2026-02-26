#pragma once 
#include "INarrator.h"
#include "GameState.h"
#include "ActionResult.h"



class BasicNarrator : public INarrator {
public:
    std::string describeCurrentRoom(const GameState& state) const override;

    std::string describeActionResult(
        const GameState& state,
        const Action& action,
        const ActionResult& result
    ) const override;

    std::string helpText() const override;
}; 