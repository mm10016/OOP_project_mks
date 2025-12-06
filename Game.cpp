#include "Game.hpp"
#include <algorithm>
#include <iostream>

Game::Game() :
    window(sf::VideoMode(1024, 768), "Astro Assault - Top-Down Shooter"),
    player({512.f, 384.f}),
    playArea(0.f, 0.f, 1024.f, 768.f),
    enemyManager(playArea),
    score(0),
    spawnProgressTimer(0.f),
    waveTime(0.f),
    backgroundLoaded(false),
    currentState(GameState::MAIN_MENU),
    menuFontLoaded(false),
    basicEnemiesKilled(0),
    fastEnemiesKilled(0),
    totalKills(0),
    playerMaxHealth(5),
    lastPowerUpSpawnCount(0),
    powerUpsCollected(0) {
    
    window.setFramerateLimit(60);
    
    if (backgroundTexture.loadFromFile("assets/background.png")) {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundLoaded = true;
        backgroundSprite.setScale(
            window.getSize().x / backgroundTexture.getSize().x,
            window.getSize().y / backgroundTexture.getSize().y
        );
    }
    
    rng.seed(std::random_device{}());
    setupMenuText();
}

void Game::setupMenuText() {
    if (!menuFont.loadFromFile("assets/arial.ttf")) {
        std::cout << "Failed to load font for menu\n";
        menuFontLoaded = false;
        return;
    }
    menuFontLoaded = true;
    
    titleText.setFont(menuFont);
    titleText.setString("Astro Assault");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(1024.f / 2 - 250.f, 100.f);
    
    startText.setFont(menuFont);
    startText.setString("Press SPACE to Start");
    startText.setCharacterSize(36);
    startText.setFillColor(sf::Color::Yellow);
    startText.setPosition(1024.f / 2 - 180.f, 350.f);
    
    instructionsText.setFont(menuFont);
    instructionsText.setString(
        "WASD - Move\n"
        "Mouse - Aim\n"
        "Left Click - Shoot\n"
        "Kill 10 fast enemies for health power-ups!\n"
        "Survive as long as possible!"
    );
    instructionsText.setCharacterSize(24);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition(1024.f / 2 - 200.f, 450.f);

    gameOverText.setFont(menuFont);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(80);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(1024.f / 2 - 200.f, 100.f);

    finalScoreText.setFont(menuFont);
    finalScoreText.setCharacterSize(28);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition(1024.f / 2 - 200.f, 200.f);

    restartText.setFont(menuFont);
    restartText.setString("Press SPACE to Play Again");
    restartText.setCharacterSize(30);
    restartText.setFillColor(sf::Color::Yellow);
    restartText.setPosition(1024.f / 2 - 180.f, 500.f);
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleEvents();
        
        if (currentState == GameState::PLAYING) {
            update(dt);
        }
        
        render();
    }
}

void Game::handleEvents() {
    sf::Event ev;
    while (window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) window.close();
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) window.close();
        
        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Space) {
                if (currentState == GameState::MAIN_MENU) {
                    startGame();
                } else if (currentState == GameState::GAME_OVER) {
                    restartGame();
                }
            }
        }
    }
}

void Game::startGame() {
    currentState = GameState::PLAYING;
    score = 0;
    basicEnemiesKilled = 0;
    fastEnemiesKilled = 0;
    totalKills = 0;
    player = Player({512.f, 384.f});
    playerMaxHealth = 5;
    enemyManager = EnemyManager(playArea);
    bullets.clear();
    powerUps.clear();
    waveTime = 0.f;
    lastPowerUpSpawnCount = 0;
    powerUpsCollected = 0;
}

void Game::restartGame() {
    startGame();
}

void Game::update(float dt) {
    dt = std::min(dt, 0.1f);
    
    ui.update(dt);
    
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.f;
    if (Entity::length(movement) > 0.f) movement = Entity::normalize(movement) * playerSpeed() * dt;
    player += movement;
    player.setPositionBounds(playArea);

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    sf::Vector2f direction = worldPos - player.getPosition();
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
    player.setRotation(angle);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        auto b = player.shootTowards(worldPos);
        if (b) bullets.push_back(std::move(b));
    }

    for (auto& b : bullets) b->update(dt);
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const std::unique_ptr<Bullet>& b){ return !b->isAlive(); }), bullets.end());

    enemyManager.update(dt, player.getPosition());
    
    CollisionManager::handleBulletEnemyCollisions(
        bullets, enemyManager.getEnemies(), 
        score, basicEnemiesKilled, fastEnemiesKilled);
    
    if (fastEnemiesKilled > 0 && fastEnemiesKilled % 10 == 0 && 
        fastEnemiesKilled != lastPowerUpSpawnCount) {
        
        spawnPowerUp(player.getPosition() + sf::Vector2f(
            std::uniform_real_distribution<float>(-100, 100)(rng),
            std::uniform_real_distribution<float>(-100, -50)(rng)
        ));
        lastPowerUpSpawnCount = fastEnemiesKilled;
    }
    
    updatePowerUps(dt);
    checkPowerUpCollection();
    
    int currentPlayerHealth = player.getHealth();
    CollisionManager::handlePlayerEnemyCollisions(player, enemyManager.getEnemies(), currentPlayerHealth);

    player.update(dt);

    if (!player.isAlive()) {
        currentState = GameState::GAME_OVER;
        totalKills = basicEnemiesKilled + fastEnemiesKilled;
        finalScoreText.setString(
            "Final Score: " + std::to_string(score) + 
            "\nTotal Kills: " + std::to_string(totalKills) +
            "\nBasic Enemies: " + std::to_string(basicEnemiesKilled) +
            "\nFast Enemies: " + std::to_string(fastEnemiesKilled) +
            "\nHealth Power-ups: " + std::to_string(powerUpsCollected)
        );
    }

    waveTime += dt;
    if (waveTime >= 12.f) {
        waveTime = 0.f;
        enemyManager.increaseDifficulty();
    }
}

void Game::spawnPowerUp(const sf::Vector2f& position) {
    powerUps.emplace_back(std::make_unique<PowerUp>(position));
    std::cout << "Health power-up spawned at position: " << position.x << ", " << position.y << std::endl;
}

void Game::updatePowerUps(float dt) {
    for (auto& p : powerUps) {
        p->update(dt);
    }
    
    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
        [](const std::unique_ptr<PowerUp>& p){ return !p->isAlive(); }), 
        powerUps.end());
}

void Game::checkPowerUpCollection() {
    for (auto& p : powerUps) {
        if (!p->isAlive()) continue;
        
        if (player.getBounds().intersects(p->getBounds())) {
            player.heal(2);
            playerMaxHealth += 1;
            
            ui.showPowerUpNotification();
            
            std::cout << "Health power-up collected! Healed 2 HP, Max HP now: " 
                      << playerMaxHealth << std::endl;
            
            p->kill();
            powerUpsCollected++;
        }
    }
}

void Game::render() {
    window.clear(sf::Color(20, 20, 40));
    
    if (currentState == GameState::MAIN_MENU) {
        renderMainMenu();
    } else if (currentState == GameState::PLAYING) {
        renderGame();
    } else if (currentState == GameState::GAME_OVER) {
        renderGameOver();
    }

    window.display();
}

void Game::renderMainMenu() {
    if (backgroundLoaded) {
        window.draw(backgroundSprite);
    } else {
        drawGrid();
    }

    if (menuFontLoaded) {
        window.draw(titleText);
        window.draw(startText);
        window.draw(instructionsText);
    }
}

void Game::renderGame() {
    if (backgroundLoaded) {
        window.draw(backgroundSprite);
    } else {
        drawGrid();
    }

    for (auto& b : bullets) b->draw(window);
    for (auto& p : powerUps) p->draw(window);
    enemyManager.draw(window);
    player.draw(window);

    ui.draw(window, score, player.getHealth(), enemyManager.getWave(),
            basicEnemiesKilled, fastEnemiesKilled, playerMaxHealth);
}

void Game::renderGameOver() {
    if (backgroundLoaded) {
        window.draw(backgroundSprite);
    } else {
        drawGrid();
    }

    if (menuFontLoaded) {
        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(restartText);
    }
}

void Game::drawGrid() {
    const int tile = 64;
    sf::RectangleShape line;
    line.setFillColor(sf::Color(30, 30, 60, 100));
    for (int x = 0; x <= (int)window.getSize().x; x += tile) {
        line.setSize({1.f, (float)window.getSize().y});
        line.setPosition((float)x, 0);
        window.draw(line);
    }
    for (int y = 0; y <= (int)window.getSize().y; y += tile) {
        line.setSize({(float)window.getSize().x, 1.f});
        line.setPosition(0,(float)y);
        window.draw(line);
    }
}

float Game::playerSpeed() const { 
    return 300.f; 
}