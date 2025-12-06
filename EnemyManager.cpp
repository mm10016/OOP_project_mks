#include "EnemyManager.hpp"
#include <algorithm>

// Constructor: Initializes the enemy manager with play area bounds
EnemyManager::EnemyManager(const sf::FloatRect& playBounds) 
    : spawnTimer(0.f),          // Timer for spawning enemies
      spawnRate(1.2f),          // Initial spawn interval in seconds
      bounds(playBounds),       // Playable area boundaries
      wave(1) {                 // Current wave number
    // Initialize random number generator with hardware seed
    rng.seed(std::random_device{}());
    // Set up distributions for random spawn positions within bounds
    distX = std::uniform_real_distribution<float>(bounds.left, bounds.left + bounds.width);
    distY = std::uniform_real_distribution<float>(bounds.top, bounds.top + bounds.height);
}

// Update all enemies and handle spawning
void EnemyManager::update(float dt, const sf::Vector2f& playerPos) {
    // Update spawn timer
    spawnTimer += dt;
    
    // Check if it's time to spawn a new enemy
    if (spawnTimer >= spawnRate) {
        spawnTimer -= spawnRate;  // Reset timer with remainder
        spawnWaveMember();        // Spawn a single enemy
    }
    
    // Update all active enemies to move toward player
    for (auto& e : enemies) {
        e->updateTowards(playerPos, dt);
    }
    
    // Remove dead enemies using erase-remove idiom
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e){ 
            return !e->isAlive();  // Predicate: remove if not alive
        }), 
        enemies.end());
}

// Spawn a single enemy for the current wave
void EnemyManager::spawnWaveMember() {
    // Randomly choose which side of the screen to spawn from
    float side = std::uniform_real_distribution<float>(0.f, 1.f)(rng);
    sf::Vector2f pos;
    
    // Determine spawn position just outside the play area
    if (side < 0.25f) {
        // Left side
        pos = {bounds.left - 20.f, distY(rng)};
    } else if (side < 0.5f) {
        // Right side
        pos = {bounds.left + bounds.width + 20.f, distY(rng)};
    } else if (side < 0.75f) {
        // Top side
        pos = {distX(rng), bounds.top - 20.f};
    } else {
        // Bottom side
        pos = {distX(rng), bounds.top + bounds.height + 20.f};
    }

    // Randomly determine enemy type (70% basic, 30% fast)
    float type = std::uniform_real_distribution<float>(0.f, 1.f)(rng);
    if (type < 0.7f) {
        // Spawn basic enemy at position with 90 speed and 1 health
        enemies.emplace_back(std::make_unique<Enemy>(pos, 90.f, 1));
    } else {
        // Spawn fast enemy
        enemies.emplace_back(std::make_unique<FastEnemy>(pos));
    }
}

// Draw all enemies to the window
void EnemyManager::draw(sf::RenderWindow& window) {
    for (auto& e : enemies) {
        e->draw(window);
    }
}

// Get reference to enemies container (for collision detection)
std::vector<std::unique_ptr<Enemy>>& EnemyManager::getEnemies() { 
    return enemies; 
}

// Get current number of active enemies
int EnemyManager::getEnemyCount() const { 
    return (int)enemies.size(); 
}

// Increase difficulty as game progresses (called when wave completes)
void EnemyManager::increaseDifficulty() {
    wave++;  // Increment wave counter
    
    // Decrease spawn interval (enemies spawn faster), with minimum 0.35s
    if (spawnRate > 0.35f) {
        spawnRate *= 0.85f;  // Reduce by 15% each wave
    }
}

// Get current wave number
int EnemyManager::getWave() const { 
    return wave; 
}
