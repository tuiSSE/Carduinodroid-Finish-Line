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

#define buttonPin 2
#define buzzerPin 5
#define ledPin 13

MI0283QT2 lcd;
DS1307 rtc;

int value = LOW;            // previous value of the LED
int buttonState;            // variable to store button state
int lastButtonState;        // variable to store last button state
long interval = 100;        // blink interval - change to suit
long previousMillis = 0;    // variable to store last time LED was updated
long startTime;             // start time for stop watch
long elapsedTime;           // elapsed time for stop watch
int fractional;             // variable used to store fractional part of time
long highscore = -1;

// 0 - standby
// 1 - running
// 2 - finished
int state;               // condition for blinking - timer is timing

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);   // turn on pullup resistors. Wire button so that press shorts pin to ground.

  Serial.println("Init Display...");
  lcd.begin();
  lcd.fillScreen(RGB(200, 200, 0));
  lcd.drawText(60, 100, "carduinodroid", RGB(0, 0, 0), RGB(200, 200, 0), 2);
  lcd.drawText(20, 200, "Pass finish line to start", RGB(0, 0, 0), RGB(200, 200, 0), 1);
}

void loop() {
  uint8_t sec, min, hour, day, month;
  char buf[16];

  // check for button press
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    if (state == 0) {    // check for a high to low transition
      // if true then found a new button press while clock is not running - start the clock
      startTime = millis();
      lcd.fillScreen(RGB(200, 200, 0));
      Serial.println("RACE STARTED");
      Serial.println(elapsedTime);
      lcd.drawText(20, 100, "RACE STARTED", RGB(0, 0, 0), RGB(200, 200, 0), 3);
      //Beep to signal start
      buzzerBeep();
      state = 1;                                     // turn on blinking while timing
      delay(5);                                               // short delay to debounce switch
      lastButtonState = buttonState;                          // store buttonState in lastButtonState, to compare next time
    }
    else if (state == 1) {    // check for a high to low transition
      // if true then found a new button press while clock is running - stop the clock and report
      elapsedTime =   millis() - startTime;              // store elapsed time
      // ignore button press in first 1 seconds,
      // to avoid button press due to second tyre moving over
      if (elapsedTime > 1000)
      {
        state = 2;                                  // turn off blinking, all done timing
        lastButtonState = buttonState;                     // store buttonState in lastButtonState, to compare next time
        // Beep to signal finish
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

        lcd.fillScreen(RGB(0, 255, 0));

        lcd.drawText(70, 100, "FINISHED", RGB(0, 0, 0), RGB(0, 255, 0), 3);
        //display output
        sprintf(buf, "Seconds: %02i.%02i",  (int)(elapsedTime / 1000L), fractional);
        Serial.println(buf);
        lcd.drawText(35, 175, buf , RGB(0, 0, 0), RGB(0, 255, 0), 2);

        if (highscore == -1 || elapsedTime < highscore) {
          lcd.drawText(50, 130, "New Highscore!", RGB(0, 0, 0), RGB(255, 100, 100), 2);
          highscore = elapsedTime;
        }

        delay(1000);
      }
    }
    else if (state == 2) {
      lcd.fillScreen(RGB(200, 200, 0));
      lcd.drawText(60, 100, "carduinodroid", RGB(0, 0, 0), RGB(200, 200, 0), 2);
      lcd.drawText(20, 200, "Pass finish line to start", RGB(0, 0, 0), RGB(200, 200, 0), 1);
      state = 0;
      if (highscore > 0) {
        fractional = (int)(highscore % 1000L);
        sprintf(buf, "Highscore: %02i.%01i",  (int)(highscore / 1000L), fractional);
        lcd.drawText(20, 160, buf , RGB(0, 0, 0), RGB(200, 200, 0), 2);
      }
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
    if (state == 1) {
      previousMillis = millis();                         // remember the last time we blinked the LED
      // if the LED is off turn it on and vice-versa.
      if (value == LOW)
        value = HIGH;
      else
        value = LOW;
      digitalWrite(ledPin, value);
      elapsedTime = millis() - startTime;
      fractional = (int)(elapsedTime % 1000L);
      sprintf(buf, "Seconds: %02i.%01i",  (int)(elapsedTime / 1000L), fractional);
      lcd.drawText(35, 175, buf , RGB(0, 0, 0), RGB(200, 200, 0), 2);
    }
    else {
      digitalWrite(ledPin, LOW);                         // turn off LED when not blinking
    }
  }
}

void buzzerBeep()
{
  tone(buzzerPin, 1000); // Send 1KHz sound signal...
  delay(200);           // ...for 1 sec
  noTone(buzzerPin);     // Stop sound...
  //delay(1000);

  return;
}

