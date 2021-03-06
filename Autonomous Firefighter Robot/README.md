# Autonomous Firefighter Robot
&nbsp; Final project for the Introduction to Robotics Course held by the Robotics Department at the Faculty of Mathematics and Computer Science,
University of Bucharest.

## Table of contents
* [About the project](#about-the-project)
* [Features](#features)
* [Hardware Specifications and Technical Drawings for 3D printing](#hardware-specifications-and-technical-drawings-for-3d-printing)
* [Tehnical information](#tehnical-information)


## About the project
&nbsp; &nbsp; &nbsp; In the United States, a country which has one of the higher fire death rates, are estimated 1.318.500 fires in 2018. These fires resulted in 3.655 civilian fire fatalities, 15.200 civilian fire injuries and an estimated $25.6 billion in direct property(including $12.4 billion in losses from major California wildfires) according to the United States Fire Departments.  

&nbsp; &nbsp; &nbsp; In an event of a fire breakout, fire spreads rapidly if it is not controlled and, in case of a gas leakage there perhaps be an explosion, so firefighters are constantly at risk of losing their life. To overcome this issue, save the life of our heroes, our autonomous firefighter robot comes to rescue. 

## Features:
&nbsp; Autonomous firefighter robot, which can avoid obstacle and, instantly,can detect a fire, send an alert message(using the wi-fi module incorporated in Arduino Mega) and activate the emergency mode. The emergency mode consists of self-driving until it reaches the flame and starting the water pump mechanism. The water pump mechanism consit from a mini-water pump, a 5V relay module with Status Led, diode and transistor, one servo and one microservo( the servos control the movement of the pump).
#### To do:
Alert messages using the wi-fi module.

##### Presentation video: https://www.youtube.com/watch?v=FE0DiZv4Mc4&feature=youtu.be

## Hardware Specifications and Technical Drawings for 3D Printing:

#### Components:
* Arduino Mega 2560 microcontroller 
* Wires
* 4 X Wheel
* 4 x Ultrasonic Distance Sensor - HC-SR04
* 4 x DC Motor
* Servo Motor
* Micro Servor
* Water pump 3-6V 
* 2 x Infrared sensor module LM393
* Battery 9V
* 3 x Battery 7,4 V
* MotorDriver Shield 239D
* 5V relay module with Status Led, diode and transistor
* Buzzer
* Switch on|off button

#### 3D Models: [models](https://github.com/danadascalescu00/Robotics/tree/master/Autonomous%20Firefighter%20Robot/3D%20Models)
![picture](Images/wpdrawing.jpeg)
![picture](Images/wpdesign.jfif)


## Tehnical information
<em>Ultrasonic Distance Sensor - HC-SR04 </em>
 
&ensp; This sensor provides 2cm to 400cm of non-contact measurement functionality with a ranging accuracy that can reach up to 3mm. Each HC-SR04 module includes an ultrasonic transmitter, a receiver and a control circuit.<br>

**Determine the distance:** We use thre distance servo for detecting objects and find the distance till obstacles(obstacles avoiding mechanism).

**Determine water level from the tank:** We used the ultrasonic distance sensor HC-SR04 which sends the sound waves in the water tank and detects reflection of sound waves that is ECHO. The system measures the time between triggered sound wave and received ECHO, and calculates the distance(d) to the water surface using given formula: <br>
 &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; ![picture](Images/CodeCogsEqn.png) <br>
  &emsp; &emsp; &emsp; &emsp; &emsp;where Δt is the travel distance time and c the speed of sound (distance travel per unit time by a sound wave as it propagates through an elastic medium - at approximatively 20 °C is 343 meters per second). <br>
  As we know the height of water tank we can calculate the water level by subtracting resulting distance measure with the ultrasonic sensor from total length of tank. 
  
  <em>Servo motors</em> <br>
  &ensp; For the water gun mechanism what we needed was to have motors that can provide relatively precise motion in a specific range. In order to achieve this we could have used servo motors or stepper motors. We choosed to use a servo motors and a micro sevo considering the following facts: less consumption of energy which was important in our case and that servo motors are more capable of accelerating loads than steppers. <br>
 <br>  <em>Flame sensors</em> <br>
  &ensp; These types of sensors are used for short range fire detection.I have found taht the sensors are mostly accurate up to about 3 feet. The flame sensor is very sensitive to IR wavelength at 760 nm ~ 1100 nm light. <br>
  &ensp; The sensors are connected to analog pins of the <em>Arduino Mega microcontroller<em> for real-time output voltage signal on the thermal resistance. Their function is to send command for stopping the vehivcle at a suitablle distance from the fire and starting the water pump mechanism(pumping water and moving the water gun to put the fire off). <br>
  
 &ensp; We choose an <em> Arduino Mega </em> because we are interested to expand the design and add some moore features.

<em>Buzzer</em> <br>
&ensp; Sounds play an important role because is one of the features which is able to announce people around that a fire broke out. <br>
``` void playSiren() {
  if (ascending) {
    if (hz < 1000) {
      if (millis() - lastSiren >= 5) {
        lastSiren = millis();
        tone(buzzerPin, hz++, sirenDelay);
      }
    } else {
      ascending = false;
    }
  } else {
    if (hz >= 440) {
      if (millis() - lastSiren >= 5) {
        lastSiren = millis();
        tone(buzzerPin, hz--, sirenDelay);
      }
    } else {
      ascending = true;
    }
  }
}
```
 
