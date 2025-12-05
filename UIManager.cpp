#include "UIManager.hpp"
#include <cmath>
#include <iostream>

UIManager::UIManager() : fontLoaded(false), notificationTimer(0.f), showingNotification(false) {
    fontLoaded = font.loadFromFile("assets/arial.ttf");
    
    if (!powerUpTexture.loadFromFile("assets/PowerUp_02.png")) {
        std::cout << "Failed to load assets/PowerUp_02.png for notification\n";
        sf::Image img;
        img.create(64, 64, sf::Color::Green);
        for (int y = 0; y < 64; ++y) {
            for (int x = 0; x < 64; ++x) {
                float dist = std::sqrt((x-32)*(x-32) + (y-32)*(y-32));
                if (dist <= 30) {
                    img.setPixel(x, y, sf::Color::Green);
                } else {
                    img.setPixel(x, y, sf::Color::Transparent);
                }
            }
        }
        powerUpTexture.loadFromImage(img);
    }
    
    powerUpIcon.setTexture(powerUpTexture);
    powerUpIcon.setOrigin(powerUpTexture.getSize().x / 2.0f, powerUpTexture.getSize().y / 2.0f);
    powerUpIcon.setScale(0.8f, 0.8f);
    
    if (fontLoaded) {
        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        
        waveText = scoreText;
        basicKillText = scoreText;
        fastKillText = scoreText;
        
        basicKillText.setFillColor(sf::Color(100, 200, 255));
        fastKillText.setFillColor(sf::Color(255, 100, 100));
        
        healthBarBackground.setSize(sf::Vector2f(200.f, 20.f));
        healthBarBackground.setFillColor(sf::Color(50, 50, 50, 200));
        healthBarBackground.setOutlineThickness(2.f);
        healthBarBackground.setOutlineColor(sf::Color::White);
        
        healthBarFill.setSize(sf::Vector2f(200.f, 20.f));
        healthBarFill.setFillColor(sf::Color(255, 50, 50));
    }
}

void UIManager::update(float dt) {
    if (showingNotification) {
        notificationTimer += dt;
        if (notificationTimer >= 1.5f) {
            showingNotification = false;
            notificationTimer = 0.f;
        }
    }
}

void UIManager::showPowerUpNotification() {
    showingNotification = true;
    notificationTimer = 0.f;
}

void UIManager::draw(sf::RenderWindow& window, int score, int health, int wave,
                     int basicKills, int fastKills, int maxHealth) {
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setPosition(12.f, 8.f);
    
    waveText.setString("Wave: " + std::to_string(wave));
    waveText.setPosition(window.getSize().x - 120.f, 8.f);
    
    basicKillText.setString("Basic: " + std::to_string(basicKills));
    basicKillText.setPosition(window.getSize().x - 150.f, 40.f);
    
    fastKillText.setString("Fast: " + std::to_string(fastKills));
    fastKillText.setPosition(window.getSize().x - 150.f, 65.f);
    
    float centerX = window.getSize().x / 2 - healthBarBackground.getSize().x / 2;
    healthBarBackground.setPosition(centerX, 10.f);
    healthBarFill.setPosition(centerX, 10.f);
    
    float healthPercent = static_cast<float>(health) / maxHealth;
    healthPercent = std::max(0.f, std::min(1.f, healthPercent));
    healthBarFill.setSize(sf::Vector2f(200.f * healthPercent, 20.f));
    
    if (healthPercent > 0.5f) {
        healthBarFill.setFillColor(sf::Color(50, 255, 50));
    } else if (healthPercent > 0.25f) {
        healthBarFill.setFillColor(sf::Color(255, 255, 50));
    } else {
        healthBarFill.setFillColor(sf::Color(255, 50, 50));
    }
    
    if (fontLoaded) {
        window.draw(scoreText);
        window.draw(waveText);
        window.draw(basicKillText);
        window.draw(fastKillText);
    }
    
    window.draw(healthBarBackground);
    window.draw(healthBarFill);
    
    if (showingNotification) {
        float iconX = window.getSize().x - 100.f;
        float iconY = 150.f;
        
        float pulse = std::sin(notificationTimer * 10.f) * 0.1f + 1.0f;
        powerUpIcon.setScale(0.8f * pulse, 0.8f * pulse);
        
        sf::Uint8 alpha = static_cast<sf::Uint8>(255.f * (1.f - (notificationTimer / 1.5f)));
        powerUpIcon.setColor(sf::Color(255, 255, 255, alpha));
        
        powerUpIcon.setPosition(iconX, iconY);
        window.draw(powerUpIcon);
        
        if (fontLoaded) {
            sf::Text plusText;
            plusText.setFont(font);
            plusText.setString("+");
            plusText.setCharacterSize(36);
            plusText.setFillColor(sf::Color(0, 255, 0, alpha));
            plusText.setPosition(iconX - 40.f, iconY - 20.f);
            window.draw(plusText);
        }
    }
}