#include "Bullet.hpp"
#include <cmath>
#include <iostream>

// Constructor: Creates a bullet with specified properties
Bullet::Bullet(const sf::Vector2f& pos, const sf::Vector2f& dir, 
               float speed, float lt, int damage) {
    // Initialize position and velocity
    position = pos;
    velocity = normalize(dir) * speed;  // Normalize direction and apply speed
    
    // Load bullet texture from file
    if (!texture.loadFromFile("assets/bullet.png")) {
        std::cout << "Failed to load assets/bullet.png\n";
        // Note: If texture fails to load, sprite will be blank but still functional
    }
    
    // Configure bullet sprite
    sprite.setTexture(texture);
    // Center origin for proper rotation and positioning
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(position);     // Set initial position
    sprite.setScale(0.5f, 0.5f);      // Scale down to 50% size
    
    // Calculate rotation angle from direction vector
    // atan2(y, x) gives angle in radians, convert to degrees for SFML
    float angle = std::atan2(dir.y, dir.x) * 180 / 3.14159f;
    sprite.setRotation(angle);        // Rotate bullet to face movement direction
    
    // Initialize lifetime and timer
    lifetime = lt;    // Total lifespan in seconds
    lifeTimer = 0.f;  // Time elapsed since creation
    
    // Initialize damage and health values
    dmg = damage;     // Damage this bullet deals on impact
    health = 1;       // Bullet can take 1 hit before being destroyed
    
    // Note: 'alive' is inherited from Entity and defaults to true
}

// Update bullet position and check lifetime
void Bullet::update(float dt) {
    // Move bullet based on velocity and time delta
    position += velocity * dt;
    sprite.setPosition(position);  // Update sprite position
    
    // Update lifetime timer
    lifeTimer += dt;
    
    // Check if bullet has exceeded its lifespan
    if (lifeTimer >= lifetime) {
        alive = false;  // Mark bullet for removal
    }
}

// Draw bullet to the window
void Bullet::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Get the bounding rectangle of the bullet (for collision detection)
sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}

// Return the entity type (used in collision handling)
Entity::Type Bullet::getType() const { 
    return Entity::Type::BULLET; 
}

// Get damage value of bullet
int Bullet::getDamage() const { 
    return dmg; 
}
