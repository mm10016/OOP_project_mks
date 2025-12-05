#ifndef BULLET_HPP
#define BULLET_HPP

#include "Entity.hpp"

class Bullet : public Entity {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float lifeTimer;
    float lifetime;
    int dmg;
    
public:
    Bullet(const sf::Vector2f& pos, const sf::Vector2f& dir, 
           float speed = 600.f, float lt = 2.f, int damage = 1);
    
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    Type getType() const override;
    
    int getDamage() const;
};

#endif