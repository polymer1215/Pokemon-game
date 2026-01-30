#include "Pokemon.h"
#include "Move.h"
#include "Battle.h"
#include "PythonSkillLoader.h"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <vector>

void loadPythonSkill(std::shared_ptr<Move> move, const std::string& scriptName) {
    try {
        auto skillFunc = PythonSkillLoader::loadSkill(scriptName, "calculate_damage");
        move->setEffectFunction(skillFunc);
        std::cout << "✓ Loaded " << move->getName() << " skill from Python script." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✓ Using default effect for " << move->getName() << "." << std::endl;
    }
}

int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Initialize Python interpreter
    PythonSkillLoader::initialize();
    
    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║    Pokemon Battle Game - Enhanced!     ║" << std::endl;
    std::cout << "║  With Types, Status Effects & More!   ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    try {
        std::cout << "Creating Pokemon..." << std::endl;
        
        // Create diverse Pokemon with different types
        auto pikachu = std::make_shared<Pokemon>("Pikachu", "Electric", 100, 55, 40, 50, 90);
        auto squirtle = std::make_shared<Pokemon>("Squirtle", "Water", 120, 48, 65, 64, 43);
        auto bulbasaur = std::make_shared<Pokemon>("Bulbasaur", "Grass", 115, 49, 49, 65, 45);
        auto charmander = std::make_shared<Pokemon>("Charmander", "Fire", 110, 52, 43, 50, 65);
        
        std::cout << "\nLoading moves..." << std::endl;
        
        // Create electric moves for Pikachu
        auto thunderbolt = std::make_shared<Move>("Thunderbolt", "thunderbolt.py", 90, 100, "Electric", MoveCategory::SPECIAL);
        auto thunderWave = std::make_shared<Move>("Thunder Wave", "thunder_wave.py", 0, 100, "Electric", MoveCategory::STATUS, "Paralyzed", 4);
        auto quickAttack = std::make_shared<Move>("Quick Attack", "", 40, 100, "Normal", MoveCategory::PHYSICAL);
        
        loadPythonSkill(thunderbolt, "thunderbolt");
        loadPythonSkill(thunderWave, "thunder_wave");
        
        // Create water moves for Squirtle
        auto waterGun = std::make_shared<Move>("Water Gun", "water_gun.py", 40, 100, "Water", MoveCategory::SPECIAL);
        auto bubble = std::make_shared<Move>("Bubble", "", 40, 100, "Water", MoveCategory::SPECIAL);
        auto withdraw = std::make_shared<Move>("Withdraw", "", 0, 100, "Water", MoveCategory::STATUS);
        
        loadPythonSkill(waterGun, "water_gun");
        
        // Create grass moves for Bulbasaur
        auto vineWhip = std::make_shared<Move>("Vine Whip", "", 45, 100, "Grass", MoveCategory::PHYSICAL);
        auto razorLeaf = std::make_shared<Move>("Razor Leaf", "", 55, 95, "Grass", MoveCategory::PHYSICAL);
        auto toxic = std::make_shared<Move>("Toxic", "toxic.py", 0, 90, "Grass", MoveCategory::STATUS, "Poisoned", 5);
        
        loadPythonSkill(toxic, "toxic");
        
        // Create fire moves for Charmander
        auto flamethrower = std::make_shared<Move>("Flamethrower", "flamethrower.py", 90, 100, "Fire", MoveCategory::SPECIAL);
        auto ember = std::make_shared<Move>("Ember", "", 40, 100, "Fire", MoveCategory::SPECIAL);
        auto scratch = std::make_shared<Move>("Scratch", "", 40, 100, "Normal", MoveCategory::PHYSICAL);
        
        loadPythonSkill(flamethrower, "flamethrower");
        
        // Assign moves to Pokemon
        pikachu->addMove(thunderbolt);
        pikachu->addMove(quickAttack);
        pikachu->addMove(thunderWave);
        
        squirtle->addMove(waterGun);
        squirtle->addMove(bubble);
        squirtle->addMove(withdraw);
        
        bulbasaur->addMove(vineWhip);
        bulbasaur->addMove(razorLeaf);
        bulbasaur->addMove(toxic);
        
        charmander->addMove(flamethrower);
        charmander->addMove(ember);
        charmander->addMove(scratch);
        
        std::cout << "\n✓ All Pokemon and moves created successfully!\n" << std::endl;
        
        // Let user choose battle matchup (for demo, we'll do multiple battles)
        std::vector<std::pair<std::shared_ptr<Pokemon>, std::shared_ptr<Pokemon>>> battles = {
            {pikachu, squirtle},   // Electric vs Water - type disadvantage for Pikachu
            {squirtle, charmander},  // Water vs Fire - type advantage for Squirtle
            {charmander, bulbasaur}  // Fire vs Grass - type advantage for Charmander
        };
        
        // Pick a random battle
        int battleChoice = rand() % battles.size();
        auto battle_pair = battles[battleChoice];
        auto pokemon1 = battle_pair.first;
        auto pokemon2 = battle_pair.second;
        
        std::cout << "Battle Selection: " << pokemon1->getName() << " vs " << pokemon2->getName() << "!" << std::endl;
        std::cout << "Type Matchup: " << pokemon1->getType() << " vs " << pokemon2->getType() << std::endl;
        
        // Create and start battle
        Battle battle(pokemon1, pokemon2);
        auto winner = battle.start();
        
        std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
        std::cout << "║         Battle Summary                 ║" << std::endl;
        std::cout << "╚════════════════════════════════════════╝" << std::endl;
        std::cout << "Winner: " << winner->getName() << " (" << winner->getType() << " type)" << std::endl;
        std::cout << "Remaining HP: " << winner->getCurrentHP() << "/" << winner->getMaxHP() << std::endl;
        std::cout << "HP Percentage: " << (winner->getCurrentHP() * 100 / winner->getMaxHP()) << "%" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        PythonSkillLoader::finalize();
        return 1;
    }
    
    // Finalize Python interpreter
    PythonSkillLoader::finalize();
    
    return 0;
}
