#ifndef BATTLE_H
#define BATTLE_H

#include "Pokemon.h"
#include <memory>

/**
 * Battle Class
 * 
 * Manages turn-based battle logic between two Pokemon.
 * Handles turn order, move execution, status effects, and victory conditions.
 */
class Battle {
private:
    std::shared_ptr<Pokemon> pokemon1;  // First Pokemon in battle
    std::shared_ptr<Pokemon> pokemon2;  // Second Pokemon in battle
    
    /**
     * Execute a single Pokemon's turn
     * Handles paralysis check, move execution, and status updates
     * 
     * @param attacker Pokemon executing their turn
     * @param defender Pokemon being targeted
     * @param moveIndex Index of move to use (0-based)
     */
    void executeTurn(Pokemon& attacker, Pokemon& defender, int moveIndex);
    
    /**
     * Determine which Pokemon attacks first based on Speed stat
     * Higher speed attacks first; ties go to Pokemon 1
     * 
     * @return Reference to Pokemon that attacks first
     */
    Pokemon& determineFirstAttacker();

public:
    /**
     * Constructor
     * Creates a new Battle between two Pokemon
     * 
     * @param p1 First Pokemon
     * @param p2 Second Pokemon
     */
    Battle(std::shared_ptr<Pokemon> p1, std::shared_ptr<Pokemon> p2);
    
    /**
     * Start and run the battle until one Pokemon faints
     * 
     * Battle flow:
     * 1. Display initial state
     * 2. Each turn:
     *    - Determine turn order by Speed
     *    - Fast Pokemon attacks
     *    - Check if slow Pokemon fainted
     *    - Slow Pokemon attacks (if alive)
     *    - Check if fast Pokemon fainted
     *    - Apply status effect damage
     *    - Display battle state
     * 3. Return winner
     * 
     * @return Shared pointer to the winning Pokemon
     */
    std::shared_ptr<Pokemon> start();
    
    /**
     * Display current state of both Pokemon in battle
     * Shows HP, status effects, stats, and available moves
     */
    void displayBattleState() const;
};

#endif // BATTLE_H
