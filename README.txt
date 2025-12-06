Project Description: A fast-paced top-down space shooter where you maneuver against endless waves of alien enemies. 
Battle through increasingly difficult waves, collect health power-ups, and survive as long as possible in the depths of space.
Get final statistics including score, number of enemies killed, and number of health power-ups at the end of the game.

Instructions on how to play:
Move - WASD keys 
Aim - Mouse cursor 
Shoot - Left mouse button 
Start/Restart - SPACE key 
Quit - ESC key

Use the following command to compile the program:
g++ -std=c++14 -I. Entity.cpp Bullet.cpp Weapon.cpp Enemy.cpp FastEnemy.cpp Player.cpp PowerUp.cpp EnemyManager.cpp CollisionManager.cpp UIManager.cpp  Game.cpp main.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

Then use command  to run it:
./game

The following is the link to our GitHub repository:
https://github.com/mm10016/OOP_project_mks