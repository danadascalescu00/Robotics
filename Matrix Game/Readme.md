# Space Invaders LED Matrix Game

## About the game
Space Invaders Retro Game is a LED Matrix Arduino game which expands the original Space Invaders and Chicken Invaders. 

## How to play
The LCD will display a menu with the following options: START GAME, SETTINGS, HIGHSCORE, and INFO (each options can be changed by moving with the xAxis of the joystick and pressing the swicth button). Each player can choose the initial level and its name in the settings (each setting will be modified after the press of the switch button). If you opt to begin at a different level (the default is 1), the difficulty will increase and your score will begin at zero. The name of the player who received the highest score, as well as his score, will be displayed in highscore.
The spacecraft can only be maneuvered along the x-axis with the joystick, and each switch button hit fires rackets.
The game's difficulty will increase with each level (enemies' starships will increase in speed). Any level finished without being damaged by an opponent spaceship will grant you a special power and a bonus score. The special ability can be activated at any time by pushing the pushbutton.

## Game specifications

### Scoring
* Player's score depends on the number of enemy shipes destroyed. Each player receives a bonus if he can reache the next level without damage caused by the enemy.
* The last level, when the big boss will appear, brings another chances of receiving bonus score.
* The first three best scores are saved along with the player's name in the *EEPROM*.

### Sound
Sounds plays an important role in the user experience by adding another layer of depth; making for a more realistic experience. To acomplish this I used a passive buzzer - Notes [pitches](https://github.com/danadascalescu00/Robotics/blob/master/Matrix%20Game/Space%20Invaders/pitches.h) 

### Input/Output
* The joystick is used to navigate in the menu (by moving with the xAxis or yAxis). Each chose is made by pressing the switch of the joystick. In the game the joystick is used to move the player's starship and, also, to fire rackets.
* The pushbutton is used to exit the options that have been selected. It is also utilized in the game to activate a different firing mechanism.
* The LCD and the led matrix are used for the interface;


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

##### Presentation video:
https://www.youtube.com/watch?v=JFGcheNMls0&feature=youtu.be
