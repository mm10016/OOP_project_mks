#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"  
#include "Weapon.hpp"  
#include <SFML/Graphics.hpp>
#include <memory>

class Bullet;

class Player : public Entity {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    Weapon weapon;
    float speed;
    
public:
    Player(const sf::Vector2f& startPos = {400.f,300.f});
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    Type getType() const override;
    
    std::unique_ptr<Bullet> shootTowards(const sf::Vector2f& target);
    void setRotation(float angle);
    Player& operator+=(const sf::Vector2f& moveVec);
    void setPositionBounds(const sf::FloatRect& bounds);
    void heal(int v);
};

#endif