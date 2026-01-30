#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <vector>
#include <memory>

class Move;

class Pokemon {
private:
    std::string name;
    std::string type;
    int maxHP;
    int currentHP;
    int attack;
    int defense;
    int specialDefense;
    int speed;
    std::vector<std::shared_ptr<Move>> moves;
    std::string statusEffect;
    int statusDuration;

public:
    Pokemon(const std::string& name, const std::string& type, int hp, int atk, int def, int spDef, int spd);
    
    // Getters
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    int getMaxHP() const { return maxHP; }
    int getCurrentHP() const { return currentHP; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getSpecialDefense() const { return specialDefense; }
    int getSpeed() const { return speed; }
    const std::vector<std::shared_ptr<Move>>& getMoves() const { return moves; }
    std::string getStatusEffect() const { return statusEffect; }
    int getStatusDuration() const { return statusDuration; }
    
    // Battle methods
    void takeDamage(int damage);
    void heal(int amount);
    bool isFainted() const { return currentHP <= 0; }
    void addMove(std::shared_ptr<Move> move);
    void applyStatusEffect(const std::string& effect, int duration);
    void updateStatus();
    bool hasStatusEffect() const { return !statusEffect.empty(); }
    
    // Display
    void displayStatus() const;
};

#endif // POKEMON_H
