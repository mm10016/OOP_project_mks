#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <SFML/Graphics.hpp>

class UIManager {
private:
    sf::Font font;
    bool fontLoaded;
    sf::Text scoreText;
    sf::Text waveText;
    sf::Text basicKillText;
    sf::Text fastKillText;
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarFill;
    
    sf::Sprite powerUpIcon;
    sf::Texture powerUpTexture;
    float notificationTimer;
    bool showingNotification;
    
public:
    UIManager();
    
    void draw(sf::RenderWindow& window, int score, int health, int wave,
              int basicKills, int fastKills, int maxHealth);
    void showPowerUpNotification();
    void update(float dt);
};

#endif