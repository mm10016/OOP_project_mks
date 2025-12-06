#include "CollisionManager.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "FastEnemy.hpp"
#include <algorithm>

// Template function to handle collisions between bullets and enemies
// BContainer: container type for bullets
// EContainer: container type for enemies
template<typename BContainer, typename EContainer>
void CollisionManager::handleBulletEnemyCollisions(
    BContainer& bullets,   // Container of bullets to check
    EContainer& enemies,   // Container of enemies to check
    int& score,            // Reference to player's score, updated on kills
    int& basicKills,       // Reference to counter for basic enemy kills
    int& fastKills)        // Reference to counter for fast enemy kills
{
    // Iterate through all bullets
    for (auto& b : bullets) {
        // Skip if bullet is already inactive
        if (!b->isAlive()) continue;
        
        // Check bullet against all enemies
        for (auto& e : enemies) {
            // Skip if enemy is already dead
            if (!e->isAlive()) continue;
            
            // Check for collision using the overloaded == operator
            if ((*b) == (*e)) {
                // Apply bullet damage to enemy
                e->damage(b->getDamage());
                // Deactivate the bullet after impact
                b->kill();
                
                // If enemy died from the damage
                if (!e->isAlive()) {
                    // Award points for the kill
                    score += 100;
                    
                    // Update specific enemy type kill counters
                    if (e->getType() == Entity::Type::BASIC_ENEMY) {
                        basicKills++;
                    } else if (e->getType() == Entity::Type::FAST_ENEMY) {
                        fastKills++;
                    }
                }
                // Break out of enemy loop since bullet can only hit one enemy
                break;
            }
        }
    }
}

// Template function to handle collisions between player and enemies
// EContainer: container type for enemies
template<typename EContainer>
void CollisionManager::handlePlayerEnemyCollisions(
    Player& player,        // The player entity
    EContainer& enemies,   // Container of enemies to check
    int& playerHealth)     // Reference to player's health (for UI updates)
{
    // Check player against all enemies
    for (auto& e : enemies) {
        // Skip if enemy is already dead
        if (!e->isAlive()) continue;
        
        // Check for collision using the overloaded == operator
        if (player == *e) {
            // Apply damage both ways (mutual damage on collision)
            e->damage(1);          // Enemy takes 1 damage
            player.damage(1);      // Player takes 1 damage
            
            // Update player health reference (likely for UI display)
            playerHealth = player.getHealth();
            
            // Calculate knockback direction (away from enemy)
            sf::Vector2f dir = Entity::normalize(player.getPosition() - e->getPosition());
            // Apply knockback to player using overloaded += operator
            player += dir * 20.f;
        }
    }
}

// Explicit template instantiations

// Instantiate for vector of unique_ptr<Bullet> and vector of unique_ptr<Enemy>
template void CollisionManager::handleBulletEnemyCollisions(
    std::vector<std::unique_ptr<Bullet>>&, 
    std::vector<std::unique_ptr<Enemy>>&, 
    int&, int&, int&);

// Instantiate for Player and vector of unique_ptr<Enemy>
template void CollisionManager::handlePlayerEnemyCollisions(
    Player&, 
    std::vector<std::unique_ptr<Enemy>>&, 
    int&);
