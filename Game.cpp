#include "Game.hpp"
#include <algorithm>
#include <iostream>

// Game constructor - Initializes all game components and resources
// Sets up the game window, player, enemy manager, UI elements, and loads assets
// Also initializes random number generation and game state variables
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
    
    // Set frame rate limit for consistent gameplay
    window.setFramerateLimit(60);
    
    // Load background texture
    if (backgroundTexture.loadFromFile("assets/space1.png")) {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundLoaded = true;
        // Scale background to fit window
        backgroundSprite.setScale(
            window.getSize().x / backgroundTexture.getSize().x,
            window.getSize().y / backgroundTexture.getSize().y
        );
    }
    
    // Initialize random number generator
    rng.seed(std::random_device{}());
    
    // Set up menu text elements
    setupMenuText();
}

// Sets up all text elements for menus (main menu, game over screen)
// Loads the font and configures text properties for all menu screens
// Handles font loading failure gracefully
void Game::setupMenuText() {
    // Load menu font
    if (!menuFont.loadFromFile("assets/arial.ttf")) {
        std::cout << "Failed to load font for menu\n";
        menuFontLoaded = false;
        return;
    }
    menuFontLoaded = true;
    
    // Configure title text
    titleText.setFont(menuFont);
    titleText.setString("Astro Assault");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(1024.f / 2 - 250.f, 100.f);
    
    // Configure start game text
    startText.setFont(menuFont);
    startText.setString("Press SPACE to Start");
    startText.setCharacterSize(36);
    startText.setFillColor(sf::Color::Yellow);
    startText.setPosition(1024.f / 2 - 180.f, 350.f);
    
    // Configure instructions text
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

    // Configure game over text
    gameOverText.setFont(menuFont);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(80);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(1024.f / 2 - 200.f, 100.f);

    // Configure final score text (content set dynamically)
    finalScoreText.setFont(menuFont);
    finalScoreText.setCharacterSize(28);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition(1024.f / 2 - 200.f, 200.f);

    // Configure restart game text
    restartText.setFont(menuFont);
    restartText.setString("Press SPACE to Play Again");
    restartText.setCharacterSize(30);
    restartText.setFillColor(sf::Color::Yellow);
    restartText.setPosition(1024.f / 2 - 180.f, 500.f);
}

// Main game loop - Runs the game until window is closed
// Handles the game state machine: processes events, updates game logic,
// and renders each frame based on current game state
void Game::run() {
    while (window.isOpen()) {
        // Calculate delta time for frame-independent movement
        float dt = clock.restart().asSeconds();
        
        // Process input events
        handleEvents();
        
        // Update game logic only when in PLAYING state
        if (currentState == GameState::PLAYING) {
            update(dt);
        }
        
        // Render current frame
        render();
    }
}

// Handles all window events (keyboard, mouse, window close)
// Processes events like closing window, pressing escape, or space bar
// for state transitions (main menu -> playing, game over -> restart)
void Game::handleEvents() {
    sf::Event ev;
    while (window.pollEvent(ev)) {
        // Close window events
        if (ev.type == sf::Event::Closed) window.close();
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) window.close();
        
        // Space bar handling for state transitions
        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Space) {
                // Start game from main menu
                if (currentState == GameState::MAIN_MENU) {
                    startGame();
                } 
                // Restart game from game over screen
                else if (currentState == GameState::GAME_OVER) {
                    restartGame();
                }
            }
        }
    }
}

// Initializes/resets the game to start playing
// Resets all game variables, creates new player and enemy manager,
// clears all bullets and power-ups, and sets initial game state
void Game::startGame() {
    currentState = GameState::PLAYING;
    score = 0;
    basicEnemiesKilled = 0;
    fastEnemiesKilled = 0;
    totalKills = 0;
    
    // Create new player at center of screen
    player = Player({512.f, 384.f});
    playerMaxHealth = 5;
    
    // Create new enemy manager with play area
    enemyManager = EnemyManager(playArea);
    
    // Clear all active game objects
    bullets.clear();
    powerUps.clear();
    
    // Reset timing variables
    waveTime = 0.f;
    lastPowerUpSpawnCount = 0;
    powerUpsCollected = 0;
}

// Restarts the game from game over state
// Simply calls startGame() to reset all game state
void Game::restartGame() {
    startGame();
}

// Updates all game logic for one frame
// dt: Delta time (seconds since last frame)
// Handles player movement, shooting, collisions, enemy updates,
// power-up spawning/collection, and game state transitions
void Game::update(float dt) {
    // Cap delta time to prevent large jumps if game lags
    dt = std::min(dt, 0.1f);
    
    // Update UI animations/effects
    ui.update(dt);
    
    // PLAYER MOVEMENT
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.f;
    
    // Normalize diagonal movement and apply speed
    if (Entity::length(movement) > 0.f) {
        movement = Entity::normalize(movement) * playerSpeed() * dt;
    }
    
    // Apply movement and keep player within bounds
    player += movement;
    player.setPositionBounds(playArea);

    // PLAYER AIMING
    // Convert mouse position to world coordinates
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    
    // Calculate direction and rotation angle
    sf::Vector2f direction = worldPos - player.getPosition();
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
    player.setRotation(angle);

    // SHOOTING
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        auto b = player.shootTowards(worldPos);
        if (b) bullets.push_back(std::move(b));
    }

    // UPDATE BULLETS
    for (auto& b : bullets) b->update(dt);
    
    // Remove dead bullets
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const std::unique_ptr<Bullet>& b){ return !b->isAlive(); }), bullets.end());

    // UPDATE ENEMIES
    enemyManager.update(dt, player.getPosition());
    
    // BULLET-ENEMY COLLISIONS
    CollisionManager::handleBulletEnemyCollisions(
        bullets, enemyManager.getEnemies(), 
        score, basicEnemiesKilled, fastEnemiesKilled);
    
    // POWER-UP SPAWNING
    // Spawn health power-up every 10 fast enemies killed
    if (fastEnemiesKilled > 0 && fastEnemiesKilled % 10 == 0 && 
        fastEnemiesKilled != lastPowerUpSpawnCount) {
        
        spawnPowerUp(player.getPosition() + sf::Vector2f(
            std::uniform_real_distribution<float>(-100, 100)(rng),
            std::uniform_real_distribution<float>(-100, -50)(rng)
        ));
        lastPowerUpSpawnCount = fastEnemiesKilled;
    }
    
    // UPDATE POWER-UPS
    updatePowerUps(dt);
    checkPowerUpCollection();
    
    // PLAYER-ENEMY COLLISIONS
    int currentPlayerHealth = player.getHealth();
    CollisionManager::handlePlayerEnemyCollisions(player, enemyManager.getEnemies(), currentPlayerHealth);

    // UPDATE PLAYER
    player.update(dt);

    // GAME OVER CHECK 
    if (!player.isAlive()) {
        currentState = GameState::GAME_OVER;
        totalKills = basicEnemiesKilled + fastEnemiesKilled;
        
        // Update final score text with game statistics
        finalScoreText.setString(
            "Final Score: " + std::to_string(score) + 
            "\nTotal Kills: " + std::to_string(totalKills) +
            "\nBasic Enemies: " + std::to_string(basicEnemiesKilled) +
            "\nFast Enemies: " + std::to_string(fastEnemiesKilled) +
            "\nHealth Power-ups: " + std::to_string(powerUpsCollected)
        );
    }

    // WAVE MANAGEMENT
    // Increase difficulty every 12 seconds
    waveTime += dt;
    if (waveTime >= 12.f) {
        waveTime = 0.f;
        enemyManager.increaseDifficulty();
    }
}

// Spawns a health power-up at the specified position
// position: World coordinates where power-up should spawn
void Game::spawnPowerUp(const sf::Vector2f& position) {
    powerUps.emplace_back(std::make_unique<PowerUp>(position));
    std::cout << "Health power-up spawned at position: " << position.x << ", " << position.y << std::endl;
}

// Updates all active power-ups
// dt: Delta time for frame-independent updates
void Game::updatePowerUps(float dt) {
    for (auto& p : powerUps) {
        p->update(dt);
    }
    
    // Remove dead power-ups
    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
        [](const std::unique_ptr<PowerUp>& p){ return !p->isAlive(); }), 
        powerUps.end());
}

// Checks for and handles player collecting power-ups
// When player collides with a power-up, they gain health and
// their maximum health increases. Also triggers UI notification.
void Game::checkPowerUpCollection() {
    for (auto& p : powerUps) {
        if (!p->isAlive()) continue;
        
        // Check collision between player and power-up
        if (player.getBounds().intersects(p->getBounds())) {
            // Apply power-up effects
            player.heal(2);
            playerMaxHealth += 1;
            
            // Show UI notification
            ui.showPowerUpNotification();
            
            // Log for debugging
            std::cout << "Health power-up collected! Healed 2 HP, Max HP now: " 
                      << playerMaxHealth << std::endl;
            
            // Remove power-up and increment counter
            p->kill();
            powerUpsCollected++;
        }
    }
}

// Renders the current frame based on game state
// Clears the screen and renders either the main menu, game,
// or game over screen depending on current state
void Game::render() {
    // Clear screen with dark blue color
    window.clear(sf::Color(20, 20, 40));
    
    // Render appropriate screen based on game state
    if (currentState == GameState::MAIN_MENU) {
        renderMainMenu();
    } else if (currentState == GameState::PLAYING) {
        renderGame();
    } else if (currentState == GameState::GAME_OVER) {
        renderGameOver();
    }

    // Display rendered frame
    window.display();
}

// Renders the main menu screen
// Draws background (or grid fallback) and all menu text elements
void Game::renderMainMenu() {
    // Draw background or grid fallback
    if (backgroundLoaded) {
        window.draw(backgroundSprite);
    } else {
        drawGrid();
    }

    // Draw menu text if font loaded successfully
    if (menuFontLoaded) {
        window.draw(titleText);
        window.draw(startText);
        window.draw(instructionsText);
    }
}

// Renders the gameplay screen
// Draws all game entities: background, bullets, power-ups,
// enemies, player, and UI elements
void Game::renderGame() {
    // Draw background or grid fallback
    if (backgroundLoaded) {
        window.draw(backgroundSprite);
    } else {
        drawGrid();
    }

    // Draw all active game objects
    for (auto& b : bullets) b->draw(window);
    for (auto& p : powerUps) p->draw(window);
    enemyManager.draw(window);
    player.draw(window);

    // Draw UI with current game stats
    ui.draw(window, score, player.getHealth(), enemyManager.getWave(),
            basicEnemiesKilled, fastEnemiesKilled, playerMaxHealth);
}

// Renders the game over screen
// Draws background and game over text with final statistics
void Game::renderGameOver() {
    // Draw background or grid fallback
    if (backgroundLoaded) {
        window.draw(backgroundSprite);
    } else {
        drawGrid();
    }

    // Draw game over text and statistics
    if (menuFontLoaded) {
        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(restartText);
    }
}

// Draws a grid pattern as fallback background
// Used when background texture fails to load, creates
// a simple grid pattern for visual reference
void Game::drawGrid() {
    const int tile = 64;
    sf::RectangleShape line;
    line.setFillColor(sf::Color(30, 30, 60, 100));
    
    // Draw vertical grid lines
    for (int x = 0; x <= (int)window.getSize().x; x += tile) {
        line.setSize({1.f, (float)window.getSize().y});
        line.setPosition((float)x, 0);
        window.draw(line);
    }
    
    // Draw horizontal grid lines
    for (int y = 0; y <= (int)window.getSize().y; y += tile) {
        line.setSize({(float)window.getSize().x, 1.f});
        line.setPosition(0,(float)y);
        window.draw(line);
    }
}

// Returns the player's movement speed
// Returns: Player speed in pixels per second
float Game::playerSpeed() const { 
    return 300.f; 
}
