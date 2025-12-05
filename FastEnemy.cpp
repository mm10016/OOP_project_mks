#include "FastEnemy.hpp"

FastEnemy::FastEnemy(const sf::Vector2f& pos) 
    : Enemy(pos, 160.f, 2, "assets/fast_enemy.png") {}

void FastEnemy::update(float dt) {
    sprite.setPosition(position);
}

Entity::Type FastEnemy::getType() const { 
    return Entity::Type::FAST_ENEMY; 
}