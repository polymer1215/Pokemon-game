#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <functional>

// Forward declaration
class Pokemon;

/**
 * MoveCategory Enumeration
 * 
 * Defines the category of a move, which determines which stats are used
 * in damage calculation and how the move behaves.
 */
enum class MoveCategory {
    PHYSICAL,   // Uses Attack vs Defense (e.g., Scratch, Tackle)
    SPECIAL,    // Uses Attack vs Special Defense (e.g., Thunderbolt, Flamethrower)
                // Note: Both categories use the same Attack stat; only defensive stat differs
    STATUS      // No direct damage, applies status effects or other effects
};

/**
 * Move Class
 * 
 * Represents a battle move with damage calculation, accuracy, type, and effects.
 * Supports custom Python scripts for flexible damage calculation.
 */
class Move {
private:
    std::string name;              // Name of the move (e.g., "Thunderbolt")
    std::string scriptPath;        // Path to Python script for custom effects
    int basePower;                 // Base power (0 for status moves)
    int accuracy;                  // Accuracy percentage (0-100)
    std::string type;              // Move type (must match Pokemon types)
    MoveCategory category;         // PHYSICAL, SPECIAL, or STATUS
    std::string statusEffect;      // Status effect to apply (empty if none)
    int statusDuration;            // Duration of status effect in turns
    
    /**
     * Function to execute Python skill script or default calculation
     * Takes attacker and defender, returns damage amount
     * - Positive: damage to defender
     * - Negative: healing to attacker
     * - Zero: status effect only
     */
    std::function<int(Pokemon&, Pokemon&)> effectFunction;

public:
    /**
     * Constructor
     * Creates a new Move with specified properties
     * 
     * @param name Display name of the move
     * @param scriptPath Python script filename (empty for default calculation)
     * @param power Base power (0 for status moves, typically 40-150 for attacks)
     * @param accuracy Hit chance percentage (0-100)
     * @param type Move type (e.g., "Electric", "Water", "Fire")
     * @param cat Move category (PHYSICAL, SPECIAL, or STATUS)
     * @param status Status effect to apply (e.g., "Paralyzed", empty if none)
     * @param duration Duration of status effect in turns
     */
    Move(const std::string& name, const std::string& scriptPath, 
         int power, int accuracy, const std::string& type, MoveCategory cat = MoveCategory::PHYSICAL,
         const std::string& status = "", int duration = 0);
    
    // ===== Getters =====
    
    std::string getName() const { return name; }
    int getBasePower() const { return basePower; }
    int getAccuracy() const { return accuracy; }
    std::string getType() const { return type; }
    MoveCategory getCategory() const { return category; }
    std::string getScriptPath() const { return scriptPath; }
    std::string getStatusEffect() const { return statusEffect; }
    int getStatusDuration() const { return statusDuration; }
    
    // ===== Execution =====
    
    /**
     * Execute the move in battle
     * 
     * Process:
     * 1. Check accuracy (may miss)
     * 2. Calculate damage using effect function
     * 3. Apply type effectiveness
     * 4. Deal damage or heal
     * 5. Apply status effect if applicable
     * 
     * @param attacker Pokemon using the move
     * @param defender Pokemon being targeted
     * @return Damage dealt (0 if missed or status move, negative for healing)
     */
    int execute(Pokemon& attacker, Pokemon& defender);
    
    /**
     * Set custom effect function (typically loaded from Python script)
     * 
     * @param func Function that calculates damage based on attacker/defender
     */
    void setEffectFunction(std::function<int(Pokemon&, Pokemon&)> func);
};

#endif // MOVE_H
