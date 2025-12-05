#include "Player.hpp"
#include "Bullet.hpp"
#include <cmath>
#include <iostream> 

static float length(const sf::Vector2f &v) {
    return std::sqrt(v.x*v.x + v.y*v.y);
}

static sf::Vector2f normalize(const sf::Vector2f &v) {
    float len = length(v);
    if (len == 0.f) return {0.f,0.f};
    return v / len;
}

Player::Player(const sf::Vector2f& startPos) {
    position = startPos;
    
    // Create a simple visible player sprite for debugging
    sf::Image img;
    img.create(64, 64, sf::Color::Transparent);
    
    // Draw a simple spaceship shape
    for (int y = 0; y < 64; ++y) {
        for (int x = 0; x < 64; ++x) {
            // Ship body (triangle)
            if (y > 10 && y < 54) {
                int halfWidth = (y - 10) / 3;
                if (x >= (32 - halfWidth) && x <= (32 + halfWidth)) {
                    img.setPixel(x, y, sf::Color::Cyan);
                }
            }
            
            // Cockpit
            if (y > 15 && y < 30 && x > 28 && x < 36) {
                img.setPixel(x, y, sf::Color::Blue);
            }
            
            // Engine glow
            if (y > 50 && y < 60 && x > 26 && x < 38) {
                int dist = std::abs(x - 32);
                if (dist < 4) {
                    img.setPixel(x, y, sf::Color::Yellow);
                } else if (dist < 6) {
                    img.setPixel(x, y, sf::Color::Red);
                }
            }
        }
    }
    
    texture.loadFromImage(img);
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(position);
    sprite.setScale(1.0f, 1.0f); // Full scale for visibility
    
    std::cout << "Player sprite created at position: " << position.x << ", " << position.y << std::endl;
    std::cout << "Sprite origin: " << sprite.getOrigin().x << ", " << sprite.getOrigin().y << std::endl;
    
    speed = 250.f;
    health = 5;
}

void Player::update(float dt) {
    weapon.update(dt);
    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

std::unique_ptr<Bullet> Player::shootTowards(const sf::Vector2f& target) {
    sf::Vector2f dir = target - position;
    auto b = weapon.fire(position, dir);
    return b;
}

void Player::setRotation(float angle) {
    sprite.setRotation(angle + 90);
}

Player& Player::operator+=(const sf::Vector2f& moveVec) {
    position += moveVec;
    return *this;
}

void Player::setPositionBounds(const sf::FloatRect& bounds) {
    if (position.x < bounds.left) position.x = bounds.left;
    if (position.y < bounds.top) position.y = bounds.top;
    if (position.x > bounds.left + bounds.width) position.x = bounds.left + bounds.width;
    if (position.y > bounds.top + bounds.height) position.y = bounds.top + bounds.height;
}

void Player::heal(int v) { health += v; }

Entity::Type Player::getType() const { return Entity::Type::PLAYER; }