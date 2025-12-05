#include "Bullet.hpp"
#include <cmath>
#include <iostream>

Bullet::Bullet(const sf::Vector2f& pos, const sf::Vector2f& dir, 
               float speed, float lt, int damage) {
    position = pos;
    velocity = normalize(dir) * speed;
    
    if (!texture.loadFromFile("assets/bullet.png")) {
        std::cout << "Failed to load assets/bullet.png\n";
    }
    
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f);
    
    float angle = std::atan2(dir.y, dir.x) * 180 / 3.14159f;
    sprite.setRotation(angle);
    
    lifetime = lt;
    lifeTimer = 0.f;
    dmg = damage;
    health = 1;
}

void Bullet::update(float dt) {
    position += velocity * dt;
    sprite.setPosition(position);
    lifeTimer += dt;
    if (lifeTimer >= lifetime) alive = false;
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}

Entity::Type Bullet::getType() const { 
    return Entity::Type::BULLET; 
}

int Bullet::getDamage() const { 
    return dmg; 
}