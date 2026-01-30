#include "Move.h"
#include "Pokemon.h"
#include "TypeEffectiveness.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constructor: Initialize move with properties and default effect function
Move::Move(const std::string& name, const std::string& scriptPath, 
           int power, int accuracy, const std::string& type, MoveCategory cat,
           const std::string& status, int duration)
    : name(name), scriptPath(scriptPath), basePower(power), accuracy(accuracy), 
      type(type), category(cat), statusEffect(status), statusDuration(duration) {
    
    // Set default effect function (basic damage calculation)
    // This will be replaced if a Python script is loaded
    effectFunction = [this](Pokemon& attacker, Pokemon& defender) -> int {
        // Status moves don't deal damage
        if (this->basePower == 0) return 0;
        
        // Determine which stats to use based on move category
        // Note: Current implementation uses same 'attack' stat for both physical and special moves
        // Only the defensive stat differs (defense for physical, special_defense for special)
        int attackStat = attacker.getAttack();
        int defenseStat = (this->category == MoveCategory::SPECIAL) ? defender.getSpecialDefense() : defender.getDefense();
        
        // Simple damage formula: (Attack * Power / Defense) / 2
        int damage = (attackStat * this->basePower) / (defenseStat * 2);
        damage = std::max(1, damage); // Minimum 1 damage
        
        // Note: Type effectiveness is applied in execute(), not here
        
        return damage;
    };
}

// Execute the move in battle
int Move::execute(Pokemon& attacker, Pokemon& defender) {
    // Step 1: Check if move hits based on accuracy
    int roll = rand() % 100;
    if (roll >= accuracy) {
        std::cout << attacker.getName() << "'s " << name << " missed!" << std::endl;
        return 0;
    }
    
    std::cout << attacker.getName() << " used " << name << "!" << std::endl;
    
    // Step 2: Calculate damage using effect function (Python or default)
    int damage = effectFunction(attacker, defender);
    
    if (damage > 0) {
        // Step 3: Apply type effectiveness multiplier for damaging moves
        double effectiveness = TypeEffectiveness::getEffectiveness(type, defender.getType());
        damage = static_cast<int>(damage * effectiveness);
        
        // Step 4: Deal damage to defender
        defender.takeDamage(damage);
        std::cout << "It dealt " << damage << " damage!" << std::endl;
        
        // Step 5: Display effectiveness message
        if (effectiveness > 1.0) {
            std::cout << "It's super effective!" << std::endl;
        } else if (effectiveness < 1.0 && effectiveness > 0.0) {
            std::cout << "It's not very effective..." << std::endl;
        } else if (effectiveness == 0.0) {
            std::cout << "It doesn't affect " << defender.getName() << "..." << std::endl;
        }
    } else if (damage < 0) {
        // Negative damage = healing move
        attacker.heal(-damage);
        std::cout << attacker.getName() << " restored " << (-damage) << " HP!" << std::endl;
    }
    
    // Step 6: Apply status effect if this is a status move
    if (!statusEffect.empty() && category == MoveCategory::STATUS) {
        defender.applyStatusEffect(statusEffect, statusDuration);
    }
    
    return damage;
}

// Set custom effect function (typically loaded from Python)
void Move::setEffectFunction(std::function<int(Pokemon&, Pokemon&)> func) {
    effectFunction = func;
}
