#ifndef TYPE_EFFECTIVENESS_H
#define TYPE_EFFECTIVENESS_H

#include <string>
#include <map>
#include <utility>

class TypeEffectiveness {
private:
    // Map of (attacking type, defending type) -> effectiveness multiplier
    static std::map<std::pair<std::string, std::string>, double> effectivenessChart;
    static bool initialized;
    
    static void initializeChart();
    
public:
    // Get the type effectiveness multiplier
    // Returns 2.0 for super effective, 0.5 for not very effective, 0.0 for no effect, 1.0 for normal
    static double getEffectiveness(const std::string& attackType, const std::string& defenseType);
};

#endif // TYPE_EFFECTIVENESS_H
