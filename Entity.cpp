#include "Entity.hpp"
#include <cmath>

// Calculate the length (magnitude) of a 2D vector
float Entity::length(const sf::Vector2f& v) {
    // Use Pythagorean theorem: sqrt(x² + y²)
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// Normalize a 2D vector (convert to unit vector)
sf::Vector2f Entity::normalize(const sf::Vector2f& v) {
    float len = length(v);  // Get vector length
    if (len == 0.f) return {0.f, 0.f};  // Avoid division by zero
    return v / len;  // Scale vector to length 1
}

// Constructor: Initialize entity with default values
Entity::Entity() 
    : position(0, 0),   // Start at origin (0,0)
      velocity(0, 0),   // No initial movement
      health(1),        // Default 1 health point
      alive(true) {}    // Entity starts alive

// Virtual destructor 
Entity::~Entity() {}

// Check if entity is currently alive
bool Entity::isAlive() const { 
    // Entity is alive if both flags indicate it
    return alive && health > 0; 
}

// Kill the entity immediately
void Entity::kill() { 
    health = 0; 
    alive = false; 
}

// Get current health value
int Entity::getHealth() const { 
    return health; 
}

// Apply damage to the entity
void Entity::damage(int d) { 
    health -= d;  // Reduce health by damage amount
    if (health <= 0) alive = false;  // Mark as dead if health reaches 0 or below
}

// Set entity's position
void Entity::setPosition(const sf::Vector2f& p) { 
    position = p; 
}

// Get entity's current position
sf::Vector2f Entity::getPosition() const { 
    return position; 
}

// Collision detection operator: check if this entity collides with another
bool Entity::operator==(const Entity& other) const {
    // Returns true if the two entities' bounds overlap
    return getBounds().intersects(other.getBounds());
}
