"""
Electro Ball - Electric type special attack
Power based on speed difference between attacker and defender
"""

def calculate_damage(attacker, defender):
    """
    Special attack with power based on speed ratio
    
    Args:
        attacker: Dictionary with attacker's stats
        defender: Dictionary with defender's stats
        
    Returns:
        int: Damage amount to deal
    """
    import random
    
    # Calculate speed ratio
    defender_speed = max(defender['speed'], 1)  # Prevent division by zero
    speed_ratio = attacker['speed'] / defender_speed
    
    # Determine base power based on speed ratio
    if speed_ratio >= 4:
        base_power = 150
        power_desc = "maximum"
    elif speed_ratio >= 3:
        base_power = 120
        power_desc = "very high"
    elif speed_ratio >= 2:
        base_power = 80
        power_desc = "high"
    elif speed_ratio >= 1:
        base_power = 60
        power_desc = "moderate"
    else:
        base_power = 40
        power_desc = "low"
    
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
    
    print(f"[Python] Electro Ball: {attacker['name']} hurls an electric orb!")
    print(f"[Python] Speed ratio: {speed_ratio:.2f}x ({power_desc} power - {base_power})")
    print(f"[Python] Calculated damage: {damage}")
    
    return damage
