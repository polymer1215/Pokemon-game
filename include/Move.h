#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <functional>

class Pokemon;

enum class MoveCategory {
    PHYSICAL,
    SPECIAL,
    STATUS
};

class Move {
private:
    std::string name;
    std::string scriptPath;
    int basePower;
    int accuracy;
    std::string type;
    MoveCategory category;
    
    // Function to execute Python skill script
    std::function<int(Pokemon&, Pokemon&)> effectFunction;

public:
    Move(const std::string& name, const std::string& scriptPath, 
         int power, int accuracy, const std::string& type, MoveCategory cat = MoveCategory::PHYSICAL);
    
    // Getters
    std::string getName() const { return name; }
    int getBasePower() const { return basePower; }
    int getAccuracy() const { return accuracy; }
    std::string getType() const { return type; }
    MoveCategory getCategory() const { return category; }
    std::string getScriptPath() const { return scriptPath; }
    
    // Execute the move
    int execute(Pokemon& attacker, Pokemon& defender);
    
    // Set custom effect function (loaded from Python)
    void setEffectFunction(std::function<int(Pokemon&, Pokemon&)> func);
};

#endif // MOVE_H
