#include "Player.hpp"
#include "Bullet.hpp"
#include <cmath>
#include <iostream> 

// Helper function: Calculate the length (magnitude) of a 2D vector
static float length(const sf::Vector2f &v) {
    return std::sqrt(v.x*v.x + v.y*v.y);
}

// Helper function: Normalize a 2D vector to unit length
static sf::Vector2f normalize(const sf::Vector2f &v) {
    float len = length(v);
    if (len == 0.f) return {0.f,0.f}; // Handle zero vector case
    return v / len;
}

// Constructor: Creates a player at the specified starting position
Player::Player(const sf::Vector2f& startPos) {
    position = startPos; // Set initial position
    
    // Create a procedurally generated sprite for the player (fallback if no texture file)
    // This creates a simple spaceship shape for debugging/visualization
    sf::Image img;
    img.create(64, 64, sf::Color::Transparent); // Create transparent 64x64 canvas
    
    // Draw a simple spaceship shape pixel by pixel
    for (int y = 0; y < 64; ++y) {
        for (int x = 0; x < 64; ++x) {
            // Ship body (triangle shape that widens toward the bottom)
            if (y > 10 && y < 54) {
                int halfWidth = (y - 10) / 3; // Calculate width at this y-position
                if (x >= (32 - halfWidth) && x <= (32 + halfWidth)) {
                    img.setPixel(x, y, sf::Color::Cyan); // Body color
                }
            }
            
            // Cockpit (small rectangle in the center-top of ship)
            if (y > 15 && y < 30 && x > 28 && x < 36) {
                img.setPixel(x, y, sf::Color::Blue);
            }
            
            // Engine glow (at bottom of ship with gradient effect)
            if (y > 50 && y < 60 && x > 26 && x < 38) {
                int dist = std::abs(x - 32); // Distance from center
                if (dist < 4) {
                    img.setPixel(x, y, sf::Color::Yellow); // Bright center
                } else if (dist < 6) {
                    img.setPixel(x, y, sf::Color::Red); // Outer glow
                }
            }
        }
    }
    
    // Load the generated image as a texture
    texture.loadFromImage(img);
    sprite.setTexture(texture);
    // Center the sprite origin for proper rotation and positioning
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(position);
    sprite.setScale(1.0f, 1.0f); // Full scale (no reduction)
    
    // Debug output to verify player creation
    std::cout << "Player sprite created at position: " << position.x << ", " << position.y << std::endl;
    std::cout << "Sprite origin: " << sprite.getOrigin().x << ", " << sprite.getOrigin().y << std::endl;
    
    // Initialize player attributes
    speed = 250.f;  // Movement speed in pixels per second
    health = 5;     // Starting health
}

// Update player state each frame
void Player::update(float dt) {
    weapon.update(dt);         // Update weapon cooldown timer
    sprite.setPosition(position); // Update sprite to match position
}

// Draw the player to the window
void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Get the bounding rectangle for collision detection
sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

// Shoot a bullet toward a target position
std::unique_ptr<Bullet> Player::shootTowards(const sf::Vector2f& target) {
    // Calculate direction from player to target
    sf::Vector2f dir = target - position;
    // Use weapon to fire a bullet in that direction
    auto b = weapon.fire(position, dir);
    return b;
}

// Set the player's rotation (sprite faces upward by default, so add 90°)
void Player::setRotation(float angle) {
    sprite.setRotation(angle + 90); // Add 90 so 0° faces upward
}

// Overloaded += operator for easy movement/knockback
Player& Player::operator+=(const sf::Vector2f& moveVec) {
    position += moveVec; // Add vector to position
    return *this;
}

// Constrain player position within specified bounds (screen edges)
void Player::setPositionBounds(const sf::FloatRect& bounds) {
    if (position.x < bounds.left) position.x = bounds.left;
    if (position.y < bounds.top) position.y = bounds.top;
    if (position.x > bounds.left + bounds.width) position.x = bounds.left + bounds.width;
    if (position.y > bounds.top + bounds.height) position.y = bounds.top + bounds.height;
}

// Heal the player by specified amount
void Player::heal(int v) { 
    health += v; 
}

// Return the entity type (used in collision detection)
Entity::Type Player::getType() const { 
    return Entity::Type::PLAYER; 
}
