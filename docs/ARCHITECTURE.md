# Architecture Overview

This document provides a detailed overview of the Pokemon Battle Game architecture, including class relationships, data flow, and system design.

## High-Level Architecture

```
┌─────────────────────────────────────────────────────┐
│                   Pokemon Battle Game                │
│                                                       │
│  ┌─────────────┐         ┌──────────────┐          │
│  │   main.cpp  │────────▶│    Battle    │          │
│  └─────────────┘         └──────────────┘          │
│         │                       │                    │
│         │                       │                    │
│         ▼                       ▼                    │
│  ┌─────────────┐         ┌──────────────┐          │
│  │  Python     │         │   Pokemon    │          │
│  │ SkillLoader │         └──────────────┘          │
│  └─────────────┘               │                    │
│         │                       │                    │
│         │                       ▼                    │
│         │                 ┌──────────────┐          │
│         │                 │     Move     │          │
│         │                 └──────────────┘          │
│         │                       │                    │
│         └───────────────────────┘                    │
│                                 │                    │
│                                 ▼                    │
│                       ┌───────────────────┐         │
│                       │ TypeEffectiveness │         │
│                       └───────────────────┘         │
│                                                       │
│  ┌──────────────────────────────────────────────┐  │
│  │          Python Scripts (scripts/)            │  │
│  │  ┌──────────┐  ┌──────────┐  ┌──────────┐   │  │
│  │  │thunderbolt│  │water_gun │  │   toxic  │...│  │
│  │  └──────────┘  └──────────┘  └──────────┘   │  │
│  └──────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────┘
```

## Core Components

### 1. Pokemon Class

**Responsibilities:**
- Store Pokemon attributes (name, type, stats, HP)
- Manage moveset
- Handle status effects
- Track HP and battle status

**Key Relationships:**
- Contains: Multiple `Move` objects
- Used by: `Battle` class
- Related to: `TypeEffectiveness` (via type)

**State Management:**
```
┌───────────────────┐
│     Pokemon       │
├───────────────────┤
│ - name            │
│ - type            │
│ - maxHP           │
│ - currentHP       │
│ - attack          │
│ - defense         │
│ - specialDefense  │
│ - speed           │
│ - moves[]         │◀─── Contains Move objects
│ - statusEffect    │
│ - statusDuration  │
└───────────────────┘
```

### 2. Move Class

**Responsibilities:**
- Store move properties (name, type, power, accuracy)
- Execute damage calculation (via Python or default)
- Apply status effects
- Handle type effectiveness

**Key Relationships:**
- Used by: `Pokemon` class
- Uses: `TypeEffectiveness` for damage multipliers
- Uses: `PythonSkillLoader` for custom effects
- Interacts with: Python scripts

**Move Execution Flow:**
```
Move.execute()
    │
    ├─▶ Check accuracy (may miss)
    │
    ├─▶ Call effectFunction()
    │   └─▶ Python script or default calculation
    │
    ├─▶ Apply type effectiveness
    │   └─▶ TypeEffectiveness.getEffectiveness()
    │
    ├─▶ Deal damage to defender
    │
    └─▶ Apply status effect (if STATUS move)
```

### 3. Battle Class

**Responsibilities:**
- Manage battle flow between two Pokemon
- Handle turn order based on Speed
- Execute turns
- Check victory conditions
- Display battle state

**Key Relationships:**
- Contains: Two `Pokemon` objects
- Uses: `Move` execution
- Manages: Turn-based combat logic

**Battle Loop:**
```
Battle.start()
    │
    ├─▶ Display initial state
    │
    └─▶ While both Pokemon alive:
        │
        ├─▶ Determine turn order (by Speed)
        │
        ├─▶ Fast Pokemon attacks
        │   └─▶ executeTurn()
        │       ├─▶ Check paralysis
        │       ├─▶ Execute move
        │       └─▶ Update status
        │
        ├─▶ Check if slow Pokemon fainted
        │
        ├─▶ Slow Pokemon attacks (if alive)
        │   └─▶ executeTurn()
        │
        ├─▶ Check if fast Pokemon fainted
        │
        └─▶ Display battle state
```

### 4. TypeEffectiveness Class

**Responsibilities:**
- Store type matchup chart
- Calculate effectiveness multipliers
- Provide type advantage/disadvantage system

**Key Relationships:**
- Used by: `Move` class for damage calculation
- Singleton-like behavior with static members

**Type Matchup System:**
```
TypeEffectiveness.getEffectiveness(attackType, defenseType)
    │
    ├─▶ Initialize chart (if first call)
    │
    ├─▶ Lookup (attackType, defenseType) in map
    │
    └─▶ Return multiplier:
        - 2.0: Super effective
        - 1.0: Normal (default)
        - 0.5: Not very effective
        - 0.0: No effect
```

### 5. PythonSkillLoader Class

**Responsibilities:**
- Initialize Python interpreter
- Load Python skill scripts
- Convert Python functions to C++ callables
- Pass Pokemon data to Python
- Finalize Python interpreter

**Key Relationships:**
- Used by: `main.cpp` for initialization
- Used by: `Move` class for custom effects
- Interacts with: Python scripts in `scripts/` directory

**Python Integration Flow:**
```
PythonSkillLoader.loadSkill(script, function)
    │
    ├─▶ Load Python module from scripts/
    │
    ├─▶ Get function reference
    │
    └─▶ Return C++ lambda that:
        │
        ├─▶ Create Python dictionaries for attacker/defender
        │   (name, hp, attack, defense, speed, etc.)
        │
        ├─▶ Call Python function with dictionaries
        │
        ├─▶ Extract damage value from Python result
        │
        └─▶ Return damage as int
```

## Data Flow

### Battle Turn Data Flow

```
1. main.cpp creates Pokemon with moves
           │
           ▼
2. Python scripts loaded for custom moves
           │
           ▼
3. Battle initialized with two Pokemon
           │
           ▼
4. Battle loop begins:
           │
           ├─▶ Determine turn order (Speed comparison)
           │
           ├─▶ First Pokemon's turn:
           │   │
           │   ├─▶ Check status (paralysis, etc.)
           │   │
           │   ├─▶ Select move (random or player choice)
           │   │
           │   ├─▶ Move.execute():
           │   │   │
           │   │   ├─▶ Check accuracy
           │   │   │
           │   │   ├─▶ Calculate damage:
           │   │   │   │
           │   │   │   ├─▶ If Python script: call Python function
           │   │   │   │   └─▶ Python calculates damage
           │   │   │   │
           │   │   │   └─▶ If default: use C++ calculation
           │   │   │
           │   │   ├─▶ Apply type effectiveness multiplier
           │   │   │
           │   │   ├─▶ Deal damage to defender
           │   │   │
           │   │   └─▶ Apply status effect (if any)
           │   │
           │   └─▶ Update attacker's status effects
           │
           ├─▶ Check if defender fainted
           │
           ├─▶ Second Pokemon's turn (same as above)
           │
           ├─▶ Check if first Pokemon fainted
           │
           └─▶ Display battle state
           │
           ▼
5. Return winner when one Pokemon faints
```

### Python Script Execution Flow

```
C++ Move.execute()
      │
      ├─▶ effectFunction(attacker, defender)
      │   │
      │   └─▶ [Lambda created by PythonSkillLoader]
      │       │
      │       ├─▶ Convert Pokemon to Python dict:
      │       │   {
      │       │     'name': 'Pikachu',
      │       │     'current_hp': 85,
      │       │     'max_hp': 100,
      │       │     'attack': 55,
      │       │     'defense': 40,
      │       │     'special_defense': 50,
      │       │     'speed': 90
      │       │   }
      │       │
      │       ├─▶ Call Python function:
      │       │   damage = calculate_damage(attacker_dict, defender_dict)
      │       │
      │       └─▶ Convert Python int to C++ int
      │           │
      │           └─▶ Return damage value
      │
      └─▶ Apply damage and type effectiveness
```

## Class Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                        Class Diagram                         │
└─────────────────────────────────────────────────────────────┘

    ┌──────────────────┐
    │      Battle      │
    ├──────────────────┤
    │ - pokemon1       │──────┐
    │ - pokemon2       │      │
    ├──────────────────┤      │
    │ + start()        │      │
    │ + displayState() │      │
    └──────────────────┘      │
                              │
                              │ contains 2
                              │
                              ▼
    ┌──────────────────┐     ┌─────────────────────┐
    │  PythonSkill     │     │      Pokemon        │
    │     Loader       │     ├─────────────────────┤
    ├──────────────────┤     │ - name: string      │
    │ + initialize()   │     │ - type: string      │
    │ + loadSkill()    │     │ - hp: int           │
    │ + finalize()     │     │ - attack: int       │
    └──────────────────┘     │ - defense: int      │
            │                │ - speed: int        │
            │                │ - moves: vector     │◀─┐
            │                │ - statusEffect      │  │
            │                ├─────────────────────┤  │
            │                │ + takeDamage()      │  │
            │                │ + heal()            │  │
            │                │ + addMove()         │  │
            │                │ + updateStatus()    │  │
            │                └─────────────────────┘  │
            │                                          │
            │                                          │ contains
            │                                          │ multiple
            │                ┌─────────────────────┐  │
            │                │       Move          │──┘
            │                ├─────────────────────┤
            └───────────────▶│ - name: string      │
                 uses        │ - type: string      │
                             │ - power: int        │
                             │ - accuracy: int     │
                             │ - category: enum    │
                             │ - effectFunction    │
                             ├─────────────────────┤
                             │ + execute()         │
                             │ + setEffectFunc()   │
                             └─────────────────────┘
                                      │
                                      │ uses
                                      ▼
                             ┌─────────────────────┐
                             │ TypeEffectiveness   │
                             ├─────────────────────┤
                             │ - chart: map        │
                             ├─────────────────────┤
                             │ + getEffectiveness()│
                             └─────────────────────┘
```

## Design Patterns

### 1. Strategy Pattern (Move Effects)

Moves use the Strategy pattern for damage calculation:
- **Context**: Move class
- **Strategy**: effectFunction (function pointer)
- **Concrete Strategies**: Python scripts or default calculation

**Benefits:**
- Flexible move behavior without modifying C++ code
- Easy to add new moves
- Runtime script modification

### 2. Singleton-like Pattern (TypeEffectiveness)

TypeEffectiveness uses static members and lazy initialization:
- **Static data**: effectivenessChart
- **Lazy init**: Chart populated on first access
- **Global access**: Any Move can query type effectiveness

**Benefits:**
- Single source of truth for type matchups
- Memory efficient (one chart for all moves)
- Easy to query from anywhere

### 3. Composite Pattern (Pokemon + Moves)

Pokemon contains a collection of Move objects:
- **Component**: Move interface
- **Composite**: Pokemon with vector of Moves

**Benefits:**
- Pokemon manages its moveset
- Flexible number of moves
- Easy to add/remove moves

## Extension Points

### Adding New Features

1. **New Pokemon Types**
   - Add entries to `TypeEffectiveness::initializeChart()`
   - Create Pokemon with new type
   - No other changes needed

2. **New Moves**
   - Create Python script with `calculate_damage()` function
   - Create Move object with script path
   - Load with PythonSkillLoader
   - Add to Pokemon's moveset

3. **New Status Effects**
   - Add case in `Pokemon::updateStatus()`
   - Create status move with new effect name
   - No other changes needed

4. **New Battle Mechanics**
   - Modify `Battle::executeTurn()` for turn logic
   - Extend `Pokemon` class for new attributes
   - Use Python scripts for complex calculations

### Potential Improvements

1. **Multi-Pokemon Battles**
   - Extend Battle to support teams
   - Add switching logic
   - Manage multiple active Pokemon

2. **AI System**
   - Create AI strategy classes
   - Implement move selection algorithms
   - Add difficulty levels

3. **Item System**
   - Create Item class
   - Add inventory to Pokemon/Battle
   - Implement item effects

4. **Experience/Leveling**
   - Add XP tracking to Pokemon
   - Implement level-up mechanics
   - Dynamic stat growth

5. **Abilities**
   - Add ability system to Pokemon
   - Create ability effect framework
   - Integrate with battle logic

## Performance Considerations

### Time Complexity

- **Type effectiveness lookup**: O(1) - hash map lookup
- **Move execution**: O(1) - direct function call
- **Battle turn**: O(n) where n = number of moves (for random selection)
- **Full battle**: O(t) where t = number of turns

### Memory Usage

- **Pokemon**: ~200 bytes + moves vector
- **Move**: ~100 bytes + function pointer
- **TypeEffectiveness**: ~2KB for full chart (shared)
- **Battle**: 2 × Pokemon size

### Python Integration Cost

- **Initialization**: One-time cost at startup
- **Script loading**: One-time per move type
- **Function call**: Minimal overhead (cached function reference)
- **Data conversion**: Small overhead for dictionary creation

### Optimization Tips

1. **Avoid repeated Python initialization**: Call once at startup
2. **Cache loaded Python functions**: Load each script once
3. **Minimize Python calls**: Use C++ for simple calculations
4. **Pre-calculate type matchups**: Static chart initialized once

## Thread Safety

**Current Status**: Not thread-safe

**Potential Issues:**
- Python interpreter is not thread-safe without GIL management
- Shared Pokemon state between threads
- TypeEffectiveness static members

**Making It Thread-Safe:**
1. Add mutex for Python calls
2. Clone Pokemon for concurrent battles
3. Use thread-local storage or separate Battle instances
4. Acquire Python GIL for interpreter calls

## Testing Strategy

### Unit Tests (Not yet implemented)

Recommended test coverage:
- **Pokemon**: HP management, status effects, move addition
- **Move**: Damage calculation, accuracy checks, type effectiveness
- **Battle**: Turn order, victory conditions, status application
- **TypeEffectiveness**: All type matchups
- **PythonSkillLoader**: Script loading, function execution

### Integration Tests

- Complete battles with various Pokemon
- Status effect chains
- Type advantage scenarios
- Python script failures

### Manual Testing

Current approach: Run complete battles and observe output

## File Organization

```
Pokemon-game/
│
├── include/              # Header files (.h)
│   ├── Battle.h          # Battle management
│   ├── Move.h            # Move definitions
│   ├── Pokemon.h         # Pokemon class
│   ├── PythonSkillLoader.h  # Python integration
│   └── TypeEffectiveness.h  # Type matchups
│
├── src/                  # Implementation files (.cpp)
│   ├── Battle.cpp
│   ├── Move.cpp
│   ├── Pokemon.cpp
│   ├── PythonSkillLoader.cpp
│   ├── TypeEffectiveness.cpp
│   └── main.cpp          # Entry point
│
├── scripts/              # Python skill scripts (.py)
│   ├── thunderbolt.py
│   ├── water_gun.py
│   ├── flamethrower.py
│   ├── toxic.py
│   ├── heal.py
│   ├── slash.py
│   ├── eruption.py
│   ├── electro_ball.py
│   └── leech_seed.py
│
├── docs/                 # Documentation
│   ├── API.md            # API reference
│   ├── PYTHON_SKILLS.md  # Python guide
│   └── ARCHITECTURE.md   # This file
│
├── CMakeLists.txt        # Build configuration
├── README.md             # Project overview
└── CONTRIBUTING.md       # Contribution guidelines
```

## Conclusion

This architecture provides:
- **Modularity**: Clear separation of concerns
- **Extensibility**: Easy to add new features
- **Flexibility**: Python scripts for custom logic
- **Simplicity**: Straightforward class hierarchy
- **Performance**: Efficient type lookups and battle execution

The design balances simplicity with extensibility, making it easy to understand while providing powerful customization through Python integration.

For more details on specific components:
- [API Documentation](API.md) - Detailed class and method reference
- [Python Skills Guide](PYTHON_SKILLS.md) - How to create custom moves
- [Contributing Guide](../CONTRIBUTING.md) - Development setup and standards
