#include <iostream>
#include <string>
//la till gamestate osv.
#include "GameState.h"
#include "Action.h"
#include "ActionResult.h"

class INarrator {
    
    public:
    virtual ~INarrator() = default;
    virtual std::string describeCurrentRoom(const GameState& state) const = 0;
    virtual std::string describeActionResult(
    const GameState& state,
    const Action& action,
    const ActionResult& result
    ) const = 0;
    virtual std::string helpText() const = 0;
};

