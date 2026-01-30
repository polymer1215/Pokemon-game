# API Documentation

This document provides detailed API documentation for all classes in the Pokemon Battle Game.

## Table of Contents

- [Pokemon Class](#pokemon-class)
- [Move Class](#move-class)
- [Battle Class](#battle-class)
- [TypeEffectiveness Class](#typeeffectiveness-class)
- [PythonSkillLoader Class](#pythonskillloader-class)
- [Enumerations](#enumerations)

---

## Pokemon Class

Represents a Pokemon with stats, moves, and status effects.

**Header:** `include/Pokemon.h`  
**Source:** `src/Pokemon.cpp`

### Constructor

```cpp
Pokemon(const std::string& name, const std::string& type, int hp, int atk, int def, int spDef, int spd)
```

Creates a new Pokemon instance.

**Parameters:**
- `name` - The name of the Pokemon (e.g., "Pikachu")
- `type` - The type of the Pokemon (e.g., "Electric", "Water", "Fire")
- `hp` - Maximum hit points
- `atk` - Attack stat (used for damage calculation)
- `def` - Defense stat (reduces physical damage)
- `spDef` - Special Defense stat (reduces special damage)
- `spd` - Speed stat (determines turn order)

**Example:**
```cpp
auto pikachu = std::make_shared<Pokemon>("Pikachu", "Electric", 100, 55, 40, 50, 90);
```

### Getters

#### `std::string getName() const`
Returns the Pokemon's name.

#### `std::string getType() const`
Returns the Pokemon's type (e.g., "Electric", "Water").

#### `int getMaxHP() const`
Returns the Pokemon's maximum hit points.

#### `int getCurrentHP() const`
Returns the Pokemon's current hit points.

#### `int getAttack() const`
Returns the Pokemon's attack stat.

#### `int getDefense() const`
Returns the Pokemon's defense stat.

#### `int getSpecialDefense() const`
Returns the Pokemon's special defense stat.

#### `int getSpeed() const`
Returns the Pokemon's speed stat (determines turn order).

#### `const std::vector<std::shared_ptr<Move>>& getMoves() const`
Returns a reference to the vector of moves this Pokemon knows.

#### `std::string getStatusEffect() const`
Returns the current status effect (e.g., "Poisoned", "Paralyzed") or empty string if none.

#### `int getStatusDuration() const`
Returns the remaining duration of the current status effect in turns.

### Battle Methods

#### `void takeDamage(int damage)`
Reduces the Pokemon's current HP by the specified damage amount. HP cannot go below 0.

**Parameters:**
- `damage` - Amount of damage to apply

**Example:**
```cpp
pokemon->takeDamage(30);
```

#### `void heal(int amount)`
Restores the Pokemon's HP by the specified amount. HP cannot exceed maximum HP.

**Parameters:**
- `amount` - Amount of HP to restore

**Example:**
```cpp
pokemon->heal(50);
```

#### `bool isFainted() const`
Returns true if the Pokemon's current HP is 0 or less.

**Returns:** `true` if fainted, `false` otherwise

#### `void addMove(std::shared_ptr<Move> move)`
Adds a move to the Pokemon's moveset.

**Parameters:**
- `move` - Shared pointer to the Move to add

**Example:**
```cpp
auto thunderbolt = std::make_shared<Move>("Thunderbolt", "thunderbolt.py", 90, 100, "Electric", MoveCategory::SPECIAL);
pikachu->addMove(thunderbolt);
```

#### `void applyStatusEffect(const std::string& effect, int duration)`
Applies a status effect to the Pokemon if it doesn't already have one.

**Parameters:**
- `effect` - Name of the status effect (e.g., "Poisoned", "Paralyzed", "Burned")
- `duration` - Number of turns the effect lasts

**Example:**
```cpp
pokemon->applyStatusEffect("Poisoned", 5);
```

#### `void updateStatus()`
Updates the current status effect, applying damage or effects and decrementing duration. Called at the end of each turn.

**Status Effects:**
- **Poisoned**: Takes 1/8 of max HP as damage
- **Burned**: Takes 1/16 of max HP as damage
- **Paralyzed**: 50% chance to be unable to move (checked in Battle class)

#### `bool hasStatusEffect() const`
Returns true if the Pokemon currently has a status effect.

**Returns:** `true` if status effect is active, `false` otherwise

### Display Methods

#### `void displayStatus() const`
Prints the Pokemon's current status including name, type, HP, stats, and moves to console.

**Example Output:**
```
Pikachu (Electric type) - HP: 85/100 [Paralyzed]
Stats - ATK: 55, DEF: 40, SP.DEF: 50, SPD: 90
Moves: Thunderbolt, Quick Attack, Thunder Wave
```

---

## Move Class

Represents a battle move with damage calculation, accuracy, and effects.

**Header:** `include/Move.h`  
**Source:** `src/Move.cpp`

### Constructor

```cpp
Move(const std::string& name, const std::string& scriptPath, 
     int power, int accuracy, const std::string& type, MoveCategory cat = MoveCategory::PHYSICAL,
     const std::string& status = "", int duration = 0)
```

Creates a new Move instance.

**Parameters:**
- `name` - Name of the move (e.g., "Thunderbolt")
- `scriptPath` - Name of Python script in scripts/ directory (e.g., "thunderbolt", without .py extension)
- `power` - Base power of the move (0 for status moves)
- `accuracy` - Accuracy percentage (0-100)
- `type` - Type of the move (must match Pokemon types)
- `cat` - Move category (PHYSICAL, SPECIAL, or STATUS)
- `status` - Optional status effect to apply (e.g., "Paralyzed")
- `duration` - Duration of status effect in turns

**Examples:**
```cpp
// Physical attack move
auto scratch = std::make_shared<Move>("Scratch", "", 40, 100, "Normal", MoveCategory::PHYSICAL);

// Special attack move with Python script (note: no .py extension in script name)
auto thunderbolt = std::make_shared<Move>("Thunderbolt", "thunderbolt", 90, 100, "Electric", MoveCategory::SPECIAL);

// Status move
auto thunderWave = std::make_shared<Move>("Thunder Wave", "thunder_wave", 0, 100, "Electric", MoveCategory::STATUS, "Paralyzed", 4);
```

### Getters

#### `std::string getName() const`
Returns the move's name.

#### `int getBasePower() const`
Returns the move's base power (0 for status moves).

#### `int getAccuracy() const`
Returns the move's accuracy percentage (0-100).

#### `std::string getType() const`
Returns the move's type.

#### `MoveCategory getCategory() const`
Returns the move's category (PHYSICAL, SPECIAL, or STATUS).

#### `std::string getScriptPath() const`
Returns the path to the Python script for custom effects.

#### `std::string getStatusEffect() const`
Returns the name of the status effect this move applies (empty if none).

#### `int getStatusDuration() const`
Returns the duration of the status effect in turns.

### Methods

#### `int execute(Pokemon& attacker, Pokemon& defender)`
Executes the move, applying damage and/or status effects.

**Parameters:**
- `attacker` - The Pokemon using the move
- `defender` - The Pokemon being targeted

**Returns:** Damage dealt (0 if missed or status move, negative for healing)

**Process:**
1. Check accuracy (may miss)
2. Calculate damage using effect function
3. Apply type effectiveness multiplier
4. Deal damage to defender
5. Apply status effect if applicable
6. Print appropriate messages

**Example:**
```cpp
int damage = thunderbolt->execute(*pikachu, *squirtle);
```

#### `void setEffectFunction(std::function<int(Pokemon&, Pokemon&)> func)`
Sets a custom effect function for the move (typically loaded from Python script).

**Parameters:**
- `func` - Function that takes attacker and defender references and returns damage amount

**Example:**
```cpp
auto skillFunc = PythonSkillLoader::loadSkill("thunderbolt", "calculate_damage");
thunderbolt->setEffectFunction(skillFunc);
```

---

## Battle Class

Manages turn-based battle logic between two Pokemon.

**Header:** `include/Battle.h`  
**Source:** `src/Battle.cpp`

### Constructor

```cpp
Battle(std::shared_ptr<Pokemon> p1, std::shared_ptr<Pokemon> p2)
```

Creates a new Battle instance between two Pokemon.

**Parameters:**
- `p1` - First Pokemon
- `p2` - Second Pokemon

**Example:**
```cpp
Battle battle(pikachu, squirtle);
```

### Methods

#### `std::shared_ptr<Pokemon> start()`
Starts the battle and runs until one Pokemon faints.

**Returns:** Shared pointer to the winning Pokemon

**Process:**
1. Display initial battle state
2. Each turn:
   - Determine turn order based on Speed stat
   - Execute first attacker's move
   - Check if second Pokemon fainted
   - Execute second attacker's move
   - Check if first Pokemon fainted
   - Apply status effect damage
   - Display updated battle state
3. Return winner

**Example:**
```cpp
auto winner = battle.start();
std::cout << winner->getName() << " wins!" << std::endl;
```

#### `void displayBattleState() const`
Displays the current state of both Pokemon in the battle.

---

## TypeEffectiveness Class

Manages type effectiveness calculations for damage multipliers.

**Header:** `include/TypeEffectiveness.h`  
**Source:** `src/TypeEffectiveness.cpp`

### Static Methods

#### `static double getEffectiveness(const std::string& attackType, const std::string& defenseType)`
Returns the type effectiveness multiplier for an attack.

**Parameters:**
- `attackType` - Type of the attacking move
- `defenseType` - Type of the defending Pokemon

**Returns:**
- `2.0` - Super effective
- `1.0` - Normal effectiveness
- `0.5` - Not very effective
- `0.0` - No effect

**Example:**
```cpp
double multiplier = TypeEffectiveness::getEffectiveness("Electric", "Water");
// Returns 2.0 (Electric is super effective against Water)
```

### Type Effectiveness Chart

| Attacking Type | Super Effective (2x) | Not Very Effective (0.5x) | No Effect (0x) |
|---------------|---------------------|---------------------------|----------------|
| Fire | Grass, Ice, Bug | Water, Fire, Rock | - |
| Water | Fire, Ground, Rock | Water, Grass | - |
| Grass | Water, Ground, Rock | Fire, Grass, Flying, Bug | - |
| Electric | Water, Flying | Electric, Grass | Ground |
| Ice | Grass, Ground, Flying, Dragon | Fire, Water, Ice | - |
| Fighting | Normal, Ice, Rock | Flying, Psychic | - |
| Ground | Fire, Electric, Rock | Grass, Bug | Flying |
| Flying | Grass, Fighting, Bug | Electric, Rock | - |
| Rock | Fire, Ice, Flying, Bug | Fighting, Ground | - |
| Bug | Grass, Psychic | Fire, Fighting, Flying | - |
| Psychic | Fighting | Psychic | - |
| Dragon | Dragon | - | - |
| Normal | - | - | - |

---

## PythonSkillLoader Class

Integrates Python scripts for custom move effects.

**Header:** `include/PythonSkillLoader.h`  
**Source:** `src/PythonSkillLoader.cpp`

### Static Methods

#### `static void initialize()`
Initializes the Python interpreter. Must be called before loading any Python skills.

**Example:**
```cpp
PythonSkillLoader::initialize();
```

#### `static void finalize()`
Finalizes and shuts down the Python interpreter. Should be called before program exit.

**Example:**
```cpp
PythonSkillLoader::finalize();
```

#### `static std::function<int(Pokemon&, Pokemon&)> loadSkill(const std::string& scriptPath, const std::string& functionName)`
Loads a skill function from a Python script and returns it as a C++ function.

**Parameters:**
- `scriptPath` - Name of the Python script file (without .py extension, from scripts/ directory)
- `functionName` - Name of the function to load (typically "calculate_damage")

**Returns:** Function object that can be called with attacker and defender Pokemon

**Example:**
```cpp
auto skillFunc = PythonSkillLoader::loadSkill("thunderbolt", "calculate_damage");
move->setEffectFunction(skillFunc);
```

**Python Script Format:**
```python
def calculate_damage(attacker, defender):
    """
    Args:
        attacker: dict with keys: name, current_hp, max_hp, attack, defense, speed
        defender: dict with keys: name, current_hp, max_hp, attack, defense, speed
    Returns:
        int: damage amount (positive for damage, negative for healing, 0 for status-only)
    """
    # Your calculation logic
    return damage
```

#### `static int executeSkill(const std::string& scriptPath, const std::string& functionName, Pokemon& attacker, Pokemon& defender)`
Directly executes a Python skill script without wrapping it in a function object.

**Parameters:**
- `scriptPath` - Name of the Python script file
- `functionName` - Name of the function to execute
- `attacker` - Attacking Pokemon
- `defender` - Defending Pokemon

**Returns:** Damage amount from the skill calculation

---

## Enumerations

### MoveCategory

Defines the category of a move, which affects which stats are used in damage calculation.

**Defined in:** `include/Move.h`

```cpp
enum class MoveCategory {
    PHYSICAL,   // Uses Attack vs Defense
    SPECIAL,    // Uses Attack vs Special Defense
    STATUS      // No damage, applies status effects or other effects
};
```

**Usage:**
```cpp
auto physicalMove = std::make_shared<Move>("Scratch", "", 40, 100, "Normal", MoveCategory::PHYSICAL);
auto specialMove = std::make_shared<Move>("Thunderbolt", "", 90, 100, "Electric", MoveCategory::SPECIAL);
auto statusMove = std::make_shared<Move>("Thunder Wave", "", 0, 100, "Electric", MoveCategory::STATUS);
```

---

## Complete Example

Here's a complete example demonstrating the API:

```cpp
#include "Pokemon.h"
#include "Move.h"
#include "Battle.h"
#include "PythonSkillLoader.h"

int main() {
    // Initialize Python
    PythonSkillLoader::initialize();
    
    // Create Pokemon
    auto pikachu = std::make_shared<Pokemon>("Pikachu", "Electric", 100, 55, 40, 50, 90);
    auto squirtle = std::make_shared<Pokemon>("Squirtle", "Water", 120, 48, 65, 64, 43);
    
    // Create moves
    auto thunderbolt = std::make_shared<Move>("Thunderbolt", "thunderbolt.py", 90, 100, "Electric", MoveCategory::SPECIAL);
    auto waterGun = std::make_shared<Move>("Water Gun", "water_gun.py", 40, 100, "Water", MoveCategory::SPECIAL);
    
    // Load Python skills
    auto tbFunc = PythonSkillLoader::loadSkill("thunderbolt", "calculate_damage");
    thunderbolt->setEffectFunction(tbFunc);
    
    auto wgFunc = PythonSkillLoader::loadSkill("water_gun", "calculate_damage");
    waterGun->setEffectFunction(wgFunc);
    
    // Assign moves to Pokemon
    pikachu->addMove(thunderbolt);
    squirtle->addMove(waterGun);
    
    // Start battle
    Battle battle(pikachu, squirtle);
    auto winner = battle.start();
    
    std::cout << winner->getName() << " wins!" << std::endl;
    
    // Cleanup
    PythonSkillLoader::finalize();
    
    return 0;
}
```

---

## Error Handling

### Common Errors

1. **Python initialization failure**: Ensure Python development headers are installed
2. **Script not found**: Python scripts must be in the `scripts/` directory
3. **Function not found**: Ensure the Python script has the correct function name
4. **Invalid damage values**: Python scripts should return valid integers

### Exception Safety

- Most methods do not throw exceptions but use defensive programming
- Python-related functions may throw `std::runtime_error` if scripts fail to load
- Always call `PythonSkillLoader::finalize()` before program exit

---

## Performance Considerations

- **Type effectiveness lookups**: O(1) using hash map
- **Move execution**: O(1) for single move
- **Battle simulation**: O(n) where n is number of turns
- **Python script loading**: Done once per move at startup
- **Memory**: Pokemon and Move objects use shared_ptr for efficient memory management

---

## Thread Safety

The current implementation is **not thread-safe**. For concurrent battles:

1. Create separate Battle instances
2. Do not share Pokemon objects between threads
3. Initialize Python interpreter once in main thread
4. Consider using mutex locks for Python calls

---

For more information, see:
- [README.md](../README.md) - Project overview and quick start
- [CONTRIBUTING.md](../CONTRIBUTING.md) - Development guidelines
- Python scripts in `scripts/` directory for skill implementation examples
