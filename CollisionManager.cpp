#include "CollisionManager.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "FastEnemy.hpp"
#include <algorithm>

template<typename BContainer, typename EContainer>
void CollisionManager::handleBulletEnemyCollisions(BContainer& bullets, EContainer& enemies, 
                                                   int& score, int& basicKills, int& fastKills) {
    for (auto& b : bullets) {
        if (!b->isAlive()) continue;
        for (auto& e : enemies) {
            if (!e->isAlive()) continue;
            if ((*b) == (*e)) {
                e->damage(b->getDamage());
                b->kill();
                if (!e->isAlive()) {
                    score += 100;
                    
                    if (e->getType() == Entity::Type::BASIC_ENEMY) {
                        basicKills++;
                    } else if (e->getType() == Entity::Type::FAST_ENEMY) {
                        fastKills++;
                    }
                }
                break;
            }
        }
    }
}

template<typename EContainer>
void CollisionManager::handlePlayerEnemyCollisions(Player& player, EContainer& enemies, int& playerHealth) {
    for (auto& e : enemies) {
        if (!e->isAlive()) continue;
        if (player == *e) {
            e->damage(1);
            player.damage(1);
            playerHealth = player.getHealth();

            sf::Vector2f dir = Entity::normalize(player.getPosition() - e->getPosition());
            player += dir * 20.f;
        }
    }
}

// Explicit template instantiations for the types we use
template void CollisionManager::handleBulletEnemyCollisions(
    std::vector<std::unique_ptr<Bullet>>&, 
    std::vector<std::unique_ptr<Enemy>>&, 
    int&, int&, int&);

template void CollisionManager::handlePlayerEnemyCollisions(
    Player&, 
    std::vector<std::unique_ptr<Enemy>>&, 
    int&);