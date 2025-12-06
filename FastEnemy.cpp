#include "FastEnemy.hpp"

// Constructor: Creates a fast enemy at specified position
// pos: Starting position of the enemy
// Calls base Enemy constructor with:
// - pos: position
// - 160.f: higher speed (160 pixels/second, vs basic enemy's 90)
// - 2: higher health (2 hits to kill, vs basic enemy's 1)
// - "assets/fast_enemy.png": different texture file for visual distinction
FastEnemy::FastEnemy(const sf::Vector2f& pos) 
    : Enemy(pos, 160.f, 2, "assets/fast_enemy.png") {
}

// Update function for fast enemy
// dt: Delta time for frame-independent updates
void FastEnemy::update(float dt) {
    // Simply update sprite position to match entity position
    sprite.setPosition(position);
}

// Returns the specific entity type for this enemy
Entity::Type FastEnemy::getType() const { 
    return Entity::Type::FAST_ENEMY; // Distinguishes from BASIC_ENEMY type
}
