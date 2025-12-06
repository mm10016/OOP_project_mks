#include "PowerUp.hpp"
#include <cmath>
#include <iostream>

PowerUp::PowerUp(const sf::Vector2f& pos) {
    position = pos;
    powerType = 0; // Always health power-up
    
    if (!texture.loadFromFile("assets/PowerUp_02.png")) {
        std::cout << "Failed to load PowerUp_02.png\n";
        
        // Create fallback
        sf::Image img;
        img.create(32, 32, sf::Color::Transparent);
        
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 32; ++x) {
                float dist = std::sqrt((x-16)*(x-16) + (y-16)*(y-16));
                if (dist <= 14) {
                    img.setPixel(x, y, sf::Color::Green);
                }
            }
        }
        texture.loadFromImage(img);
    }
    
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(position);
    sprite.setScale(0.6f, 0.6f);
    
    velocity = sf::Vector2f(0, 50.f);
    lifetime = 8.0f;
    lifeTimer = 0.f;
    health = 1;
}

void PowerUp::update(float dt) {
    position += velocity * dt;
    sprite.setPosition(position);
    lifeTimer += dt;
    
    // Gentle bobbing motion
    sprite.move(0, std::sin(lifeTimer * 3.0f) * 0.5f);
    
    if (lifeTimer >= lifetime) alive = false;
}

void PowerUp::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    
    // Green glow effect
    sf::CircleShape glow(25.f);
    glow.setFillColor(sf::Color(0, 255, 0, 30));
    glow.setOrigin(25.f, 25.f);
    glow.setPosition(position);
    window.draw(glow);
}

sf::FloatRect PowerUp::getBounds() const {
    return sprite.getGlobalBounds();
}

Entity::Type PowerUp::getType() const { 
    return Entity::Type::FAST_ENEMY; 
}

int PowerUp::getPowerType() const { 
    return powerType; 
}