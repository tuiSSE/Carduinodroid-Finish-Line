# Carduinodroid-Finish-Line

Carduinodroid is an RC model car used as an attraction at the TU Ilmenau Open Day event where visitors try to beat a high score for a course. The task is to build a sensitive start/finishing line, measure the time needed using an Arduino, and a display to show the finish time. 

Components:
Arduino UNO
Watterott MSD Shield
Watterott MI0283QT v1 - 2.8" LCD + Touchpanel 
Pushbuttons
Piezzo Buzzer

Implementation: 

Circuit:
![Circuit](https://github.com/phba1682/Carduinodroid-Finish-Line/blob/master/breadboard.PNG)

Schematic:
![Schematic](https://github.com/phba1682/Carduinodroid-Finish-Line/blob/master/schematic.PNG)

Set-up:
![First prototype set-up](https://github.com/bthara/Carduinodroid-Finish-Line/blob/master/CarduinodroidPrototype_img1.jpg)
![Finish Line](https://github.com/bthara/Carduinodroid-Finish-Line/blob/master/finishline.jpg)

LCD Display:

![LCD Display-2](https://github.com/bthara/Carduinodroid-Finish-Line/blob/master/racestarted.JPG)

![LCD Display-2](https://github.com/bthara/Carduinodroid-Finish-Line/blob/master/racefinish1.JPG)


The “CarduinodroidFinishLine_sketch” continually reads the button's state from pin 2. When one of the buttons is pressed for the first time, i.e., buttonState is HIGH then the timer is started. When the car moves over the finish line for the second time,ie buttons are pressed for a second time the state change is detected and timer is stopped.The elapsed time is then displayed on a LCD display.A 1KHz sound signal using a piezzo buzzer is used to signal the finish of the race.
A change in state is ignored for a time interval of 10 seconds in order to avoid the issue of timer being stoped when the back tyres of RC car passes over the line.






