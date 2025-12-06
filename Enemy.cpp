#include "Enemy.hpp"
#include <cmath>
#include <iostream>

// Constructor: Creates an enemy with specified properties
// pos: Starting position of the enemy
// spd: Movement speed in pixels per second
// hp: Initial health points
// texturePath: Path to the enemy's texture file
Enemy::Enemy(const sf::Vector2f& pos, float spd, int hp, const std::string& texturePath) {
    position = pos;  // Set initial position
    speed = spd;     // Set movement speed
    health = hp;     // Set health points
    // Note: 'alive' is inherited from Entity and defaults to true
    
    // Load enemy texture from file
    if (!texture.loadFromFile(texturePath)) {
        std::cout << "Failed to load " << texturePath << "\n";
        // If texture fails to load, sprite will be blank but still functional
    }
    
    // Configure enemy sprite
    sprite.setTexture(texture);
    // Center origin for proper rotation and positioning
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(position);     // Set initial sprite position
    sprite.setScale(0.5f, 0.5f);      // Scale down to 50% of original size
}

// Update enemy position to move toward a target (e.g., player)
// target: The position to move toward
// dt: Delta time for frame-independent movement
void Enemy::updateTowards(const sf::Vector2f& target, float dt) {
    // Calculate direction vector from enemy to target and normalize it
    sf::Vector2f dir = normalize(target - position);
    
    // Move enemy in the calculated direction
    position += dir * speed * dt;
    sprite.setPosition(position);  // Update sprite position
    
    // Calculate rotation angle for sprite to face movement direction
    // atan2(y, x) gives angle in radians, convert to degrees for SFML
    float angle = std::atan2(dir.y, dir.x) * 180 / 3.14159f;
    // Add 90 degrees so sprite faces forward (assuming sprite art faces upward by default)
    sprite.setRotation(angle + 90);
}

// Basic update function (used when not moving toward a specific target)
// dt: Delta time (not used in this simple implementation but kept for interface consistency)
void Enemy::update(float dt) {
    sprite.setPosition(position);  // Just update sprite position
}

// Draw enemy to the window
void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Get the bounding rectangle of the enemy (for collision detection)
sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

// Return the entity type (used in collision handling and enemy type tracking)
Entity::Type Enemy::getType() const { 
    return Entity::Type::BASIC_ENEMY; 
}
