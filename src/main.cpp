#include "Pokemon.h"
#include "Move.h"
#include "Battle.h"
#include "PythonSkillLoader.h"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>

int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Initialize Python interpreter
    PythonSkillLoader::initialize();
    
    std::cout << "=== Pokemon Battle Game ===" << std::endl;
    std::cout << "Core battle system with Python skill integration\n" << std::endl;
    
    try {
        // Create Pokemon with types
        auto pikachu = std::make_shared<Pokemon>("Pikachu", "Electric", 100, 55, 40, 50, 90);
        auto charmander = std::make_shared<Pokemon>("Charmander", "Fire", 110, 52, 43, 50, 65);
        
        // Create moves with Python scripts and proper categories
        auto thunderbolt = std::make_shared<Move>("Thunderbolt", "thunderbolt.py", 90, 100, "Electric", MoveCategory::SPECIAL);
        auto flamethrower = std::make_shared<Move>("Flamethrower", "flamethrower.py", 90, 100, "Fire", MoveCategory::SPECIAL);
        
        // Try to load Python skill functions
        try {
            auto thunderboltFunc = PythonSkillLoader::loadSkill("thunderbolt", "calculate_damage");
            thunderbolt->setEffectFunction(thunderboltFunc);
            std::cout << "Loaded Thunderbolt skill from Python script." << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Using default Thunderbolt effect (Python script not found)." << std::endl;
        }
        
        try {
            auto flamethrowerFunc = PythonSkillLoader::loadSkill("flamethrower", "calculate_damage");
            flamethrower->setEffectFunction(flamethrowerFunc);
            std::cout << "Loaded Flamethrower skill from Python script." << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Using default Flamethrower effect (Python script not found)." << std::endl;
        }
        
        // Create additional basic moves
        auto quickAttack = std::make_shared<Move>("Quick Attack", "", 40, 100, "Normal", MoveCategory::PHYSICAL);
        auto ember = std::make_shared<Move>("Ember", "", 40, 100, "Fire", MoveCategory::SPECIAL);
        
        // Assign moves to Pokemon
        pikachu->addMove(thunderbolt);
        pikachu->addMove(quickAttack);
        
        charmander->addMove(flamethrower);
        charmander->addMove(ember);
        
        std::cout << "\nPokemon created successfully!\n" << std::endl;
        
        // Create and start battle
        Battle battle(pikachu, charmander);
        auto winner = battle.start();
        
        std::cout << "\n=== Battle Summary ===" << std::endl;
        std::cout << "Winner: " << winner->getName() << std::endl;
        std::cout << "Remaining HP: " << winner->getCurrentHP() << "/" << winner->getMaxHP() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        PythonSkillLoader::finalize();
        return 1;
    }
    
    // Finalize Python interpreter
    PythonSkillLoader::finalize();
    
    return 0;
}
