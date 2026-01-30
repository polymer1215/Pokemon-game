"""
Thunder Wave skill - Electric type status move
Paralyzes the target
"""

def calculate_damage(attacker, defender):
    """
    Thunder Wave move - inflicts paralysis on the defender
    
    Args:
        attacker: Dictionary with attacker's stats
        defender: Dictionary with defender's stats
        
    Returns:
        int: 0 damage (status effect only)
    """
    print(f"[Python] {defender['name']} was paralyzed!")
    print(f"[Python] {defender['name']}'s Speed is reduced!")
    
    return 0
