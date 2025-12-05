#include "EnemyManager.hpp"
#include <algorithm>

EnemyManager::EnemyManager(const sf::FloatRect& playBounds) 
    : spawnTimer(0.f), spawnRate(1.2f), bounds(playBounds), wave(1) {
    rng.seed(std::random_device{}());
    distX = std::uniform_real_distribution<float>(bounds.left, bounds.left + bounds.width);
    distY = std::uniform_real_distribution<float>(bounds.top, bounds.top + bounds.height);
}

void EnemyManager::update(float dt, const sf::Vector2f& playerPos) {
    spawnTimer += dt;
    if (spawnTimer >= spawnRate) {
        spawnTimer -= spawnRate;
        spawnWaveMember();
    }
    
    for (auto& e : enemies) {
        e->updateTowards(playerPos, dt);
    }
    
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e){ return !e->isAlive(); }), 
        enemies.end());
}

void EnemyManager::spawnWaveMember() {
    float side = std::uniform_real_distribution<float>(0.f, 1.f)(rng);
    sf::Vector2f pos;
    if (side < 0.25f) pos = {bounds.left - 20.f, distY(rng)};
    else if (side < 0.5f) pos = {bounds.left + bounds.width + 20.f, distY(rng)};
    else if (side < 0.75f) pos = {distX(rng), bounds.top - 20.f};
    else pos = {distX(rng), bounds.top + bounds.height + 20.f};

    float type = std::uniform_real_distribution<float>(0.f, 1.f)(rng);
    if (type < 0.7f) {
        enemies.emplace_back(std::make_unique<Enemy>(pos, 90.f, 1));
    } else {
        enemies.emplace_back(std::make_unique<FastEnemy>(pos));
    }
}

void EnemyManager::draw(sf::RenderWindow& window) {
    for (auto& e : enemies) e->draw(window);
}

std::vector<std::unique_ptr<Enemy>>& EnemyManager::getEnemies() { 
    return enemies; 
}

int EnemyManager::getEnemyCount() const { 
    return (int)enemies.size(); 
}

void EnemyManager::increaseDifficulty() {
    wave++;
    if (spawnRate > 0.35f) spawnRate *= 0.85f;
}

int EnemyManager::getWave() const { 
    return wave; 
}