#include <string>

enum class Result {SUCCESS, INVALID_DIRECTION, QUIT};

struct ActionResult
{
    bool success;
    std::string message; // vad som händer för loggen
};