#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "EnemyManager.hpp"
#include "UIManager.hpp"
#include "Bullet.hpp"
#include "PowerUp.hpp"
#include "CollisionManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    Player player;
    EnemyManager enemyManager;
    UIManager ui;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<PowerUp>> powerUps;
    int score;
    sf::FloatRect playArea;
    float spawnProgressTimer;
    float waveTime;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool backgroundLoaded;

    int basicEnemiesKilled;
    int fastEnemiesKilled;
    int totalKills;
    int playerMaxHealth;
    int lastPowerUpSpawnCount;
    int powerUpsCollected;
    
    std::mt19937 rng;

    enum class GameState { MAIN_MENU, PLAYING, GAME_OVER };
    GameState currentState;
    sf::Font menuFont;
    sf::Text titleText;
    sf::Text startText;
    sf::Text instructionsText;
    sf::Text gameOverText;
    sf::Text finalScoreText;
    sf::Text restartText;
    bool menuFontLoaded;

    void setupMenuText();
    void handleEvents();
    void startGame();
    void restartGame();
    void update(float dt);
    void render();
    void renderMainMenu();
    void renderGame();
    void renderGameOver();
    void drawGrid();
    float playerSpeed() const;

    void spawnPowerUp(const sf::Vector2f& position);
    void updatePowerUps(float dt);
    void checkPowerUpCollection();

public:
    Game();
    void run();
};

#endif