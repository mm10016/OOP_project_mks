#include "Weapon.hpp"
#include "Bullet.hpp"

// Constructor: Initializes weapon with fire rate and damage
Weapon::Weapon(float fr, int dmg) 
    : fireRate(fr),     // Shots per second (higher = faster firing)
      cooldown(0.f),    // Time until next shot is available (0 = ready)
      damage(dmg) {}    // Damage per bullet

// Update the weapon's cooldown timer
void Weapon::update(float dt) {
    // Decrease cooldown by time passed, but don't go below 0
    if (cooldown > 0.f) cooldown -= dt;
}

// Check if weapon is ready to fire
bool Weapon::canFire() const {
    // Weapon can fire when cooldown reaches or passes 0
    return cooldown <= 0.f;
}

// Attempt to fire the weapon, creating a bullet if ready
std::unique_ptr<Bullet> Weapon::fire(const sf::Vector2f& from, const sf::Vector2f& dir) {
    // Check if weapon is ready to fire
    if (!canFire()) return nullptr; // Return null if still on cooldown
    
    // Reset cooldown to time between shots (1/fireRate = seconds per shot)
    cooldown = 1.f / fireRate;
    
    // Create and return a new bullet with specified properties:
    // - Position: from (where the shot originates)
    // - Direction: dir (normalized direction vector)
    // - Speed: 700.f (pixels per second)
    // - Lifetime: 2.0f (seconds before bullet disappears)
    // - Damage: damage (damage value from weapon)
    return std::make_unique<Bullet>(from, dir, 700.f, 2.0f, damage);
}

// Setter for fire rate (shots per second)
void Weapon::setFireRate(float f) { 
    fireRate = f; 
}

// Setter for damage per bullet
void Weapon::setDamage(int d) { 
    damage = d; 
}

// Getter for fire rate
float Weapon::getFireRate() const { 
    return fireRate; 
}

// Getter for damage
int Weapon::getDamage() const { 
    return damage; 
}
