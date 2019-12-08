# Space Invaders LED Matrix

## About the game
Space Invaders Retro Game is a LED Matrix Arduino game which expands the original Space Invaders. 

## How to play
On the LCD will appear the menu with the options: START GAME, SETTINGS, HIGHSCORE and INFO. In settings, each player can set how many 
lives will have (default is 3) and the its name (each setting will be modified after the press of the switch button). In highscore will be
displayed the name of the player who get the best score and his score.

## Game specifications
The ship can be moved only with the joystick on x-axis direction and at each press of the switch button it will fire rackets.
The difficulty of the game will increase with each level(the ship's speed will increase and the enemies will have new powers).

First highscore with the name of the player will be saved in EEPROM, and on each level the player will get the chance to get special
powers. A special power(different missile firing mechanism) can be used only once at the press of pushbutton.

The player will get points for destroying the enemy's ships, bonus score for finishing the level without loosing lives.

**Required Items:**
* Arduino Board
* Conector Cable
* 8x8 LED matrix
* LCD
* MAX7219 Driver
* Joystick
* Pushbutton
* Buzzer
* RGB Led
* Wires
* Resistors
