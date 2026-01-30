#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <vector>
#include <memory>

// Forward declaration to avoid circular dependency
class Move;

/**
 * Pokemon Class
 * 
 * Represents a Pokemon with stats, moves, and status effects.
 * Manages HP, combat stats, moveset, and status conditions during battle.
 */
class Pokemon {
private:
    std::string name;              // Pokemon's name (e.g., "Pikachu")
    std::string type;              // Pokemon's type (e.g., "Electric", "Water")
    int maxHP;                     // Maximum hit points
    int currentHP;                 // Current hit points (0 = fainted)
    int attack;                    // Attack stat (used for damage calculation)
    int defense;                   // Defense stat (reduces physical damage)
    int specialDefense;            // Special Defense stat (reduces special damage)
    int speed;                     // Speed stat (determines turn order)
    std::vector<std::shared_ptr<Move>> moves;  // List of moves this Pokemon knows
    std::string statusEffect;      // Current status effect ("Poisoned", "Paralyzed", etc.)
    int statusDuration;            // Remaining turns for status effect

public:
    /**
     * Constructor
     * Creates a new Pokemon with specified stats
     * 
     * @param name Pokemon's name
     * @param type Pokemon's type (must match types in TypeEffectiveness)
     * @param hp Maximum and initial HP
     * @param atk Attack stat
     * @param def Defense stat
     * @param spDef Special Defense stat
     * @param spd Speed stat (higher = attacks first)
     */
    Pokemon(const std::string& name, const std::string& type, int hp, int atk, int def, int spDef, int spd);
    
    // ===== Getters =====
    // These methods provide read-only access to Pokemon's attributes
    
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
    
    // ===== Battle Methods =====
    
    /**
     * Reduces Pokemon's HP by damage amount
     * HP cannot go below 0
     * 
     * @param damage Amount of damage to apply
     */
    void takeDamage(int damage);
    
    /**
     * Restores Pokemon's HP by specified amount
     * HP cannot exceed maxHP
     * 
     * @param amount Amount of HP to restore
     */
    void heal(int amount);
    
    /**
     * Checks if Pokemon has fainted (HP <= 0)
     * 
     * @return true if fainted, false otherwise
     */
    bool isFainted() const { return currentHP <= 0; }
    
    /**
     * Adds a move to Pokemon's moveset
     * Pokemon can learn any number of moves (no limit in current implementation)
     * 
     * @param move Shared pointer to the Move to add
     */
    void addMove(std::shared_ptr<Move> move);
    
    /**
     * Applies a status effect to the Pokemon
     * Only works if Pokemon doesn't already have a status effect
     * 
     * @param effect Name of status effect ("Poisoned", "Paralyzed", "Burned")
     * @param duration Number of turns the effect lasts
     */
    void applyStatusEffect(const std::string& effect, int duration);
    
    /**
     * Updates the Pokemon's status effect
     * Applies damage/effects and decrements duration
     * Called at the end of each turn
     * 
     * Effects:
     * - Poisoned: 1/8 max HP damage per turn
     * - Burned: 1/16 max HP damage per turn
     * - Paralyzed: Handled in Battle class (50% immobilization)
     */
    void updateStatus();
    
    /**
     * Checks if Pokemon currently has a status effect
     * 
     * @return true if status effect is active, false otherwise
     */
    bool hasStatusEffect() const { return !statusEffect.empty(); }
    
    // ===== Display =====
    
    /**
     * Displays Pokemon's current status to console
     * Shows name, type, HP, stats, status effects, and moves
     */
    void displayStatus() const;
};

#endif // POKEMON_H
