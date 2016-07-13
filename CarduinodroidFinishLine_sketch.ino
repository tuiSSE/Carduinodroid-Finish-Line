/* Carduinodroid Finish Line


  Carduinodroid is an RC model car used as an attraction at the
  Open Day event where visitors try to beat a high score for a course.
  The task is to build a sensitive start/finishing line,
  measure the time needed using an Arduino, and a display
  to show the finish time.


*/
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT2.h>
#include <DS1307.h>

#define ledPin  13                  // LED connected to digital pin 13

#define buttonPin 2                 // button on pin 2

MI0283QT2 lcd;  //MI0283QT2 Adapter v1
DS1307 rtc;

int value = LOW;                    // previous value of the LED

int buttonState;                    // variable to store button state

int lastButtonState;                // variable to store last button state

int blinking;                       // condition for blinking - timer is timing

long interval = 100;                // blink interval - change to suit

long previousMillis = 0;            // variable to store last time LED was updated

long startTime ;                    // start time for stop watch

long elapsedTime ;                  // elapsed time for stop watch

int fractional;                     // variable used to store fractional part of time

const int buzzer = 5;

void setup()

{
  //init Serial port
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);         // sets the digital pin as output
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);   // turn on pullup resistors. Wire button so that press shorts pin to ground.

  //init Display
  Serial.println("Init Display...");
  lcd.begin();
  lcd.fillScreen(RGB(255, 255, 255));

  pinMode(buzzer, OUTPUT); // Set buzzer - pin 5 as an output


}



void loop()

{

  uint8_t sec, min, hour, day, month;

  char buf[16];

  // check for button press

  buttonState = digitalRead(buttonPin);                   // read the button state and store



  if (buttonState == LOW && lastButtonState == HIGH  &&  blinking == false) {    // check for a high to low transition

    // if true then found a new button press while clock is not running - start the clock



    startTime = millis();                                   // store the start time

    lcd.fillScreen(RGB(255, 255, 255));

    Serial.println("RACE STARTED");
     Serial.println(elapsedTime);
    lcd.drawText(20, 100, "RACE STARTED", RGB(0, 0, 0), RGB(25, 255, 255), 3);

    //Beep to signal start

    buzzerBeep();
    blinking = true;                                     // turn on blinking while timing

    delay(5);                                               // short delay to debounce switch

    lastButtonState = buttonState;                          // store buttonState in lastButtonState, to compare next time




  }



  else if (buttonState == LOW && lastButtonState == HIGH && blinking == true) {    // check for a high to low transition

    // if true then found a new button press while clock is running - stop the clock and report



    elapsedTime =   millis() - startTime;              // store elapsed time

    // ignore button press in first 10 seconds,
    // to avoid button press due to second tyre moving over

    if (elapsedTime > 10000)
    {

      blinking = false;                                  // turn off blinking, all done timing

      lastButtonState = buttonState;                     // store buttonState in lastButtonState, to compare next time

      //Beep to signal finish
      buzzerBeep();

      // routine to report elapsed time

      Serial.print( (int)(elapsedTime / 1000L));         // divide by 1000 to convert to seconds - then cast to an int to print



      Serial.print(".");                             // print decimal point



      // use modulo operator to get fractional part of time

      fractional = (int)(elapsedTime % 1000L);


      // pad in leading zeros

      if (fractional == 0)

        Serial.print("000");      // add three zero's

      else if (fractional < 10)    // if fractional < 10 the 0 is ignored giving a wrong time, so add the zeros

        Serial.print("00");       // add two zeros

      else if (fractional < 100)

        Serial.print("0");        // add one zero


      Serial.println(fractional);  // print fractional part of time

      Serial.println("seconds");
      Serial.println("FINISH");

      lcd.fillScreen(RGB(255, 255, 255));

      lcd.drawText(50, 50, "FINISH", RGB(0, 0, 0), RGB(25, 255, 255), 5);
      //display output
      sprintf(buf, " %02i.%02i seconds",  (int)(elapsedTime / 1000L), fractional);
      Serial.println(buf);
      lcd.drawText(50, 100, buf , RGB(0, 0, 0), RGB(255, 255, 255), 2);


      //wait 10 seconds
      delay(10000);

    }

  }

  else {

    lastButtonState = buttonState;                         // store buttonState in lastButtonState, to compare next time

  }



  // blink routine - blink the LED while timing

  // check to see if it's time to blink the LED; that is, the difference

  // between the current time and last time we blinked the LED is larger than

  // the interval at which we want to blink the LED.



  if ( (millis() - previousMillis > interval) ) {



    if (blinking == true) {

      previousMillis = millis();                         // remember the last time we blinked the LED



      // if the LED is off turn it on and vice-versa.

      if (value == LOW)

        value = HIGH;


      else

        value = LOW;

      digitalWrite(ledPin, value);

    }


    else {

      digitalWrite(ledPin, LOW);                         // turn off LED when not blinking

    }

  }



}

void buzzerBeep()
{
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);

  return;
}

