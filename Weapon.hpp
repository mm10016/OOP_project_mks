#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <SFML/System.hpp>
#include <memory>

class Bullet;  // Forward declaration

class Weapon {
private:
    float fireRate;
    float cooldown;
    int damage;
    
public:
    Weapon(float fr = 6.f, int dmg = 1);
    
    void update(float dt);
    bool canFire() const;
    std::unique_ptr<Bullet> fire(const sf::Vector2f& from, const sf::Vector2f& dir);
    
    void setFireRate(float f);
    void setDamage(int d);
    float getFireRate() const;
    int getDamage() const;
};

#endif