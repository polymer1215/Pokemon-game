#include "Move.h"
#include "Pokemon.h"
#include "TypeEffectiveness.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Move::Move(const std::string& name, const std::string& scriptPath, 
           int power, int accuracy, const std::string& type, MoveCategory cat,
           const std::string& status, int duration)
    : name(name), scriptPath(scriptPath), basePower(power), accuracy(accuracy), 
      type(type), category(cat), statusEffect(status), statusDuration(duration) {
    // Default effect function (basic damage calculation)
    effectFunction = [this](Pokemon& attacker, Pokemon& defender) -> int {
        if (this->basePower == 0) return 0; // Status moves
        
        // Calculate base damage based on move category
        int attackStat = (this->category == MoveCategory::SPECIAL) ? attacker.getAttack() : attacker.getAttack();
        int defenseStat = (this->category == MoveCategory::SPECIAL) ? defender.getSpecialDefense() : defender.getDefense();
        
        // Simple damage formula: (Attack * Power / Defense) / 2
        int damage = (attackStat * this->basePower) / (defenseStat * 2);
        damage = std::max(1, damage); // Minimum 1 damage
        
        // Apply type effectiveness
        double effectiveness = TypeEffectiveness::getEffectiveness(this->type, defender.getType());
        damage = static_cast<int>(damage * effectiveness);
        
        return damage;
    };
}

int Move::execute(Pokemon& attacker, Pokemon& defender) {
    // Check accuracy
    int roll = rand() % 100;
    if (roll >= accuracy) {
        std::cout << attacker.getName() << "'s " << name << " missed!" << std::endl;
        return 0;
    }
    
    std::cout << attacker.getName() << " used " << name << "!" << std::endl;
    
    // Execute the effect function
    int damage = effectFunction(attacker, defender);
    
    if (damage > 0) {
        // Damaging move
        double effectiveness = TypeEffectiveness::getEffectiveness(type, defender.getType());
        
        defender.takeDamage(damage);
        std::cout << "It dealt " << damage << " damage!" << std::endl;
        
        if (effectiveness > 1.0) {
            std::cout << "It's super effective!" << std::endl;
        } else if (effectiveness < 1.0 && effectiveness > 0.0) {
            std::cout << "It's not very effective..." << std::endl;
        } else if (effectiveness == 0.0) {
            std::cout << "It doesn't affect " << defender.getName() << "..." << std::endl;
        }
    } else if (damage < 0) {
        // Healing move
        attacker.heal(-damage);
        std::cout << attacker.getName() << " restored " << (-damage) << " HP!" << std::endl;
    }
    
    // Apply status effect if any
    if (!statusEffect.empty() && category == MoveCategory::STATUS) {
        defender.applyStatusEffect(statusEffect, statusDuration);
    }
    
    return damage;
}

void Move::setEffectFunction(std::function<int(Pokemon&, Pokemon&)> func) {
    effectFunction = func;
}
