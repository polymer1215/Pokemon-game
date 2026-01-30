"""
Slash - Normal type physical attack
High critical hit ratio move
"""

def calculate_damage(attacker, defender):
    """
    Physical attack with increased critical hit chance
    
    Args:
        attacker: Dictionary with attacker's stats
        defender: Dictionary with defender's stats
        
    Returns:
        int: Damage amount to deal
    """
    import random
    
    # Base power for Slash
    base_power = 70
    
    # Basic damage calculation
    level = 50
    attack_stat = attacker['attack']
    defense_stat = defender['defense']
    
    damage = ((2 * level / 5 + 2) * base_power * attack_stat / defense_stat) / 50 + 2
    damage = int(damage)
    
    # High critical hit ratio (30% instead of normal 6.25%)
    critical_chance = random.randint(1, 100)
    if critical_chance <= 30:
        damage = int(damage * 1.5)
        print(f"[Python] Critical hit!")
    
    # Random factor
    random_factor = random.randint(85, 100) / 100.0
    damage = int(damage * random_factor)
    
    # Minimum damage
    damage = max(1, damage)
    
    print(f"[Python] Slash: {attacker['name']} attacks {defender['name']}")
    print(f"[Python] Calculated damage: {damage}")
    
    return damage
