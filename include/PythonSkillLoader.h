#ifndef PYTHON_SKILL_LOADER_H
#define PYTHON_SKILL_LOADER_H

#include <string>
#include <functional>
#include <Python.h>

// Forward declaration
class Pokemon;

/**
 * PythonSkillLoader Class
 * 
 * Provides integration between C++ and Python for custom move effects.
 * Allows moves to use Python scripts for flexible damage calculations
 * and complex effects without recompiling C++ code.
 * 
 * Usage:
 * 1. Call initialize() at program start
 * 2. Use loadSkill() to load Python functions
 * 3. Call finalize() before program exit
 */
class PythonSkillLoader {
private:
    // Flag to track Python interpreter state
    static bool pythonInitialized;
    
public:
    /**
     * Initialize Python interpreter
     * Must be called before any Python operations
     * Safe to call multiple times (only initializes once)
     */
    static void initialize();
    
    /**
     * Finalize Python interpreter
     * Should be called before program exit to clean up Python resources
     * Safe to call multiple times (only finalizes once)
     */
    static void finalize();
    
    /**
     * Load a skill function from a Python script
     * 
     * The Python script should be in the scripts/ directory and contain
     * a function with the specified name that takes attacker and defender
     * dictionaries and returns an integer damage value.
     * 
     * Python function signature:
     *   def calculate_damage(attacker, defender):
     *       return int(damage)
     * 
     * @param scriptPath Name of Python file without .py extension (e.g., "thunderbolt")
     * @param functionName Name of function to load (typically "calculate_damage")
     * @return Function object that can be called with Pokemon references
     * @throws std::runtime_error if script or function cannot be loaded
     */
    static std::function<int(Pokemon&, Pokemon&)> loadSkill(const std::string& scriptPath, const std::string& functionName);
    
    /**
     * Execute Python skill directly without creating a function object
     * Useful for one-time calculations or testing
     * 
     * @param scriptPath Name of Python file without .py extension
     * @param functionName Name of function to execute
     * @param attacker Attacking Pokemon
     * @param defender Defending Pokemon
     * @return Damage value calculated by Python function
     * @throws std::runtime_error if execution fails
     */
    static int executeSkill(const std::string& scriptPath, const std::string& functionName,
                           Pokemon& attacker, Pokemon& defender);
};

#endif // PYTHON_SKILL_LOADER_H
