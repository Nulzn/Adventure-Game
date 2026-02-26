
class GameEngine{
    private:
    GameState state;
    public:
    ActionResult processaction(const Action& action, const GameState& state);
};