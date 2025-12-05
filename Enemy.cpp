#include "Enemy.hpp"
#include <cmath>
#include <iostream>

Enemy::Enemy(const sf::Vector2f& pos, float spd, int hp, const std::string& texturePath) {
    position = pos;
    speed = spd;
    health = hp;
    
    if (!texture.loadFromFile(texturePath)) {
        std::cout << "Failed to load " << texturePath << "\n";
    }
    
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f);
}

void Enemy::updateTowards(const sf::Vector2f& target, float dt) {
    sf::Vector2f dir = normalize(target - position);
    position += dir * speed * dt;
    sprite.setPosition(position);
    
    float angle = std::atan2(dir.y, dir.x) * 180 / 3.14159f;
    sprite.setRotation(angle + 90);
}

void Enemy::update(float dt) {
    sprite.setPosition(position);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

Entity::Type Enemy::getType() const { 
    return Entity::Type::BASIC_ENEMY; 
}