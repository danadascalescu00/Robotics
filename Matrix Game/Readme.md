# Space Invaders LED Matrix Game

## About the game
Space Invaders Retro Game is a LED Matrix Arduino game which expands the original Space Invaders and Chicken Invaders. 

## How to play
On the LCD will appear the menu with the options: START GAME, SETTINGS, HIGHSCORE and INFO(each options can be changed by moving with the xAxis of the joystick and pressing the swicth button). In settings, each player can set the starting level and the its name (each setting will be modified after the press of the switch button). Pay attention if you choosed to start from a another level(default is 1) the dificulty is increasing and the score will start from zero. In highscore will be displayed the name of the player who get the best score and his score.
The ship can be moved only with the joystick on x-axis direction and at each press of the switch button it will fire rackets.
The difficulty of the game will increase with each level(enmies starships will increase in speed). Any level completed without any damage caused by enemy ship will be give you a special power and bonus score. The special power can be used anytime by pressing the pushbutton.

## Game specifications

### Scoring
* Player's score depends on the number of enemy shipes destroyed. Each player receives a bonus if he can reache the next level without damage caused by the enemy.
* The last level, when the big boss will appear, brings another chances of receiving bonus score.
* The first three best scores are saved along with the player's name in the *EEPROM*.

### Sound
Sounds plays an important role in the user experience by adding another layer of depth; making for a more realistic experience. To acomplish this I used a passive buzzer - Notes [pitches](https://github.com/danadascalescu00/Robotics/blob/master/Matrix%20Game/Space%20Invaders/pitches.h) 

### Input/Output
* The joystick is used to navigate in the menu(by moving with the xAxis or yAxis). Each chose is made by pressing the switch of the joystick. In the game the joystick is used to move the player's starship and, also, to fire rackets.
* The pushbutton is ued for exiting the options choosed. In game is used to activate a different firing mechanism.
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
