#include "Entity.hpp"
#include <cmath>

float Entity::length(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f Entity::normalize(const sf::Vector2f& v) {
    float len = length(v);
    if (len == 0.f) return {0.f, 0.f};
    return v / len;
}

Entity::Entity() : position(0, 0), velocity(0, 0), health(1), alive(true) {}
Entity::~Entity() {}

bool Entity::isAlive() const { return alive && health > 0; }
void Entity::kill() { health = 0; alive = false; }
int Entity::getHealth() const { return health; }
void Entity::damage(int d) { health -= d; if (health <= 0) alive = false; }
void Entity::setPosition(const sf::Vector2f& p) { position = p; }
sf::Vector2f Entity::getPosition() const { return position; }

bool Entity::operator==(const Entity& other) const {
    return getBounds().intersects(other.getBounds());
}