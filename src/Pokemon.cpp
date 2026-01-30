#include "Pokemon.h"
#include "Move.h"
#include <iostream>
#include <algorithm>

// Constructor: Initialize Pokemon with stats
Pokemon::Pokemon(const std::string& name, const std::string& type, int hp, int atk, int def, int spDef, int spd)
    : name(name), type(type), maxHP(hp), currentHP(hp), attack(atk), defense(def), 
      specialDefense(spDef), speed(spd), statusEffect(""), statusDuration(0) {
}

// Reduce HP by damage amount, minimum 0
void Pokemon::takeDamage(int damage) {
    currentHP = std::max(0, currentHP - damage);
}

// Restore HP by amount, maximum maxHP
void Pokemon::heal(int amount) {
    currentHP = std::min(maxHP, currentHP + amount);
}

// Add a move to this Pokemon's moveset
void Pokemon::addMove(std::shared_ptr<Move> move) {
    moves.push_back(move);
}

// Apply a status effect if Pokemon doesn't already have one
void Pokemon::applyStatusEffect(const std::string& effect, int duration) {
    if (statusEffect.empty()) {
        statusEffect = effect;
        statusDuration = duration;
        std::cout << name << " is now " << effect << "!" << std::endl;
    }
}

// Update status effect: apply damage and decrement duration
void Pokemon::updateStatus() {
    if (statusEffect.empty()) return;
    
    // Apply status effect damage/effects
    if (statusEffect == "Poisoned") {
        // Poison deals 1/8 max HP per turn
        int poisonDamage = maxHP / 8;
        takeDamage(poisonDamage);
        std::cout << name << " is hurt by poison! (-" << poisonDamage << " HP)" << std::endl;
    } else if (statusEffect == "Burned") {
        // Burn deals 1/16 max HP per turn
        int burnDamage = maxHP / 16;
        takeDamage(burnDamage);
        std::cout << name << " is hurt by burn! (-" << burnDamage << " HP)" << std::endl;
    } else if (statusEffect == "Paralyzed") {
        // Paralysis message (immobilization handled in Battle class)
        std::cout << name << " is paralyzed!" << std::endl;
    }
    
    // Decrement duration and clear if expired
    statusDuration--;
    if (statusDuration <= 0) {
        std::cout << name << " recovered from " << statusEffect << "!" << std::endl;
        statusEffect = "";
    }
}

// Display Pokemon's current battle status
void Pokemon::displayStatus() const {
    std::cout << name << " (" << type << " type) - HP: " << currentHP << "/" << maxHP;
    if (!statusEffect.empty()) {
        std::cout << " [" << statusEffect << "]";
    }
    std::cout << std::endl;
    std::cout << "Stats - ATK: " << attack << ", DEF: " << defense 
              << ", SP.DEF: " << specialDefense << ", SPD: " << speed << std::endl;
    std::cout << "Moves: ";
    for (size_t i = 0; i < moves.size(); ++i) {
        std::cout << moves[i]->getName();
        if (i < moves.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}
