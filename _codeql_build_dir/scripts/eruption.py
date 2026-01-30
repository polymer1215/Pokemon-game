"""
Eruption - Fire type special attack
Power decreases as user's HP decreases
"""

def calculate_damage(attacker, defender):
    """
    Special attack whose power scales with remaining HP
    
    Args:
        attacker: Dictionary with attacker's stats
        defender: Dictionary with defender's stats
        
    Returns:
        int: Damage amount to deal
    """
    import random
    
    # Calculate base power based on remaining HP ratio
    # Maximum power of 150 at full HP, minimum of 1
    hp_ratio = attacker['current_hp'] / attacker['max_hp']
    base_power = int(150 * hp_ratio)
    base_power = max(1, base_power)
    
    # Damage calculation
    level = 50
    attack_stat = attacker['attack']
    defense_stat = defender['special_defense']
    
    damage = ((2 * level / 5 + 2) * base_power * attack_stat / defense_stat) / 50 + 2
    damage = int(damage)
    
    # Random factor
    random_factor = random.randint(85, 100) / 100.0
    damage = int(damage * random_factor)
    
    # Minimum damage
    damage = max(1, damage)
    
    hp_percent = int(hp_ratio * 100)
    print(f"[Python] Eruption: {attacker['name']} unleashes fire with {hp_percent}% HP!")
    print(f"[Python] Base power: {base_power}")
    print(f"[Python] Calculated damage: {damage}")
    
    return damage
