#ifndef COLLISIONMANAGER_HPP
#define COLLISIONMANAGER_HPP

#include <vector>
#include <memory>

// Forward declarations
class Bullet;
class Enemy;
class Player;

class CollisionManager {
public:
    template<typename BContainer, typename EContainer>
    static void handleBulletEnemyCollisions(BContainer& bullets, EContainer& enemies, 
                                           int& score, int& basicKills, int& fastKills);
    
    template<typename EContainer>
    static void handlePlayerEnemyCollisions(Player& player, EContainer& enemies, int& playerHealth);
};

#endif