# Pokemon Battle Game

一个类似宝可梦的精灵对战游戏，使用 C++ 实现核心战斗逻辑，使用 Python 脚本定义技能效果。

A Pokemon-like sprite battle game with C++ core battle logic and Python scripts for skill effects.

## Features

- **核心战斗系统 (Core Battle System)**: 完整的回合制战斗逻辑
- **Python 技能集成 (Python Skill Integration)**: 使用 Python 脚本定义精灵技能，方便扩展复杂效果
- **属性克制系统 (Type Effectiveness System)**: 完整的属性相克系统，如水克火、火克草等
- **状态效果 (Status Effects)**: 中毒、麻痹、灼烧等状态效果
- **技能分类 (Move Categories)**: 物理、特殊和状态技能
- **多样化精灵 (Diverse Pokemon)**: 多种不同属性的精灵可供对战
- **可扩展架构 (Extensible Architecture)**: 模块化设计，易于添加新功能
- **无 UI 设计 (No UI)**: 专注于核心逻辑，便于后期集成任何 UI 框架

## Architecture

### C++ Core Components

- **Pokemon**: 精灵类，包含属性（HP, Attack, Defense, Special Defense, Speed, Type）、状态效果和技能
- **Move**: 技能类，支持从 Python 脚本加载效果函数，包含物理/特殊/状态分类
- **Battle**: 战斗类，管理回合制战斗流程，处理速度优先级和状态效果
- **TypeEffectiveness**: 属性克制系统，计算属性相性倍率
- **PythonSkillLoader**: Python 集成层，加载和执行 Python 技能脚本

### Python Skill Scripts

技能脚本位于 `scripts/` 目录，每个技能定义一个 `calculate_damage` 函数：

```python
def calculate_damage(attacker, defender):
    """
    Args:
        attacker: dict with keys: name, current_hp, max_hp, attack, defense, speed
        defender: dict with keys: name, current_hp, max_hp, attack, defense, speed
    Returns:
        int: damage amount (positive for damage, negative for healing, 0 for status-only moves)
    """
    # Your skill logic here
    damage = calculate_your_damage(attacker, defender)
    return damage
```

## Building

### Requirements

- **C++14** compatible compiler (g++ 7+, clang 5+, MSVC 2017+)
- **Python 3.6+** with development headers
- **CMake 3.10+**

### Platform-Specific Setup

#### Ubuntu/Debian Linux

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential cmake python3-dev

# Clone and build
git clone https://github.com/polymer1215/Pokemon-game.git
cd Pokemon-game
mkdir build && cd build
cmake ..
cmake --build .

# Run
./pokemon_battle
```

#### macOS

```bash
# Install dependencies (using Homebrew)
brew install cmake python3

# Clone and build
git clone https://github.com/polymer1215/Pokemon-game.git
cd Pokemon-game
mkdir build && cd build
cmake ..
cmake --build .

# Run
./pokemon_battle
```

#### Windows (MSVC)

```bash
# Prerequisites: Install Visual Studio 2017+ and Python 3.6+
# Make sure Python development libraries are included

# Clone repository
git clone https://github.com/polymer1215/Pokemon-game.git
cd Pokemon-game

# Create build directory
mkdir build
cd build

# Configure (adjust Python path if needed)
cmake .. -G "Visual Studio 16 2019"

# Build
cmake --build . --config Release

# Run
Release\pokemon_battle.exe
```

#### Windows (MinGW)

```bash
# Prerequisites: Install MinGW-w64 and Python 3.6+

# Clone and build
git clone https://github.com/polymer1215/Pokemon-game.git
cd Pokemon-game
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .

# Run
pokemon_battle.exe
```

### Quick Build Instructions

For a quick build on Unix-like systems:

```bash
mkdir build && cd build
cmake .. && cmake --build .
./pokemon_battle
```

## Usage Example

```cpp
// Create Pokemon with types
auto pikachu = std::make_shared<Pokemon>("Pikachu", "Electric", 100, 55, 40, 50, 90);
auto squirtle = std::make_shared<Pokemon>("Squirtle", "Water", 120, 48, 65, 64, 43);

// Create moves with Python scripts
auto thunderbolt = std::make_shared<Move>("Thunderbolt", "thunderbolt.py", 90, 100, "Electric", MoveCategory::SPECIAL);
auto waterGun = std::make_shared<Move>("Water Gun", "water_gun.py", 40, 100, "Water", MoveCategory::SPECIAL);

// Create status moves
auto thunderWave = std::make_shared<Move>("Thunder Wave", "thunder_wave.py", 0, 100, "Electric", MoveCategory::STATUS, "Paralyzed", 4);

// Load Python skill
auto skillFunc = PythonSkillLoader::loadSkill("thunderbolt", "calculate_damage");
thunderbolt->setEffectFunction(skillFunc);

// Add moves to Pokemon
pikachu->addMove(thunderbolt);
pikachu->addMove(thunderWave);
squirtle->addMove(waterGun);

// Start battle
Battle battle(pikachu, squirtle);
auto winner = battle.start();
```

## Type Effectiveness

The game implements a comprehensive type effectiveness system:

- **Super Effective (2x damage)**: Water → Fire, Fire → Grass, Grass → Water/Ground/Rock, Electric → Water/Flying, etc.
- **Not Very Effective (0.5x damage)**: Fire → Water, Water → Grass, Grass → Fire/Flying, etc.
- **No Effect (0x damage)**: Electric → Ground, Ground → Flying

## Status Effects

- **Poisoned**: Takes 1/8 max HP damage each turn
- **Burned**: Takes 1/16 max HP damage each turn
- **Paralyzed**: 50% chance to be unable to move each turn

## Move Categories

- **Physical**: Uses Attack vs Defense
- **Special**: Uses Attack vs Special Defense
- **Status**: Applies status effects or other non-damaging effects

## Damage Calculation

### Base Damage Formula

The game uses a simplified Pokemon damage formula:

```
Damage = ((2 × Level ÷ 5 + 2) × Power × Attack ÷ Defense) ÷ 50 + 2
```

Where:
- **Level**: Assumed to be 50 for all Pokemon
- **Power**: Base power of the move (e.g., 90 for Thunderbolt)
- **Attack**: Attacker's Attack stat (or Special Attack for special moves)
- **Defense**: Defender's Defense stat (or Special Defense for special moves)

### Modifiers

After base damage calculation, several modifiers are applied:

1. **Random Factor**: Multiplies damage by 85-100% for variation
2. **Type Effectiveness**: Multiplies by 2.0 (super effective), 1.0 (normal), 0.5 (not very effective), or 0.0 (no effect)
3. **Critical Hits** (in some moves): Multiplies damage by 1.5-2.0

### Example Calculation

Pikachu uses Thunderbolt (Power: 90, Special) on Squirtle:
- Pikachu's Attack: 55
- Squirtle's Special Defense: 64
- Type: Electric vs Water = Super Effective (2.0×)

```
Base = ((2 × 50 ÷ 5 + 2) × 90 × 55 ÷ 64) ÷ 50 + 2 ≈ 30
Random = 30 × 0.92 (example) ≈ 27
Type = 27 × 2.0 = 54 damage
```

### Custom Damage Functions

Python skill scripts can implement custom damage calculations. See [Python Skills Guide](docs/PYTHON_SKILLS.md) for details.

## Extending the System

### Adding New Pokemon

```cpp
auto newPokemon = std::make_shared<Pokemon>(
    "Name",      // Name
    "Type",      // Type (Electric, Water, Fire, Grass, etc.)
    100,         // Max HP
    50,          // Attack
    45,          // Defense
    50,          // Special Defense
    70           // Speed
);
```

### Adding New Skills

1. Create a Python script in `scripts/` directory:

```python
# scripts/new_skill.py
def calculate_damage(attacker, defender):
    # Your custom logic
    base_power = 80
    damage = (attacker['attack'] * base_power) / defender['defense']
    return int(damage)
```

2. Create the move in C++ and load the script:

```cpp
auto newSkill = std::make_shared<Move>("NewSkill", "new_skill.py", 80, 100, "Type", MoveCategory::SPECIAL);
auto skillFunc = PythonSkillLoader::loadSkill("new_skill", "calculate_damage");
newSkill->setEffectFunction(skillFunc);
```

### Adding Status Moves

```cpp
auto statusMove = std::make_shared<Move>(
    "Toxic",              // Move name
    "toxic.py",           // Python script
    0,                    // Base power (0 for status moves)
    90,                   // Accuracy
    "Poison",             // Type
    MoveCategory::STATUS, // Category
    "Poisoned",          // Status effect to apply
    5                     // Duration in turns
);
```

## Future Extensions

- ✅ 状态效果 (Status effects): 中毒、麻痹、灼烧等 - **Implemented**
- ✅ 属性克制 (Type effectiveness): 属性相克系统 - **Implemented**
- ✅ 技能分类 (Move categories): 物理/特殊/状态技能 - **Implemented**
- 多只精灵战斗 (Multi-Pokemon battles): 组队战斗
- AI 对手 (AI opponents): 智能对战策略
- 经验值和等级系统 (Experience and leveling)
- 物品系统 (Item system)
- UI 集成 (UI integration): 图形界面
- 更多属性 (More types): Psychic, Dragon, Fighting, etc.

## Available Pokemon

The game currently includes:
- **Pikachu** (Electric): Fast special attacker with Thunder Wave support
- **Squirtle** (Water): Tanky water specialist with good defenses
- **Bulbasaur** (Grass): Balanced attacker with Toxic support
- **Charmander** (Fire): Strong fire attacker with decent speed

## Troubleshooting

### Build Issues

#### Python.h not found

**Error**: `fatal error: Python.h: No such file or directory`

**Solution**:
- **Ubuntu/Debian**: `sudo apt-get install python3-dev`
- **macOS**: `brew install python3`
- **Windows**: Reinstall Python and ensure "Development headers" are checked

#### CMake cannot find Python

**Error**: `Could NOT find Python3 (missing: Python3_LIBRARIES Python3_INCLUDE_DIRS)`

**Solution**:
- Ensure Python 3 is installed and in your PATH
- Try specifying Python path explicitly:
  ```bash
  cmake .. -DPython3_ROOT_DIR=/path/to/python3
  ```
- On Windows, ensure Python launcher is enabled

#### Linker errors with Python libraries

**Error**: `undefined reference to 'Py_Initialize'`

**Solution**:
- Ensure Python development libraries are installed
- Check CMake output for Python library detection
- Try cleaning build directory and reconfiguring: `rm -rf build && mkdir build && cd build && cmake ..`

### Runtime Issues

#### Python scripts not loading

**Problem**: Moves use default damage calculation instead of Python scripts

**Solution**:
- Ensure scripts are in the `scripts/` directory
- Check script filenames match exactly (case-sensitive)
- Verify Python syntax with: `python3 -m py_compile scripts/your_script.py`
- Check console output for Python errors

#### Segmentation fault on startup

**Problem**: Program crashes immediately

**Solution**:
- Ensure Python interpreter is properly initialized
- Check that `PythonSkillLoader::initialize()` is called before any Python operations
- Verify Python version compatibility (3.6+)
- Try rebuilding: `cd build && cmake --build . --clean-first`

#### Unexpected damage values

**Problem**: Damage seems incorrect

**Solution**:
- Review Python skill script calculations
- Check that correct stats are used (defense vs special_defense)
- Verify type effectiveness is calculated correctly
- Add debug print statements in Python scripts

### Common Questions

**Q: Can I use Pokemon with more than 4 moves?**  
A: Yes, the code supports any number of moves. The current implementation randomly selects from available moves.

**Q: How do I change the battle matchup?**  
A: Modify `main.cpp` to change which Pokemon battle each other. You can also add logic for user selection.

**Q: Can I add more types?**  
A: Yes! Add new type effectiveness rules in `TypeEffectiveness.cpp` and create Pokemon with the new type.

**Q: How do I make Pokemon use specific moves instead of random ones?**  
A: Modify the Battle class to implement move selection logic instead of random selection.

**Q: Can I save and load Pokemon/Battle state?**  
A: Not currently implemented. You'd need to add serialization logic to save Pokemon stats and battle state.

## Documentation

### Detailed Guides

- **[API Documentation](docs/API.md)** - Complete API reference for all classes
- **[Python Skills Guide](docs/PYTHON_SKILLS.md)** - How to create custom moves with Python
- **[Contributing Guide](CONTRIBUTING.md)** - Development guidelines and how to contribute

### Quick Links

- [Adding New Pokemon](#adding-new-pokemon)
- [Adding New Skills](#adding-new-skills)
- [Type Effectiveness Chart](#type-effectiveness)
- [Damage Calculation Formula](#damage-calculation)

### Examples

Check the `scripts/` directory for Python skill examples:
- `thunderbolt.py` - Basic special attack
- `flamethrower.py` - Attack with critical hit chance
- `water_gun.py` - Simple water attack
- `toxic.py` - Status effect move
- `heal.py` - HP restoration move
- `slash.py` - High critical hit ratio move
- `eruption.py` - HP-based variable power
- `electro_ball.py` - Speed-based damage

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for:
- Development environment setup
- Coding standards
- How to add new features
- Pull request process

### Areas for Contribution

- 🎮 New Pokemon with unique stat distributions
- ⚔️ New moves with creative effects
- 🏗️ New game mechanics (abilities, items, weather)
- 📚 Documentation improvements
- 🐛 Bug fixes and optimizations
- 🧪 Test coverage
- 🎨 UI/Graphics integration

## License

MIT License