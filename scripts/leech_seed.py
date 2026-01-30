"""
Leech Seed - Grass type status move
Drains HP each turn (implemented in C++)
"""

def calculate_damage(attacker, defender):
    """
    Status move that drains HP over time
    
    Args:
        attacker: Dictionary with attacker's stats
        defender: Dictionary with defender's stats
        
    Returns:
        int: 0 (status effect only)
    """
    print(f"[Python] {attacker['name']} planted a seed on {defender['name']}!")
    print(f"[Python] {defender['name']} was seeded!")
    
    # Return 0 damage (leech effect would be applied by C++ each turn)
    return 0
