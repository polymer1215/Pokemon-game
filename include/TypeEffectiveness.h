#ifndef TYPE_EFFECTIVENESS_H
#define TYPE_EFFECTIVENESS_H

#include <string>
#include <map>
#include <utility>

/**
 * TypeEffectiveness Class
 * 
 * Manages type matchup calculations for Pokemon battles.
 * Implements the type advantage/disadvantage system where certain types
 * deal more or less damage to other types.
 * 
 * Examples:
 * - Water beats Fire (2x damage)
 * - Fire beats Grass (2x damage)
 * - Electric has no effect on Ground (0x damage)
 */
class TypeEffectiveness {
private:
    /**
     * Map of type matchups to effectiveness multipliers
     * Key: (attacking type, defending type)
     * Value: multiplier (2.0 = super effective, 0.5 = not very effective, 0.0 = no effect)
     */
    static std::map<std::pair<std::string, std::string>, double> effectivenessChart;
    
    // Flag to ensure chart is only initialized once
    static bool initialized;
    
    /**
     * Initialize the type effectiveness chart with all matchups
     * Called automatically on first use
     */
    static void initializeChart();
    
public:
    /**
     * Get the type effectiveness multiplier for an attack
     * 
     * @param attackType Type of the attacking move (e.g., "Electric")
     * @param defenseType Type of the defending Pokemon (e.g., "Water")
     * @return Effectiveness multiplier:
     *         - 2.0: Super effective
     *         - 1.0: Normal effectiveness (default)
     *         - 0.5: Not very effective
     *         - 0.0: No effect (immune)
     */
    static double getEffectiveness(const std::string& attackType, const std::string& defenseType);
};

#endif // TYPE_EFFECTIVENESS_H
