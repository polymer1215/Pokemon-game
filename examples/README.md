# Pokemon Battle Game Examples

This directory contains example code snippets and usage scenarios for the Pokemon Battle Game.

## Contents

- [basic_battle.cpp](#basic_battlecpp) - Simple one-on-one battle
- [custom_moves.cpp](#custom_movescpp) - Creating custom moves
- [status_effects.cpp](#status_effectscpp) - Using status effects
- [type_advantages.cpp](#type_advantagescpp) - Demonstrating type effectiveness

## basic_battle.cpp

Basic battle between Pikachu and Squirtle:

```cpp
#include "Pokemon.h"
#include "Move.h"
#include "Battle.h"
#include "PythonSkillLoader.h"
#include <memory>

int main() {
    // Initialize Python interpreter
    PythonSkillLoader::initialize();
    
    // Create two Pokemon
    auto pikachu = std::make_shared<Pokemon>("Pikachu", "Electric", 100, 55, 40, 50, 90);
    auto squirtle = std::make_shared<Pokemon>("Squirtle", "Water", 120, 48, 65, 64, 43);
    
    // Create basic moves
    auto thunderbolt = std::make_shared<Move>("Thunderbolt", "", 90, 100, "Electric", MoveCategory::SPECIAL);
    auto waterGun = std::make_shared<Move>("Water Gun", "", 40, 100, "Water", MoveCategory::SPECIAL);
    
    // Add moves to Pokemon
    pikachu->addMove(thunderbolt);
    squirtle->addMove(waterGun);
    
    // Create and start battle
    Battle battle(pikachu, squirtle);
    auto winner = battle.start();
    
    std::cout << "Winner: " << winner->getName() << std::endl;
    
    // Cleanup
    PythonSkillLoader::finalize();
    
    return 0;
}
```

## custom_moves.cpp

Creating moves with Python scripts:

```cpp
#include "Pokemon.h"
#include "Move.h"
#include "Battle.h"
#include "PythonSkillLoader.h"
#include <memory>

// Helper function to load Python skills
void loadPythonSkill(std::shared_ptr<Move> move, const std::string& scriptName) {
    auto skillFunc = PythonSkillLoader::loadSkill(scriptName, "calculate_damage");
    move->setEffectFunction(skillFunc);
}

int main() {
    PythonSkillLoader::initialize();
    
    // Create Pokemon
    auto charmander = std::make_shared<Pokemon>("Charmander", "Fire", 110, 52, 43, 50, 65);
    auto bulbasaur = std::make_shared<Pokemon>("Bulbasaur", "Grass", 115, 49, 49, 65, 45);
    
    // Create moves with Python scripts (note: script names without .py extension)
    auto flamethrower = std::make_shared<Move>("Flamethrower", "flamethrower", 90, 100, "Fire", MoveCategory::SPECIAL);
    auto slash = std::make_shared<Move>("Slash", "slash", 70, 100, "Normal", MoveCategory::PHYSICAL);
    
    // Load Python skills
    loadPythonSkill(flamethrower, "flamethrower");
    loadPythonSkill(slash, "slash");
    
    // Create moves for Bulbasaur
    auto vineWhip = std::make_shared<Move>("Vine Whip", "", 45, 100, "Grass", MoveCategory::PHYSICAL);
    auto razorLeaf = std::make_shared<Move>("Razor Leaf", "", 55, 95, "Grass", MoveCategory::PHYSICAL);
    
    // Assign moves
    charmander->addMove(flamethrower);
    charmander->addMove(slash);
    bulbasaur->addMove(vineWhip);
    bulbasaur->addMove(razorLeaf);
    
    // Battle
    Battle battle(charmander, bulbasaur);
    auto winner = battle.start();
    
    std::cout << "Winner: " << winner->getName() << std::endl;
    
    PythonSkillLoader::finalize();
    return 0;
}
```

## status_effects.cpp

Demonstrating status effects:

```cpp
#include "Pokemon.h"
#include "Move.h"
#include "Battle.h"
#include "PythonSkillLoader.h"
#include <memory>

int main() {
    PythonSkillLoader::initialize();
    
    // Create Pokemon
    auto pikachu = std::make_shared<Pokemon>("Pikachu", "Electric", 100, 55, 40, 50, 90);
    auto bulbasaur = std::make_shared<Pokemon>("Bulbasaur", "Grass", 115, 49, 49, 65, 45);
    
    // Create damaging moves
    auto thunderbolt = std::make_shared<Move>("Thunderbolt", "thunderbolt.py", 90, 100, "Electric", MoveCategory::SPECIAL);
    auto vineWhip = std::make_shared<Move>("Vine Whip", "", 45, 100, "Grass", MoveCategory::PHYSICAL);
    
    // Create status moves (script names without .py extension)
    auto thunderWave = std::make_shared<Move>(
        "Thunder Wave",           // Name
        "thunder_wave",           // Script (without .py)
        0,                        // Power (0 for status)
        100,                      // Accuracy
        "Electric",               // Type
        MoveCategory::STATUS,     // Category
        "Paralyzed",             // Status effect
        4                         // Duration
    );
    
    auto toxic = std::make_shared<Move>(
        "Toxic",
        "toxic",
        0,
        90,
        "Poison",
        MoveCategory::STATUS,
        "Poisoned",
        5
    );
    
    // Load Python skills
    auto tbFunc = PythonSkillLoader::loadSkill("thunderbolt", "calculate_damage");
    thunderbolt->setEffectFunction(tbFunc);
    
    auto twFunc = PythonSkillLoader::loadSkill("thunder_wave", "calculate_damage");
    thunderWave->setEffectFunction(twFunc);
    
    auto toxFunc = PythonSkillLoader::loadSkill("toxic", "calculate_damage");
    toxic->setEffectFunction(toxFunc);
    
    // Assign moves
    pikachu->addMove(thunderbolt);
    pikachu->addMove(thunderWave);  // Paralyzes opponent
    
    bulbasaur->addMove(vineWhip);
    bulbasaur->addMove(toxic);      // Poisons opponent
    
    // Battle with status effects
    Battle battle(pikachu, bulbasaur);
    auto winner = battle.start();
    
    std::cout << "Winner: " << winner->getName() << std::endl;
    
    PythonSkillLoader::finalize();
    return 0;
}
```

## type_advantages.cpp

Demonstrating type effectiveness:

```cpp
#include "Pokemon.h"
#include "Move.h"
#include "Battle.h"
#include "TypeEffectiveness.h"
#include <iostream>
#include <memory>

void demonstrateTypeMatchup(const std::string& attackType, const std::string& defenseType) {
    double effectiveness = TypeEffectiveness::getEffectiveness(attackType, defenseType);
    std::cout << attackType << " vs " << defenseType << ": ";
    
    if (effectiveness == 2.0) {
        std::cout << "Super Effective! (2x damage)" << std::endl;
    } else if (effectiveness == 1.0) {
        std::cout << "Normal (1x damage)" << std::endl;
    } else if (effectiveness == 0.5) {
        std::cout << "Not Very Effective (0.5x damage)" << std::endl;
    } else if (effectiveness == 0.0) {
        std::cout << "No Effect! (0x damage)" << std::endl;
    }
}

int main() {
    std::cout << "=== Type Effectiveness Examples ===" << std::endl;
    
    // Super effective examples
    demonstrateTypeMatchup("Water", "Fire");      // 2x
    demonstrateTypeMatchup("Fire", "Grass");      // 2x
    demonstrateTypeMatchup("Electric", "Water");  // 2x
    demonstrateTypeMatchup("Grass", "Water");     // 2x
    
    std::cout << std::endl;
    
    // Not very effective examples
    demonstrateTypeMatchup("Fire", "Water");      // 0.5x
    demonstrateTypeMatchup("Water", "Grass");     // 0.5x
    demonstrateTypeMatchup("Grass", "Fire");      // 0.5x
    
    std::cout << std::endl;
    
    // No effect examples
    demonstrateTypeMatchup("Electric", "Ground"); // 0x
    demonstrateTypeMatchup("Ground", "Flying");   // 0x
    
    std::cout << std::endl;
    
    // Normal effectiveness
    demonstrateTypeMatchup("Normal", "Water");    // 1x
    demonstrateTypeMatchup("Fire", "Fighting");   // 1x
    
    std::cout << "\n=== Battle Example ===" << std::endl;
    
    PythonSkillLoader::initialize();
    
    // Water vs Fire - Water has advantage
    auto squirtle = std::make_shared<Pokemon>("Squirtle", "Water", 120, 48, 65, 64, 43);
    auto charmander = std::make_shared<Pokemon>("Charmander", "Fire", 110, 52, 43, 50, 65);
    
    auto waterGun = std::make_shared<Move>("Water Gun", "water_gun", 40, 100, "Water", MoveCategory::SPECIAL);
    auto flamethrower = std::make_shared<Move>("Flamethrower", "flamethrower", 90, 100, "Fire", MoveCategory::SPECIAL);
    
    // Load skills
    auto wgFunc = PythonSkillLoader::loadSkill("water_gun", "calculate_damage");
    waterGun->setEffectFunction(wgFunc);
    
    auto ftFunc = PythonSkillLoader::loadSkill("flamethrower", "calculate_damage");
    flamethrower->setEffectFunction(ftFunc);
    
    squirtle->addMove(waterGun);
    charmander->addMove(flamethrower);
    
    std::cout << "\nSquirtle (Water) vs Charmander (Fire)" << std::endl;
    std::cout << "Squirtle's Water Gun will be SUPER EFFECTIVE!" << std::endl;
    std::cout << "Charmander's Flamethrower will be NOT VERY EFFECTIVE!" << std::endl;
    
    Battle battle(squirtle, charmander);
    auto winner = battle.start();
    
    PythonSkillLoader::finalize();
    return 0;
}
```

## Running the Examples

To compile and run these examples:

```bash
# Create a new .cpp file with the example code
nano my_example.cpp

# Modify CMakeLists.txt to add a new executable:
# add_executable(my_example examples/my_example.cpp src/Pokemon.cpp src/Move.cpp src/Battle.cpp src/PythonSkillLoader.cpp src/TypeEffectiveness.cpp)
# target_link_libraries(my_example ${Python3_LIBRARIES})

# Or compile directly with g++:
g++ -std=c++14 -I./include -I/usr/include/python3.x \
    my_example.cpp \
    src/Pokemon.cpp src/Move.cpp src/Battle.cpp \
    src/PythonSkillLoader.cpp src/TypeEffectiveness.cpp \
    -lpython3.x -o my_example

# Run
./my_example
```

## More Examples

For more examples, see:
- [API Documentation](../docs/API.md) - Complete API with examples
- [Python Skills Guide](../docs/PYTHON_SKILLS.md) - Custom move examples
- [main.cpp](../src/main.cpp) - Full game implementation

## Creating Your Own Examples

1. Start with a basic battle setup
2. Add your custom Pokemon and moves
3. Experiment with type matchups
4. Try different status effects
5. Create custom Python skill scripts

Happy coding!
