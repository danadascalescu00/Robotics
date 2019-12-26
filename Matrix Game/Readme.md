# Space Invaders LED Matrix Game

## About the game
Space Invaders Retro Game is a LED Matrix Arduino game which expands the original Space Invaders and Chicken Invaders. 

## How to play
On the LCD will appear the menu with the options: START GAME, SETTINGS, HIGHSCORE and INFO(each options can be changed by moving with the xAxis of the joystick and pressing the swicth button). In settings, each player can set the starting level and the its name (each setting will be modified after the press of the switch button). Pay attention if you choosed to start from a another level(default is 1) the dificulty is increasing and the score will start from zero. In highscore will be displayed the name of the player who get the best score and his score.
The ship can be moved only with the joystick on x-axis direction and at each press of the switch button it will fire rackets.
The difficulty of the game will increase with each level(enmies starships will increase in speed). Any level completed without any damage caused by enemy ship will be give you a special power and bonus score. The special power can be used anytime by pressing the pushbutton.

## Game specifications

### Sound
Sounds plays an important role in the user experience by adding another layer of depth; making fo a more realistic experience. To acomplish this I used a passive buzzer - Notes [pitches](pitches.h) 


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
