#ifndef ENEMYMANAGER_HPP
#define ENEMYMANAGER_HPP

#include "Enemy.hpp"
#include "FastEnemy.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>

class EnemyManager {
private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    float spawnTimer;
    float spawnRate;
    sf::FloatRect bounds;
    int wave;
    std::mt19937 rng;
    std::uniform_real_distribution<float> distX;
    std::uniform_real_distribution<float> distY;
    
public:
    EnemyManager(const sf::FloatRect& playBounds);
    
    void update(float dt, const sf::Vector2f& playerPos);
    void spawnWaveMember();
    void draw(sf::RenderWindow& window);
    
    std::vector<std::unique_ptr<Enemy>>& getEnemies();
    int getEnemyCount() const;
    void increaseDifficulty();
    int getWave() const;
};

#endif