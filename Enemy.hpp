#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

class Enemy : public Entity {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    
public:
    Enemy(const sf::Vector2f& pos = {0, 0}, float spd = 90.f, int hp = 1, 
          const std::string& texturePath = "assets/enemy.png");
    
    virtual void updateTowards(const sf::Vector2f& target, float dt);
    virtual void update(float dt) override;
    virtual void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    Type getType() const override;
};

#endif