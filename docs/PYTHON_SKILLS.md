# Python Skill Development Guide

This guide explains how to create custom Pokemon moves using Python scripts.

## Table of Contents

- [Overview](#overview)
- [Basic Structure](#basic-structure)
- [Skill Categories](#skill-categories)
- [Advanced Examples](#advanced-examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

## Overview

The Pokemon Battle Game allows you to define custom move effects using Python scripts. This provides flexibility to create complex damage calculations, special effects, and unique mechanics without modifying the C++ core.

### How It Works

1. **Python Script**: You write a Python function that calculates damage based on attacker and defender stats
2. **C++ Integration**: The `PythonSkillLoader` class loads your script and integrates it into the move
3. **Execution**: When the move is used in battle, your Python function is called to calculate the effect

### Advantages

- **Easy to modify**: Change move effects without recompiling
- **Flexible**: Implement complex formulas and conditions
- **Rapid prototyping**: Test new move ideas quickly
- **Extensible**: Add new mechanics without touching core code

## Basic Structure

Every Python skill script must define a `calculate_damage` function with this signature:

```python
def calculate_damage(attacker, defender):
    """
    Calculate damage/effect for this move
    
    Args:
        attacker: Dictionary with attacker's stats
            Keys: name, current_hp, max_hp, attack, defense, special_defense, speed
        defender: Dictionary with defender's stats
            Keys: name, current_hp, max_hp, attack, defense, special_defense, speed
    
    Returns:
        int: Damage amount
            - Positive value: Damage to deal to defender
            - Negative value: Healing to apply to attacker
            - Zero: Status effect only (no damage/healing)
    """
    # Your calculation logic here
    damage = 0
    return damage
```

### Available Data

Both `attacker` and `defender` dictionaries contain:

| Key | Type | Description |
|-----|------|-------------|
| `name` | str | Pokemon's name |
| `current_hp` | int | Current HP |
| `max_hp` | int | Maximum HP |
| `attack` | int | Attack stat |
| `defense` | int | Defense stat |
| `special_defense` | int | Special Defense stat |
| `speed` | int | Speed stat |

## Skill Categories

### 1. Damaging Moves

Moves that deal damage to the opponent.

#### Simple Physical Attack

```python
"""
Tackle - Normal type physical attack
Basic physical move with fixed base power
"""

def calculate_damage(attacker, defender):
    """Basic physical attack"""
    base_power = 40
    
    # Simple damage formula: (Attack * Power) / Defense
    damage = (attacker['attack'] * base_power) / defender['defense']
    
    return int(damage)
```

#### Special Attack with Pokemon Formula

```python
"""
Thunderbolt - Electric type special attack
Uses the classic Pokemon damage formula
"""

def calculate_damage(attacker, defender):
    """
    Calculate damage using Pokemon damage formula
    Formula: ((2 * Level / 5 + 2) * Power * Attack / Defense) / 50 + 2
    """
    import random
    
    base_power = 90
    level = 50  # Assume level 50
    
    # Pokemon damage calculation
    attack_stat = attacker['attack']
    defense_stat = defender['special_defense']  # Use special defense for special moves
    
    # Base damage
    damage = ((2 * level / 5 + 2) * base_power * attack_stat / defense_stat) / 50 + 2
    damage = int(damage)
    
    # Random factor (85% to 100%)
    random_factor = random.randint(85, 100) / 100.0
    damage = int(damage * random_factor)
    
    # Minimum damage
    damage = max(1, damage)
    
    print(f"[Python] {attacker['name']} used Thunderbolt on {defender['name']}")
    print(f"[Python] Calculated damage: {damage}")
    
    return damage
```

#### High Critical Hit Ratio Move

```python
"""
Slash - Normal type physical attack
High critical hit ratio move
"""

def calculate_damage(attacker, defender):
    """Physical attack with 30% critical hit chance"""
    import random
    
    base_power = 70
    
    # Basic damage
    damage = (attacker['attack'] * base_power) / defender['defense']
    
    # Critical hit chance (30%)
    if random.randint(1, 100) <= 30:
        damage *= 2
        print(f"[Python] Critical hit!")
    
    return int(damage)
```

### 2. Healing Moves

Moves that restore HP. Return negative values for healing.

#### Self-Healing

```python
"""
Recover - Restores 50% of max HP
"""

def calculate_damage(attacker, defender):
    """Healing move - returns negative damage"""
    # Heal 50% of max HP
    heal_amount = int(attacker['max_hp'] * 0.5)
    
    print(f"[Python] {attacker['name']} used Recover!")
    print(f"[Python] Restoring {heal_amount} HP")
    
    # Return negative to indicate healing
    return -heal_amount
```

#### Conditional Healing

```python
"""
Rest - Fully heals HP but only if below 50%
"""

def calculate_damage(attacker, defender):
    """Conditional healing move"""
    current_hp_percent = attacker['current_hp'] / attacker['max_hp']
    
    if current_hp_percent < 0.5:
        # Heal to full HP
        heal_amount = attacker['max_hp'] - attacker['current_hp']
        print(f"[Python] {attacker['name']} went to sleep and restored all HP!")
        return -heal_amount
    else:
        print(f"[Python] {attacker['name']} is not tired enough to rest!")
        return 0
```

### 3. Status Moves

Moves that apply status effects without dealing damage. Return 0 for damage.

#### Basic Status Move

```python
"""
Thunder Wave - Paralyzes the target
"""

def calculate_damage(attacker, defender):
    """Status move - inflicts paralysis"""
    print(f"[Python] {defender['name']} was paralyzed!")
    print(f"[Python] {defender['name']}'s Speed is reduced!")
    
    # Status effect is applied by C++ code
    # Return 0 damage
    return 0
```

#### Poison Move

```python
"""
Toxic - Badly poisons the target
"""

def calculate_damage(attacker, defender):
    """Poison status move"""
    print(f"[Python] {defender['name']} was badly poisoned!")
    
    # Return 0 damage (poison damage applied by C++ each turn)
    return 0
```

### 4. Variable Power Moves

Moves whose power depends on conditions.

#### HP-Based Power

```python
"""
Eruption - Power decreases as user's HP decreases
"""

def calculate_damage(attacker, defender):
    """Power scales with user's remaining HP"""
    # Base power scales from 150 (full HP) to 1 (low HP)
    hp_ratio = attacker['current_hp'] / attacker['max_hp']
    base_power = int(150 * hp_ratio)
    base_power = max(1, base_power)  # Minimum power of 1
    
    damage = (attacker['attack'] * base_power) / defender['defense']
    
    print(f"[Python] Eruption with {base_power} power!")
    
    return int(damage)
```

#### Speed-Based Damage

```python
"""
Electro Ball - Power based on speed difference
"""

def calculate_damage(attacker, defender):
    """Damage scales with speed difference"""
    speed_ratio = attacker['speed'] / max(defender['speed'], 1)
    
    # Determine base power based on speed ratio
    if speed_ratio >= 4:
        base_power = 150
    elif speed_ratio >= 3:
        base_power = 120
    elif speed_ratio >= 2:
        base_power = 80
    elif speed_ratio >= 1:
        base_power = 60
    else:
        base_power = 40
    
    damage = (attacker['attack'] * base_power) / defender['special_defense']
    
    print(f"[Python] Electro Ball with {base_power} power (speed ratio: {speed_ratio:.2f})")
    
    return int(damage)
```

### 5. Multi-Hit Moves

Moves that hit multiple times (simulated).

```python
"""
Double Kick - Hits twice
Note: This is a simplified version. True multi-hit would need C++ support.
"""

def calculate_damage(attacker, defender):
    """Simulated double-hit move"""
    import random
    
    base_power = 30
    
    # Calculate damage for one hit
    single_hit = (attacker['attack'] * base_power) / defender['defense']
    
    # Hit twice
    total_damage = single_hit * 2
    
    print(f"[Python] Hit 2 times!")
    
    return int(total_damage)
```

### 6. Recoil Moves

Moves that damage the user as well.

```python
"""
Double-Edge - High power but causes recoil
Note: Recoil would need to be applied in C++ code. This returns extra info via print.
"""

def calculate_damage(attacker, defender):
    """High-power move with recoil (informational)"""
    base_power = 120
    
    damage = (attacker['attack'] * base_power) / defender['defense']
    recoil = int(damage * 0.33)  # 33% recoil
    
    print(f"[Python] Double-Edge deals {int(damage)} damage!")
    print(f"[Python] {attacker['name']} would take {recoil} recoil damage (not implemented)")
    
    return int(damage)
```

## Advanced Examples

### Stat-Boosting Move (Informational)

```python
"""
Swords Dance - Sharply raises Attack (informational only)
Note: Stat changes would need C++ support. This is informational.
"""

def calculate_damage(attacker, defender):
    """Status move that would boost attack"""
    print(f"[Python] {attacker['name']}'s Attack rose sharply!")
    print(f"[Python] (Stat changes not yet implemented)")
    
    return 0
```

### Weather-Dependent Move

```python
"""
Thunder - Higher accuracy in rain, lower in sun
Note: Weather system not implemented. This is a conceptual example.
"""

def calculate_damage(attacker, defender):
    """Weather-dependent move (conceptual)"""
    import random
    
    base_power = 110
    weather = "normal"  # In real implementation, this would come from battle state
    
    # Accuracy varies with weather (would be checked in C++ execute())
    # This is just the damage calculation
    
    damage = (attacker['attack'] * base_power) / defender['special_defense']
    
    # Random factor
    random_factor = random.randint(85, 100) / 100.0
    damage = int(damage * random_factor)
    
    print(f"[Python] Thunder strike!")
    
    return int(damage)
```

### Combo Move

```python
"""
Venoshock - Double power if target is poisoned
Note: Would need to check status from defender dictionary
"""

def calculate_damage(attacker, defender):
    """More powerful against poisoned targets"""
    base_power = 65
    
    # In a full implementation, defender would have 'status' field
    # For now, just use regular power
    # if defender.get('status') == 'Poisoned':
    #     base_power = 130
    
    damage = (attacker['attack'] * base_power) / defender['special_defense']
    
    return int(damage)
```

## Best Practices

### 1. Always Return an Integer

```python
# Good
return int(damage)

# Bad - might return float
return damage
```

### 2. Handle Edge Cases

```python
# Good - prevent division by zero
defense_stat = max(defender['defense'], 1)
damage = (attacker['attack'] * base_power) / defense_stat

# Good - ensure minimum damage
damage = max(1, damage)

# Good - cap healing
heal_amount = min(heal_amount, attacker['max_hp'])
```

### 3. Add Informative Print Statements

```python
# Good - helps debugging and adds flavor
print(f"[Python] {attacker['name']} used Flamethrower!")
print(f"[Python] Calculated damage: {damage}")

# Avoid - too verbose
# print(f"Attack stat: {attacker['attack']}")
# print(f"Defense stat: {defender['defense']}")
# print(f"Base power: {base_power}")
```

### 4. Use Consistent Formatting

Follow PEP 8 style guidelines:
- 4 spaces for indentation
- snake_case for variable names
- Clear variable names
- Docstrings for functions

### 5. Test Your Moves

After creating a skill, test it in battle to ensure:
- Damage values are reasonable
- No crashes or exceptions
- Print statements work correctly
- Edge cases are handled

## Troubleshooting

### Common Issues

#### 1. Move Not Loading

**Problem**: Move uses default calculation instead of Python script

**Solutions**:
- Check that script file is in `scripts/` directory
- Verify filename in Move constructor matches script name
- Ensure `loadPythonSkill()` is called after creating the move
- Check for Python syntax errors

#### 2. Damage Too High/Low

**Problem**: Move deals unexpected damage

**Solutions**:
- Review your damage formula
- Check that you're using the right stats (attack/defense for physical, special_defense for special)
- Verify base power is reasonable (typically 40-150)
- Add print statements to debug calculations

#### 3. Import Errors

**Problem**: `ImportError` when using Python libraries

**Solutions**:
- Python interpreter is initialized before loading skills
- Only use standard library modules (random, math, etc.)
- Avoid external packages (numpy, etc.) unless they're installed

#### 4. Type Errors

**Problem**: Type mismatch errors

**Solutions**:
- Always return `int`, not `float`
- Use `int()` to convert calculations
- Access dictionary values with correct keys

### Debugging Tips

1. **Add Print Statements**:
   ```python
   print(f"[DEBUG] Attacker attack: {attacker['attack']}")
   print(f"[DEBUG] Defender defense: {defender['defense']}")
   print(f"[DEBUG] Damage before adjustments: {damage}")
   ```

2. **Test in Isolation**:
   Create a simple Python script to test your formula:
   ```python
   # test_move.py
   attacker = {'name': 'Pikachu', 'attack': 55, 'current_hp': 100, 'max_hp': 100, 'defense': 40, 'special_defense': 50, 'speed': 90}
   defender = {'name': 'Squirtle', 'attack': 48, 'current_hp': 120, 'max_hp': 120, 'defense': 65, 'special_defense': 64, 'speed': 43}
   
   from scripts.thunderbolt import calculate_damage
   damage = calculate_damage(attacker, defender)
   print(f"Damage: {damage}")
   ```

3. **Check Return Values**:
   - Positive for damage
   - Negative for healing
   - Zero for status only

## Template

Use this template for new moves:

```python
"""
[Move Name] - [Type] type [category] move
[Brief description of what this move does]
"""

def calculate_damage(attacker, defender):
    """
    [Detailed description of the move's effect]
    
    Args:
        attacker: Dictionary with attacker's stats
        defender: Dictionary with defender's stats
    
    Returns:
        int: Damage amount (positive/negative/zero)
    """
    import random  # If needed
    
    # Configuration
    base_power = 50  # Adjust as needed
    
    # Damage calculation
    # Choose appropriate formula for your move type
    
    # Physical move:
    # damage = (attacker['attack'] * base_power) / defender['defense']
    
    # Special move:
    # damage = (attacker['attack'] * base_power) / defender['special_defense']
    
    # Healing move:
    # heal_amount = int(attacker['max_hp'] * 0.5)
    # return -heal_amount
    
    # Status move:
    # return 0
    
    damage = 0  # Replace with your calculation
    
    # Add flavor text
    print(f"[Python] {attacker['name']} used [Move Name]!")
    
    # Return damage as integer
    return int(damage)
```

---

## Next Steps

1. **Create your own moves** using the examples above
2. **Test thoroughly** in actual battles
3. **Share your creations** with the community
4. **Experiment** with new mechanics and effects

For more information:
- [API Documentation](API.md) - Complete API reference
- [Contributing Guide](../CONTRIBUTING.md) - How to contribute your moves
- [README](../README.md) - Project overview

Happy move creating!
