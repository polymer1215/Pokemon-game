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

- C++14 compatible compiler (g++ 7+, clang 5+, MSVC 2017+)
- Python 3.x development headers
- CMake 3.10+

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# Run
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

## License

MIT License