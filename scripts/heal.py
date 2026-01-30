"""
Healing move that restores HP
"""

def calculate_damage(attacker, defender):
    """
    Healing move - returns negative damage to indicate healing amount
    
    Args:
        attacker: Dictionary with attacker's stats
        defender: Dictionary with defender's stats
        
    Returns:
        int: Negative value indicates healing amount
    """
    # Heal 50% of max HP
    heal_amount = int(attacker['max_hp'] * 0.5)
    
    print(f"[Python] {attacker['name']} used Recover!")
    print(f"[Python] Restoring {heal_amount} HP")
    
    # Return negative to indicate healing
    return -heal_amount

