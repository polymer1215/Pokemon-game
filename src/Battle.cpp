#include "Battle.h"
#include "Move.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constructor: Initialize battle with two Pokemon
Battle::Battle(std::shared_ptr<Pokemon> p1, std::shared_ptr<Pokemon> p2)
    : pokemon1(p1), pokemon2(p2) {
}

// Determine which Pokemon attacks first based on Speed stat
Pokemon& Battle::determineFirstAttacker() {
    // Higher Speed attacks first
    // In case of tie, pokemon1 goes first
    if (pokemon1->getSpeed() >= pokemon2->getSpeed()) {
        return *pokemon1;
    }
    return *pokemon2;
}

// Execute a single turn for one Pokemon
void Battle::executeTurn(Pokemon& attacker, Pokemon& defender, int moveIndex) {
    // Check if attacker has any moves
    if (attacker.getMoves().empty()) {
        std::cout << attacker.getName() << " has no moves!" << std::endl;
        return;
    }
    
    // Check paralysis - 50% chance to be fully paralyzed
    if (attacker.getStatusEffect() == "Paralyzed") {
        if (rand() % 100 < 50) {
            std::cout << attacker.getName() << " is fully paralyzed and can't move!" << std::endl;
            return;
        }
    }
    
    // Validate move index, default to first move if invalid
    if (moveIndex < 0 || moveIndex >= static_cast<int>(attacker.getMoves().size())) {
        moveIndex = 0;
    }
    
    // Execute the selected move
    auto move = attacker.getMoves()[moveIndex];
    move->execute(attacker, defender);
    
    // Apply status effect damage/effects at end of turn
    attacker.updateStatus();
}

// Display current state of both Pokemon
void Battle::displayBattleState() const {
    std::cout << "\n=== Battle State ===" << std::endl;
    pokemon1->displayStatus();
    std::cout << "VS" << std::endl;
    pokemon2->displayStatus();
    std::cout << "==================\n" << std::endl;
}

// Start the battle and run until one Pokemon faints
std::shared_ptr<Pokemon> Battle::start() {
    std::cout << "\n*** Battle Start! ***" << std::endl;
    std::cout << pokemon1->getName() << " vs " << pokemon2->getName() << "!" << std::endl;
    
    // Display initial battle state
    displayBattleState();
    
    int turn = 0;
    
    // Battle loop: continue until one Pokemon faints
    while (!pokemon1->isFainted() && !pokemon2->isFainted()) {
        std::cout << "\n--- Turn " << ++turn << " ---" << std::endl;
        
        // Determine turn order based on Speed stat
        Pokemon& first = determineFirstAttacker();
        Pokemon& second = ((&first == pokemon1.get()) ? *pokemon2 : *pokemon1);
        
        // First attacker's turn
        std::cout << "\n" << first.getName() << "'s turn:" << std::endl;
        // Randomly select a move (in a real game, this would be player/AI choice)
        int firstMoveIndex = rand() % first.getMoves().size();
        executeTurn(first, second, firstMoveIndex);
        
        // Check if second Pokemon fainted from the attack
        if (second.isFainted()) {
            std::cout << "\n" << second.getName() << " fainted!" << std::endl;
            break;
        }
        
        // Second attacker's turn
        std::cout << "\n" << second.getName() << "'s turn:" << std::endl;
        int secondMoveIndex = rand() % second.getMoves().size();
        executeTurn(second, first, secondMoveIndex);
        
        // Check if first Pokemon fainted from the counter-attack
        if (first.isFainted()) {
            std::cout << "\n" << first.getName() << " fainted!" << std::endl;
            break;
        }
        
        // Display updated battle state
        displayBattleState();
    }
    
    // Determine and announce winner
    std::shared_ptr<Pokemon> winner = pokemon1->isFainted() ? pokemon2 : pokemon1;
    std::cout << "\n*** " << winner->getName() << " wins the battle! ***\n" << std::endl;
    
    return winner;
}
