#include "Pokemon.h"
#include "Move.h"
#include <iostream>
#include <algorithm>

Pokemon::Pokemon(const std::string& name, const std::string& type, int hp, int atk, int def, int spDef, int spd)
    : name(name), type(type), maxHP(hp), currentHP(hp), attack(atk), defense(def), 
      specialDefense(spDef), speed(spd), statusEffect(""), statusDuration(0) {
}

void Pokemon::takeDamage(int damage) {
    currentHP = std::max(0, currentHP - damage);
}

void Pokemon::heal(int amount) {
    currentHP = std::min(maxHP, currentHP + amount);
}

void Pokemon::addMove(std::shared_ptr<Move> move) {
    moves.push_back(move);
}

void Pokemon::applyStatusEffect(const std::string& effect, int duration) {
    if (statusEffect.empty()) {
        statusEffect = effect;
        statusDuration = duration;
        std::cout << name << " is now " << effect << "!" << std::endl;
    }
}

void Pokemon::updateStatus() {
    if (statusEffect.empty()) return;
    
    if (statusEffect == "Poisoned") {
        int poisonDamage = maxHP / 8;
        takeDamage(poisonDamage);
        std::cout << name << " is hurt by poison! (-" << poisonDamage << " HP)" << std::endl;
    } else if (statusEffect == "Burned") {
        int burnDamage = maxHP / 16;
        takeDamage(burnDamage);
        std::cout << name << " is hurt by burn! (-" << burnDamage << " HP)" << std::endl;
    } else if (statusEffect == "Paralyzed") {
        std::cout << name << " is paralyzed!" << std::endl;
    }
    
    statusDuration--;
    if (statusDuration <= 0) {
        std::cout << name << " recovered from " << statusEffect << "!" << std::endl;
        statusEffect = "";
    }
}

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
