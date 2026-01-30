"""
Water Gun skill - Water type attack
Basic water attack
"""

def calculate_damage(attacker, defender):
    """
    Calculate damage for Water Gun attack
    
    Args:
        attacker: Dictionary with attacker's stats
        defender: Dictionary with defender's stats
        
    Returns:
        int: Damage amount to deal
    """
    # Base power for Water Gun
    base_power = 40
    
    # Calculate damage using Pokemon formula
    level = 50  # Assume level 50
    attack_stat = attacker['attack']
    defense_stat = defender['defense']
    
    damage = ((2 * level / 5 + 2) * base_power * attack_stat / defense_stat) / 50 + 2
    damage = int(damage)
    
    # Random factor (85% to 100%)
    import random
    random_factor = random.randint(85, 100) / 100.0
    damage = int(damage * random_factor)
    
    # Minimum damage
    damage = max(1, damage)
    
    print(f"[Python] Water Gun: {attacker['name']} attacks {defender['name']}")
    print(f"[Python] Calculated damage: {damage}")
    
    return damage
