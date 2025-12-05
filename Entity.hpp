#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <memory>

class Entity {
public:
    enum class Type { PLAYER, BASIC_ENEMY, FAST_ENEMY, BULLET, POWERUP };
    
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    int health;
    bool alive;
    
public:
    Entity();
    virtual ~Entity();
    
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;
    virtual Type getType() const = 0;
    
    bool isAlive() const;
    void kill();
    int getHealth() const;
    void damage(int d);
    void setPosition(const sf::Vector2f& p);
    sf::Vector2f getPosition() const;
    
    bool operator==(const Entity& other) const;
    
    // Static utility functions
    static float length(const sf::Vector2f& v);
    static sf::Vector2f normalize(const sf::Vector2f& v);
};

#endif