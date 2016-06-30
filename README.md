# Carduinodroid-Finish-Line

Carduinodroid is an RC model car used as an attraction at the Open Day event where visitors try to beat a high score for a course. The task is to build a sensitive start/finishing line, measure the time needed using an Arduino, and a display to show the finish time. 


Implementation – First Prototype


Circuit:


Schematic:


First prototype set-up:
![First prototype set-up](https://github.com/bthara/Carduinodroid-Finish-Line/blob/master/CarduinodroidPrototype_img1.jpg)
![First prototype set-up 2](https://github.com/bthara/Carduinodroid-Finish-Line/blob/master/CarduinodroidPrototype_img2.jpg)
Screenshot:

![Serial Monitor](https://github.com/bthara/Carduinodroid-Finish-Line/blob/master/Carduinodroid_serialmonitorresult.png)

The “CarduinodroidFinishLine_sketch” continually reads the button's state from pin 2. When one of the buttons is pressed for the first time, i.e., buttonState is HIGH then the timer is started. When the car moves over the finish line for the second time,ie buttons are pressed for a second time the state change is detected and timer is stopped.The elapsed time is then displayed.
A change in state is ignored for a time interval of 10 seconds in order to avoid the issue of timer being stoped when the back tyres of RC car passes over the line.Additionally, a blink routine is implemented which blinks the LED on the Arduino pin 13 while timer is running.






