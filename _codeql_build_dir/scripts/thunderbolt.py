"""
Thunderbolt skill - Electric type attack
This is an example of how to define Pokemon skills in Python
"""

def calculate_damage(attacker, defender):
    """
    Calculate damage for Thunderbolt attack
    
    Args:
        attacker: Dictionary with attacker's stats (name, current_hp, max_hp, attack, defense, speed)
        defender: Dictionary with defender's stats
        
    Returns:
        int: Damage amount to deal
    """
    # Base power for Thunderbolt
    base_power = 90
    
    # Calculate damage using Pokemon formula
    # Damage = ((2 * Level / 5 + 2) * Power * Attack / Defense) / 50 + 2
    # Simplified version for this demo
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
    
    print(f"[Python] Thunderbolt: {attacker['name']} attacks {defender['name']}")
    print(f"[Python] Calculated damage: {damage}")
    
    return damage
