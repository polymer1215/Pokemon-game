"""
Toxic skill - Poison type status move
Badly poisons the target
"""

def calculate_damage(attacker, defender):
    """
    Toxic move - inflicts poison status on the defender
    Note: This is a Poison-type move
    
    Args:
        attacker: Dictionary with attacker's stats
        defender: Dictionary with defender's stats
        
    Returns:
        int: 0 damage (status effect only, actual status applied in C++)
    """
    print(f"[Python] {defender['name']} was badly poisoned!")
    
    # Return 0 damage (status effect is applied by C++ code)
    return 0
