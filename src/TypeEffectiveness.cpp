#include "TypeEffectiveness.h"
#include <iostream>

// Static member initialization
std::map<std::pair<std::string, std::string>, double> TypeEffectiveness::effectivenessChart;
bool TypeEffectiveness::initialized = false;

/**
 * Initialize the type effectiveness chart
 * This function populates the map with all type matchup multipliers
 * 
 * Multipliers:
 * - 2.0: Super effective (attacker has advantage)
 * - 1.0: Normal effectiveness (default, not listed in chart)
 * - 0.5: Not very effective (attacker has disadvantage)
 * - 0.0: No effect (attacker is completely ineffective)
 */
void TypeEffectiveness::initializeChart() {
    // Only initialize once
    if (initialized) return;
    
    // Fire type effectiveness
    effectivenessChart[{"Fire", "Water"}] = 0.5;      // Fire vs Water: Not very effective
    effectivenessChart[{"Fire", "Grass"}] = 2.0;      // Fire vs Grass: Super effective
    effectivenessChart[{"Fire", "Fire"}] = 0.5;       // Fire vs Fire: Not very effective
    effectivenessChart[{"Fire", "Ice"}] = 2.0;        // Fire vs Ice: Super effective
    effectivenessChart[{"Fire", "Bug"}] = 2.0;        // Fire vs Bug: Super effective
    effectivenessChart[{"Fire", "Rock"}] = 0.5;       // Fire vs Rock: Not very effective
    
    // Water type effectiveness
    effectivenessChart[{"Water", "Fire"}] = 2.0;      // Water vs Fire: Super effective
    effectivenessChart[{"Water", "Water"}] = 0.5;     // Water vs Water: Not very effective
    effectivenessChart[{"Water", "Grass"}] = 0.5;     // Water vs Grass: Not very effective
    effectivenessChart[{"Water", "Ground"}] = 2.0;    // Water vs Ground: Super effective
    effectivenessChart[{"Water", "Rock"}] = 2.0;      // Water vs Rock: Super effective
    
    // Grass type effectiveness
    effectivenessChart[{"Grass", "Water"}] = 2.0;     // Grass vs Water: Super effective
    effectivenessChart[{"Grass", "Ground"}] = 2.0;    // Grass vs Ground: Super effective
    effectivenessChart[{"Grass", "Rock"}] = 2.0;      // Grass vs Rock: Super effective
    effectivenessChart[{"Grass", "Fire"}] = 0.5;      // Grass vs Fire: Not very effective
    effectivenessChart[{"Grass", "Grass"}] = 0.5;     // Grass vs Grass: Not very effective
    effectivenessChart[{"Grass", "Flying"}] = 0.5;    // Grass vs Flying: Not very effective
    effectivenessChart[{"Grass", "Bug"}] = 0.5;       // Grass vs Bug: Not very effective
    
    // Electric type effectiveness
    effectivenessChart[{"Electric", "Water"}] = 2.0;  // Electric vs Water: Super effective
    effectivenessChart[{"Electric", "Flying"}] = 2.0; // Electric vs Flying: Super effective
    effectivenessChart[{"Electric", "Electric"}] = 0.5; // Electric vs Electric: Not very effective
    effectivenessChart[{"Electric", "Grass"}] = 0.5;  // Electric vs Grass: Not very effective
    effectivenessChart[{"Electric", "Ground"}] = 0.0; // Electric vs Ground: No effect
    
    // Ice type effectiveness
    effectivenessChart[{"Ice", "Grass"}] = 2.0;       // Ice vs Grass: Super effective
    effectivenessChart[{"Ice", "Ground"}] = 2.0;      // Ice vs Ground: Super effective
    effectivenessChart[{"Ice", "Flying"}] = 2.0;      // Ice vs Flying: Super effective
    effectivenessChart[{"Ice", "Dragon"}] = 2.0;      // Ice vs Dragon: Super effective
    effectivenessChart[{"Ice", "Fire"}] = 0.5;        // Ice vs Fire: Not very effective
    effectivenessChart[{"Ice", "Water"}] = 0.5;       // Ice vs Water: Not very effective
    effectivenessChart[{"Ice", "Ice"}] = 0.5;         // Ice vs Ice: Not very effective
    
    // Fighting type effectiveness
    effectivenessChart[{"Fighting", "Normal"}] = 2.0; // Fighting vs Normal: Super effective
    effectivenessChart[{"Fighting", "Ice"}] = 2.0;    // Fighting vs Ice: Super effective
    effectivenessChart[{"Fighting", "Rock"}] = 2.0;   // Fighting vs Rock: Super effective
    effectivenessChart[{"Fighting", "Flying"}] = 0.5; // Fighting vs Flying: Not very effective
    effectivenessChart[{"Fighting", "Psychic"}] = 0.5; // Fighting vs Psychic: Not very effective
    
    // Ground type effectiveness
    effectivenessChart[{"Ground", "Fire"}] = 2.0;     // Ground vs Fire: Super effective
    effectivenessChart[{"Ground", "Electric"}] = 2.0; // Ground vs Electric: Super effective
    effectivenessChart[{"Ground", "Rock"}] = 2.0;     // Ground vs Rock: Super effective
    effectivenessChart[{"Ground", "Grass"}] = 0.5;    // Ground vs Grass: Not very effective
    effectivenessChart[{"Ground", "Bug"}] = 0.5;      // Ground vs Bug: Not very effective
    effectivenessChart[{"Ground", "Flying"}] = 0.0;   // Ground vs Flying: No effect
    
    // Flying type effectiveness
    effectivenessChart[{"Flying", "Grass"}] = 2.0;    // Flying vs Grass: Super effective
    effectivenessChart[{"Flying", "Fighting"}] = 2.0; // Flying vs Fighting: Super effective
    effectivenessChart[{"Flying", "Bug"}] = 2.0;      // Flying vs Bug: Super effective
    effectivenessChart[{"Flying", "Electric"}] = 0.5; // Flying vs Electric: Not very effective
    effectivenessChart[{"Flying", "Rock"}] = 0.5;     // Flying vs Rock: Not very effective
    
    // Rock type effectiveness
    effectivenessChart[{"Rock", "Fire"}] = 2.0;       // Rock vs Fire: Super effective
    effectivenessChart[{"Rock", "Ice"}] = 2.0;        // Rock vs Ice: Super effective
    effectivenessChart[{"Rock", "Flying"}] = 2.0;     // Rock vs Flying: Super effective
    effectivenessChart[{"Rock", "Bug"}] = 2.0;        // Rock vs Bug: Super effective
    effectivenessChart[{"Rock", "Fighting"}] = 0.5;   // Rock vs Fighting: Not very effective
    effectivenessChart[{"Rock", "Ground"}] = 0.5;     // Rock vs Ground: Not very effective
    
    // Bug type effectiveness
    effectivenessChart[{"Bug", "Grass"}] = 2.0;       // Bug vs Grass: Super effective
    effectivenessChart[{"Bug", "Psychic"}] = 2.0;     // Bug vs Psychic: Super effective
    effectivenessChart[{"Bug", "Fire"}] = 0.5;        // Bug vs Fire: Not very effective
    effectivenessChart[{"Bug", "Fighting"}] = 0.5;    // Bug vs Fighting: Not very effective
    effectivenessChart[{"Bug", "Flying"}] = 0.5;      // Bug vs Flying: Not very effective
    
    // Psychic type effectiveness
    effectivenessChart[{"Psychic", "Fighting"}] = 2.0; // Psychic vs Fighting: Super effective
    effectivenessChart[{"Psychic", "Psychic"}] = 0.5;  // Psychic vs Psychic: Not very effective
    
    // Dragon type effectiveness
    effectivenessChart[{"Dragon", "Dragon"}] = 2.0;    // Dragon vs Dragon: Super effective
    
    // Mark as initialized
    initialized = true;
}

/**
 * Get the type effectiveness multiplier for an attack
 * 
 * @param attackType Type of the attacking move
 * @param defenseType Type of the defending Pokemon
 * @return Effectiveness multiplier (2.0, 1.0, 0.5, or 0.0)
 */
double TypeEffectiveness::getEffectiveness(const std::string& attackType, const std::string& defenseType) {
    // Ensure chart is initialized
    initializeChart();
    
    // Normal type attacks are always neutral (1.0x) against all types
    if (attackType == "Normal") return 1.0;
    
    // Look up the matchup in the chart
    auto key = std::make_pair(attackType, defenseType);
    auto it = effectivenessChart.find(key);
    
    if (it != effectivenessChart.end()) {
        // Found a specific matchup
        return it->second;
    }
    
    // Default to normal effectiveness (1.0x) if no specific matchup exists
    return 1.0;
}
