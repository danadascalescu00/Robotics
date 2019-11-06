
              __Laboratory Homework__
              
***I. Controlling an RGB Led using 3 potentiometers***

**1. Introduction**
  An RGB Led has three tiny leds of 3 primary colors(red, green and blue) where a terminal is common for all, some of them have positive terminal, known as anode, and some of them have negative anode, known as terminal. When different voltage is applied to different LEDs, they make a mixture and produce everal thousands of colors.(*This example I used a RGB Led with  a common negative terminal or cathode*)
  
**2. CIRCUIT for the RGB LED with a common cathode:**
    - First of all I checked if my pin has a common cathode or anode with a multimeter. ( To check if is a positive or a negative terminal: 
 You can set it to diode and use - on the long pin and + on a small one. If it lights up, it’s common cathode. Otherwise, use + on the long pin and - on a small one, if it lights up now, then it’s common anode. )
    - The common cathode is connected to the ground and each LED's anode through a resistor to the output pin
    
