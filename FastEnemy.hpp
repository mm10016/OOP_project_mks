#ifndef FASTENEMY_HPP
#define FASTENEMY_HPP

#include "Enemy.hpp"

class FastEnemy : public Enemy {
public:
    FastEnemy(const sf::Vector2f& pos = {0, 0});
    void update(float dt) override;
    Type getType() const override;
};

#endif