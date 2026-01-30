#include "Battle.h"
#include "Move.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Battle::Battle(std::shared_ptr<Pokemon> p1, std::shared_ptr<Pokemon> p2)
    : pokemon1(p1), pokemon2(p2) {
}

Pokemon& Battle::determineFirstAttacker() {
    // Faster Pokemon attacks first
    if (pokemon1->getSpeed() >= pokemon2->getSpeed()) {
        return *pokemon1;
    }
    return *pokemon2;
}

void Battle::executeTurn(Pokemon& attacker, Pokemon& defender, int moveIndex) {
    if (attacker.getMoves().empty()) {
        std::cout << attacker.getName() << " has no moves!" << std::endl;
        return;
    }
    
    // Check if paralyzed (50% chance to be unable to move)
    if (attacker.getStatusEffect() == "Paralyzed") {
        if (rand() % 100 < 50) {
            std::cout << attacker.getName() << " is fully paralyzed and can't move!" << std::endl;
            return;
        }
    }
    
    // Use the specified move (or first move if index out of bounds)
    if (moveIndex < 0 || moveIndex >= static_cast<int>(attacker.getMoves().size())) {
        moveIndex = 0;
    }
    
    auto move = attacker.getMoves()[moveIndex];
    move->execute(attacker, defender);
    
    // Apply status effect damage at end of turn
    attacker.updateStatus();
}

void Battle::displayBattleState() const {
    std::cout << "\n=== Battle State ===" << std::endl;
    pokemon1->displayStatus();
    std::cout << "VS" << std::endl;
    pokemon2->displayStatus();
    std::cout << "==================\n" << std::endl;
}

std::shared_ptr<Pokemon> Battle::start() {
    std::cout << "\n*** Battle Start! ***" << std::endl;
    std::cout << pokemon1->getName() << " vs " << pokemon2->getName() << "!" << std::endl;
    
    displayBattleState();
    
    int turn = 0;
    while (!pokemon1->isFainted() && !pokemon2->isFainted()) {
        std::cout << "\n--- Turn " << ++turn << " ---" << std::endl;
        
        // Determine turn order based on speed
        Pokemon& first = determineFirstAttacker();
        Pokemon& second = ((&first == pokemon1.get()) ? *pokemon2 : *pokemon1);
        
        // First attacker's turn
        std::cout << "\n" << first.getName() << "'s turn:" << std::endl;
        int firstMoveIndex = rand() % first.getMoves().size();
        executeTurn(first, second, firstMoveIndex);
        
        // Check if second Pokemon fainted
        if (second.isFainted()) {
            std::cout << "\n" << second.getName() << " fainted!" << std::endl;
            break;
        }
        
        // Second attacker's turn
        std::cout << "\n" << second.getName() << "'s turn:" << std::endl;
        int secondMoveIndex = rand() % second.getMoves().size();
        executeTurn(second, first, secondMoveIndex);
        
        // Check if first Pokemon fainted
        if (first.isFainted()) {
            std::cout << "\n" << first.getName() << " fainted!" << std::endl;
            break;
        }
        
        displayBattleState();
    }
    
    // Determine winner
    std::shared_ptr<Pokemon> winner = pokemon1->isFainted() ? pokemon2 : pokemon1;
    std::cout << "\n*** " << winner->getName() << " wins the battle! ***\n" << std::endl;
    
    return winner;
}
