#include "UIManager.hpp"
#include <cmath>
#include <iostream>

// Constructor: Initializes UI elements and loads assets
UIManager::UIManager() : fontLoaded(false), notificationTimer(0.f), showingNotification(false) {
    // Load the main font for text rendering
    fontLoaded = font.loadFromFile("assets/arial.ttf");
    
    // Try to load the power-up notification texture
    if (!powerUpTexture.loadFromFile("assets/PowerUp_02.png")) {
        // If texture file not found, create a fallback placeholder graphic
        std::cout << "Failed to load assets/PowerUp_02.png for notification\n";
        sf::Image img;
        img.create(64, 64, sf::Color::Green); // Create 64x64 image with green background
        
        // Draw a green circle as fallback power-up icon
        for (int y = 0; y < 64; ++y) {
            for (int x = 0; x < 64; ++x) {
                float dist = std::sqrt((x-32)*(x-32) + (y-32)*(y-32)); // Distance from center
                if (dist <= 30) {
                    img.setPixel(x, y, sf::Color::Green); // Inside circle: green
                } else {
                    img.setPixel(x, y, sf::Color::Transparent); // Outside: transparent
                }
            }
        }
        powerUpTexture.loadFromImage(img); // Load the generated image as texture
    }
    
    // Configure the power-up icon sprite
    powerUpIcon.setTexture(powerUpTexture);
    // Center the sprite's origin for rotation/scale
    powerUpIcon.setOrigin(powerUpTexture.getSize().x / 2.0f, powerUpTexture.getSize().y / 2.0f);
    powerUpIcon.setScale(0.8f, 0.8f); // Slightly reduce size
    
    // If font loaded successfully, set up all text elements
    if (fontLoaded) {
        // Main score text (top-left)
        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        
        // Clone the style for other text elements
        waveText = scoreText;          // Wave counter (top-right)
        basicKillText = scoreText;     // Basic enemy kill count
        fastKillText = scoreText;      // Fast enemy kill count
        
        // Custom colors for kill counters
        basicKillText.setFillColor(sf::Color(100, 200, 255));  // Light blue
        fastKillText.setFillColor(sf::Color(255, 100, 100));   // Light red
        
        // Health bar background (dark gray with outline)
        healthBarBackground.setSize(sf::Vector2f(200.f, 20.f));
        healthBarBackground.setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent dark
        healthBarBackground.setOutlineThickness(2.f);
        healthBarBackground.setOutlineColor(sf::Color::White);
        
        // Health bar fill (starts as red, changes based on health)
        healthBarFill.setSize(sf::Vector2f(200.f, 20.f));
        healthBarFill.setFillColor(sf::Color(255, 50, 50)); // Initial red color
    }
}

// Update UI animations and timers
void UIManager::update(float dt) {
    // If a power-up notification is active, update its timer
    if (showingNotification) {
        notificationTimer += dt;
        // Hide notification after 1.5 seconds
        if (notificationTimer >= 1.5f) {
            showingNotification = false;
            notificationTimer = 0.f;
        }
    }
}

// Trigger a power-up collected notification
void UIManager::showPowerUpNotification() {
    showingNotification = true;
    notificationTimer = 0.f; // Reset timer for new notification
}

// Draw all UI elements to the window
void UIManager::draw(sf::RenderWindow& window, int score, int health, int wave,
                     int basicKills, int fastKills, int maxHealth) {
    // Update and position score text (top-left corner)
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setPosition(12.f, 8.f);
    
    // Update and position wave text (top-right corner)
    waveText.setString("Wave: " + std::to_string(wave));
    waveText.setPosition(window.getSize().x - 120.f, 8.f);
    
    // Update and position basic enemy kill counter
    basicKillText.setString("Basic: " + std::to_string(basicKills));
    basicKillText.setPosition(window.getSize().x - 150.f, 40.f);
    
    // Update and position fast enemy kill counter
    fastKillText.setString("Fast: " + std::to_string(fastKills));
    fastKillText.setPosition(window.getSize().x - 150.f, 65.f);
    
    // Position health bar at top center of window
    float centerX = window.getSize().x / 2 - healthBarBackground.getSize().x / 2;
    healthBarBackground.setPosition(centerX, 10.f);
    healthBarFill.setPosition(centerX, 10.f);
    
    // Calculate health percentage (clamped between 0 and 1)
    float healthPercent = static_cast<float>(health) / maxHealth;
    healthPercent = std::max(0.f, std::min(1.f, healthPercent));
    
    // Set health bar width based on health percentage
    healthBarFill.setSize(sf::Vector2f(200.f * healthPercent, 20.f));
    
    // Change health bar color based on health percentage:
    if (healthPercent > 0.5f) {
        healthBarFill.setFillColor(sf::Color(50, 255, 50));     // Green (healthy)
    } else if (healthPercent > 0.25f) {
        healthBarFill.setFillColor(sf::Color(255, 255, 50));    // Yellow (warning)
    } else {
        healthBarFill.setFillColor(sf::Color(255, 50, 50));     // Red (critical)
    }
    
    // Draw all text elements if font was loaded
    if (fontLoaded) {
        window.draw(scoreText);
        window.draw(waveText);
        window.draw(basicKillText);
        window.draw(fastKillText);
    }
    
    // Draw health bar (background and fill)
    window.draw(healthBarBackground);
    window.draw(healthBarFill);
    
    // Draw power-up notification if active
    if (showingNotification) {
        // Position notification icon in top-right area
        float iconX = window.getSize().x - 100.f;
        float iconY = 150.f;
        
        // Create pulsing animation using sine wave
        float pulse = std::sin(notificationTimer * 10.f) * 0.1f + 1.0f;
        powerUpIcon.setScale(0.8f * pulse, 0.8f * pulse);
        
        // Fade out effect as notification timer progresses
        sf::Uint8 alpha = static_cast<sf::Uint8>(255.f * (1.f - (notificationTimer / 1.5f)));
        powerUpIcon.setColor(sf::Color(255, 255, 255, alpha));
        
        // Set position and draw the icon
        powerUpIcon.setPosition(iconX, iconY);
        window.draw(powerUpIcon);
        
        // Draw a "+" symbol next to the icon if font is available
        if (fontLoaded) {
            sf::Text plusText;
            plusText.setFont(font);
            plusText.setString("+");
            plusText.setCharacterSize(36);
            plusText.setFillColor(sf::Color(0, 255, 0, alpha)); // Green with same alpha
            plusText.setPosition(iconX - 40.f, iconY - 20.f);
            window.draw(plusText);
        }
    }
}
