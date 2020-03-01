# Qt game with pathfinder and AI mode
The player must fight and kill all enemies in the map. There are health packs scattered around where the player can pick up. The player loses the game when there's no more health or is out of energy. The player loses the energy when he or she walks on a tile, but the energy can be regained by killing an enemy.

Goal: Win the game with fewest possible moves as possible

The AI mode will show the most optimal solution at a given map configuration.

A test has been made on the pathfinding algorithm to determine the efficiency of the application. The player was able to navigate through a 4,000,000-pixel maze in under 1 second.

## Features
* Autopilot mode (AI mode)
* Graphical user interface
* Command line interface
* Selection of the number of enemies and health packs
* Level 2 enemies: may affect the player with poison which degrades the health over time
* Level 3 enemies: shoots a bomb at every random second which will follow the player until it explodes
* SFXs and BGMs
* May navigate the player with arrow keys or mouse right-click
* Health and energy bar

Made with Qt Creator, using Qt framework and C++17.
