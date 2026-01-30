# Contributing to Pokemon Battle Game

Thank you for your interest in contributing to the Pokemon Battle Game! This document provides guidelines and instructions for contributing to this project.

## Table of Contents

- [Development Environment Setup](#development-environment-setup)
- [Project Architecture](#project-architecture)
- [Coding Standards](#coding-standards)
- [Adding New Features](#adding-new-features)
- [Testing Guidelines](#testing-guidelines)
- [Submitting Changes](#submitting-changes)

## Development Environment Setup

### Prerequisites

- **C++ Compiler**: GCC 7+, Clang 5+, or MSVC 2017+
- **CMake**: Version 3.10 or higher
- **Python**: Version 3.6 or higher with development headers
- **Git**: For version control

### Setting Up Your Development Environment

1. **Clone the repository**:
   ```bash
   git clone https://github.com/polymer1215/Pokemon-game.git
   cd Pokemon-game
   ```

2. **Install Python development headers**:
   
   On Ubuntu/Debian:
   ```bash
   sudo apt-get install python3-dev
   ```
   
   On macOS:
   ```bash
   brew install python3
   ```
   
   On Windows:
   - Install Python from [python.org](https://www.python.org/)
   - Make sure to check "Install development headers" during installation

3. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

4. **Run the game**:
   ```bash
   ./pokemon_battle
   ```

## Project Architecture

### Directory Structure

```
Pokemon-game/
├── include/           # Header files
│   ├── Battle.h
│   ├── Move.h
│   ├── Pokemon.h
│   ├── PythonSkillLoader.h
│   └── TypeEffectiveness.h
├── src/              # Source files
│   ├── Battle.cpp
│   ├── Move.cpp
│   ├── Pokemon.cpp
│   ├── PythonSkillLoader.cpp
│   ├── TypeEffectiveness.cpp
│   └── main.cpp
├── scripts/          # Python skill scripts
│   ├── thunderbolt.py
│   ├── water_gun.py
│   ├── flamethrower.py
│   └── ...
└── CMakeLists.txt    # Build configuration
```

### Core Components

1. **Pokemon Class**: Represents a Pokemon with stats, moves, and status effects
2. **Move Class**: Represents a battle move with damage calculation and effects
3. **Battle Class**: Manages the battle logic and turn execution
4. **TypeEffectiveness Class**: Handles type advantage/disadvantage calculations
5. **PythonSkillLoader Class**: Integrates Python scripts for custom skill effects

## Coding Standards

### C++ Style Guidelines

- **Naming Conventions**:
  - Classes: PascalCase (e.g., `Pokemon`, `TypeEffectiveness`)
  - Functions/Methods: camelCase (e.g., `takeDamage`, `getCurrentHP`)
  - Variables: camelCase (e.g., `currentHP`, `maxHP`)
  - Constants: UPPER_SNAKE_CASE (e.g., `MAX_MOVES`)
  - Private members: camelCase (same as variables)

- **Code Formatting**:
  - Indentation: 4 spaces (no tabs)
  - Braces: Opening brace on same line for functions and control structures
  - Maximum line length: 100 characters

- **Comments**:
  - Use `//` for single-line comments
  - Use `/* */` for multi-line comments
  - Document all public methods with brief descriptions
  - Explain complex algorithms and non-obvious code

### Python Style Guidelines

For Python skill scripts, follow PEP 8:

- **Function Names**: snake_case (e.g., `calculate_damage`)
- **Indentation**: 4 spaces
- **Docstrings**: Use triple quotes for function documentation
- **Comments**: Explain the logic and calculations

Example Python skill script:

```python
"""
Skill Name - Type type attack
Brief description of what this skill does
"""

def calculate_damage(attacker, defender):
    """
    Calculate damage for [Skill Name] attack
    
    Args:
        attacker: Dictionary with attacker's stats
            Keys: name, current_hp, max_hp, attack, defense, speed
        defender: Dictionary with defender's stats
            
    Returns:
        int: Damage amount (positive for damage, negative for healing, 0 for status-only)
    """
    # Your calculation logic here
    base_power = 50
    damage = (attacker['attack'] * base_power) / defender['defense']
    return int(damage)
```

## Adding New Features

### Adding a New Pokemon

1. Open `src/main.cpp`
2. Create a new Pokemon instance with appropriate stats:

```cpp
auto newPokemon = std::make_shared<Pokemon>(
    "PokemonName",   // Name
    "Type",          // Type (Electric, Water, Fire, Grass, etc.)
    100,             // Max HP
    50,              // Attack
    45,              // Defense
    50,              // Special Defense
    70               // Speed
);
```

3. Create and assign moves to the Pokemon
4. Add the Pokemon to the battle lineup

### Adding a New Move

1. **Create the Python script** (optional, for custom effects):
   
   Create a new file in `scripts/` directory:
   
   ```python
   # scripts/my_new_move.py
   def calculate_damage(attacker, defender):
       # Custom damage calculation
       base_power = 80
       damage = (attacker['attack'] * base_power) / defender['defense']
       return int(damage)
   ```

2. **Create the Move in C++**:
   
   ```cpp
   // Physical move
   auto newMove = std::make_shared<Move>(
       "Move Name",           // Name
       "my_new_move.py",      // Python script (or "" for default)
       80,                    // Base power
       100,                   // Accuracy (0-100)
       "Type",                // Type
       MoveCategory::PHYSICAL // PHYSICAL, SPECIAL, or STATUS
   );
   
   // Status move
   auto statusMove = std::make_shared<Move>(
       "Status Move",
       "status_move.py",
       0,                     // Base power (0 for status)
       90,                    // Accuracy
       "Type",
       MoveCategory::STATUS,
       "Paralyzed",          // Status effect to apply
       4                      // Duration in turns
   );
   ```

3. **Load the Python skill** (if using custom script):
   
   ```cpp
   loadPythonSkill(newMove, "my_new_move");
   ```

4. **Assign to Pokemon**:
   
   ```cpp
   pokemon->addMove(newMove);
   ```

### Adding New Pokemon Types

1. Open `src/TypeEffectiveness.cpp`
2. Add effectiveness entries in the `initializeChart()` function:

```cpp
// NewType vs OtherType
effectivenessChart[{"NewType", "OtherType"}] = 2.0;  // Super effective
effectivenessChart[{"NewType", "AnotherType"}] = 0.5; // Not very effective
effectivenessChart[{"NewType", "YetAnotherType"}] = 0.0; // No effect
```

### Adding New Status Effects

1. Open `src/Pokemon.cpp`
2. Add the new status effect in the `updateStatus()` method:

```cpp
void Pokemon::updateStatus() {
    if (statusEffect.empty()) return;
    
    if (statusEffect == "NewStatus") {
        // Implement the status effect logic
        int damage = maxHP / 10;
        takeDamage(damage);
        std::cout << name << " is affected by " << statusEffect << "! (-" << damage << " HP)" << std::endl;
    }
    // ... existing status effects ...
    
    statusDuration--;
    if (statusDuration <= 0) {
        std::cout << name << " recovered from " << statusEffect << "!" << std::endl;
        statusEffect = "";
    }
}
```

## Testing Guidelines

### Manual Testing

After making changes, always test your modifications:

1. **Build the project**:
   ```bash
   cd build
   cmake --build .
   ```

2. **Run the game**:
   ```bash
   ./pokemon_battle
   ```

3. **Verify your changes**:
   - Check that new Pokemon/moves appear correctly
   - Verify damage calculations are reasonable
   - Ensure status effects work as expected
   - Test type effectiveness combinations

### Test Checklist

Before submitting changes, ensure:

- [ ] Code compiles without errors or warnings
- [ ] Game runs without crashes
- [ ] New features work as intended
- [ ] Existing functionality is not broken
- [ ] Python scripts are syntactically correct
- [ ] Documentation is updated

## Submitting Changes

### Pull Request Process

1. **Fork the repository** on GitHub

2. **Create a feature branch**:
   ```bash
   git checkout -b feature/my-new-feature
   ```

3. **Make your changes** following the coding standards

4. **Test your changes** thoroughly

5. **Commit your changes** with clear messages:
   ```bash
   git add .
   git commit -m "Add new feature: description"
   ```

6. **Push to your fork**:
   ```bash
   git push origin feature/my-new-feature
   ```

7. **Create a Pull Request** on GitHub with:
   - Clear title describing the change
   - Detailed description of what was changed and why
   - Any testing performed
   - Screenshots/examples if applicable

### Commit Message Guidelines

- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Move cursor to..." not "Moves cursor to...")
- Keep first line under 72 characters
- Reference issues and pull requests when relevant

Examples:
```
Add Psychic type effectiveness rules
Fix paralysis chance calculation
Update README with build instructions
```

## Questions or Issues?

If you have questions or need help:

1. Check existing issues on GitHub
2. Create a new issue with details about your question
3. Reach out to project maintainers

Thank you for contributing to the Pokemon Battle Game!
