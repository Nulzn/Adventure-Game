#include <string>
//enum var typ goated för att skilja actions åt. Det är typ ett tal 
enum class ActionType{GO, TAKE, DROP, EXIT, USE_ON, USE, QUIT, INSPECT, INVALID};

struct Action { //två olika targets
    ActionType type;
    std::string target; //saker som north eller typ key 
    std::string secondtarget; //saker som use on door 
};