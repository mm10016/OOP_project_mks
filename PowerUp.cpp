#include "PowerUp.hpp"
#include <cmath>
#include <iostream>

// PowerUp constructor - Creates a health power-up at specified position
// pos: Initial position where the power-up will spawn
PowerUp::PowerUp(const sf::Vector2f& pos) {
    position = pos;
    powerType = 0; // Always health power-up (0 represents health type)
    
    // Try to load power-up texture from file
    if (!texture.loadFromFile("assets/PowerUp_02.png")) {
        std::cout << "Failed to load PowerUp_02.png\n";
        
        // Create a fallback texture if file loading fails
        sf::Image img;
        img.create(32, 32, sf::Color::Transparent);
        
        // Create a simple green circle as fallback graphic
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 32; ++x) {
                // Calculate distance from center
                float dist = std::sqrt((x-16)*(x-16) + (y-16)*(y-16));
                // Draw green pixels within radius 14
                if (dist <= 14) {
                    img.setPixel(x, y, sf::Color::Green);
                }
            }
        }
        texture.loadFromImage(img);
    }
    
    // Set up the sprite with the loaded texture
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(position);
    sprite.setScale(0.6f, 0.6f); // Scale down slightly
    
    // Set initial movement properties
    velocity = sf::Vector2f(0, 50.f); // Moves downward
    lifetime = 8.0f; // Power-up lasts for 8 seconds
    lifeTimer = 0.f; // Timer starts at 0
    health = 1; // Power-up has 1 health (can be collected once)
}

// Updates the power-up's position and state
// dt: Delta time since last frame
void PowerUp::update(float dt) {
    // Move power-up based on velocity
    position += velocity * dt;
    sprite.setPosition(position);
    
    // Update lifetime timer
    lifeTimer += dt;
    
    // Add gentle bobbing motion for visual appeal
    sprite.move(0, std::sin(lifeTimer * 3.0f) * 0.5f);
    
    // Mark as dead if lifetime exceeded
    if (lifeTimer >= lifetime) alive = false;
}

// Draws the power-up to the screen
// window: Reference to the render window
void PowerUp::draw(sf::RenderWindow& window) {
    // Draw the main sprite
    window.draw(sprite);
    
    // Draw a green glow effect behind the power-up
    sf::CircleShape glow(25.f);
    glow.setFillColor(sf::Color(0, 255, 0, 30)); // Semi-transparent green
    glow.setOrigin(25.f, 25.f); // Center the glow
    glow.setPosition(position);
    window.draw(glow);
}

// Returns the bounding rectangle for collision detection
// Returns: FloatRect representing the power-up's collision bounds
sf::FloatRect PowerUp::getBounds() const {
    return sprite.getGlobalBounds();
}

// Returns the entity type (inherited from Entity class)
// Returns: Type identifier (FAST_ENEMY in this case, might be a placeholder)
Entity::Type PowerUp::getType() const { 
    return Entity::Type::FAST_ENEMY; 
}

// Returns the type of power-up (0 for health)
// Returns: Integer representing power-up type
int PowerUp::getPowerType() const { 
    return powerType; 
}
