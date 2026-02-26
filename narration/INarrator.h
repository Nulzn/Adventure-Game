#include <iostream>

class INarrator {

    public:
    virtual ~INarrator() = default;
    virtual std::string describeRoom(const GameState& state) const = 0;
    virtual std::string describeActionResult(
    const GameState& state,
    const Action& action,
    const ActionResult& result
    ) const = 0;
    virtual std::string helpText() const = 0;
};

