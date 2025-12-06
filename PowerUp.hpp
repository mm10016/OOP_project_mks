#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Entity.hpp"

class PowerUp : public Entity {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    float lifetime;
    float lifeTimer;
    int powerType; // Always 0 for health power-up
    
public:
    PowerUp(const sf::Vector2f& pos);
    
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    Type getType() const override;
    
    int getPowerType() const;
};

#endif